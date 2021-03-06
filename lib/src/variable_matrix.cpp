#include <index-selector-lib/variable_matrix.hpp>
#include <stdexcept>
#include <format>

namespace IndexSelector
{

	VariableMatrix::VariableMatrix (const IloEnv _env, const Problem& _problem, bool _reduceVariables) : m_pProblem{ &_problem }
	{
		std::string name{ 13 };
		const size_t ni{ _problem.nIndices () }, nq{ _problem.nQueries () };
		std::optional<IloBoolVar>* const xs = new std::optional<IloBoolVar>[ni * nq];
		std::optional<IloBoolVar>* const ys = new std::optional<IloBoolVar>[ni];
		IloBoolVar* const uxs = new IloBoolVar[nq];
		for (size_t i{ 0 }; i < ni; i++)
		{
			const Index& index{ _problem.indices[i] };
			if (!_reduceVariables or index.size <= _problem.maxSize)
			{
				bool valid{ false };
				for (size_t q{ 0 }; q < nq; q++)
				{
					if (!_reduceVariables or index.queryCosts[q] < _problem.unindexedQueryCosts[q])
					{
						m_nActiveXs++;
						valid = true;
						name.clear ();
						std::format_to (std::back_inserter (name), "i{}_q{}_x", i, q);
						xs[i * nq + q] = IloBoolVar{ _env , name.c_str () };
					}
				}
				if (!_reduceVariables or valid)
				{
					m_nActiveYs++;
					name.clear ();
					std::format_to (std::back_inserter (name), "i{}_y", i);
					ys[i] = IloBoolVar{ _env , name.c_str () };
				}
			}
		}
		for (size_t q{ 0 }; q < nq; q++)
		{
			name.clear ();
			std::format_to (std::back_inserter (name), "q{}_ux", q);
			uxs[q] = IloBoolVar{ _env , name.c_str () };
		}
		m_uxs = ImmutableArray<IloBoolVar>::takeOwnership (uxs, nq);
		m_xs = ImmutableArray<std::optional<IloBoolVar>>::takeOwnership (xs, ni * nq);
		m_ys = ImmutableArray<std::optional<IloBoolVar>>::takeOwnership (ys, ni);
	}

	const std::optional<IloBoolVar>& VariableMatrix::x (size_t _i, size_t _q) const
	{
		const Problem& p = problem ();
		if (_i < 0 or _q < 0 or _i >= p.nIndices () or _q >= p.nQueries ())
		{
			throw std::invalid_argument ("Out of range");
		}
		return m_xs[_i * p.nQueries () + _q];
	}

	const std::optional<IloBoolVar>& VariableMatrix::y (size_t _i) const
	{
		return m_ys[_i];
	}

	const IloBoolVar& VariableMatrix::ux (size_t _i) const
	{
		return m_uxs[_i];
	}

	const Problem& VariableMatrix::problem () const
	{
		return *m_pProblem;
	}

	size_t VariableMatrix::nActiveXs () const
	{
		return m_nActiveXs;
	}

	size_t VariableMatrix::nActiveYs () const
	{
		return m_nActiveYs;
	}

}