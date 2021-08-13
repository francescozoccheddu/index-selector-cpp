#include <index-selector-lib/variable_matrix.hpp>
#include <stdexcept>
#include <format>

namespace IndexSelector
{

	VariableMatrix::VariableMatrix (const IloEnv _env, const Problem& _problem, bool _prune) : m_problem{ _problem }
	{
		std::string name{ 13 };
		const size_t ni{ _problem.nIndices () }, nq{ _problem.nQueries () };
		std::optional<IloBoolVar>* const xs = new std::optional<IloBoolVar>[ni * nq];
		std::optional<IloBoolVar>* const ys = new std::optional<IloBoolVar>[ni];
		IloBoolVar* const uxs = new IloBoolVar[nq];
		for (size_t i{ 0 }; i < ni; i++)
		{
			const Index& index{ _problem.indices[i] };
			if (!_prune || index.size <= _problem.maxSize)
			{
				bool valid{ false };
				for (size_t q{ 0 }; q < nq; q++)
				{
					if (!_prune || index.queryCosts[q] < _problem.unindexedQueryCosts[q])
					{
						m_nActiveXs++;
						valid = true;
						name.clear ();
						std::format_to (std::back_inserter (name), "i{}_q{}_x", i, q);
						xs[i * nq + q] = IloBoolVar{ _env , name.c_str() };
					}
				}
				if (!_prune || valid)
				{
					m_nActiveYs++;
					name.clear ();
					std::format_to (std::back_inserter (name), "i{}_y", i);
					ys[i] = IloBoolVar{ _env , name.c_str() };
				}
			}
		}
		for (size_t q{ 0 }; q < nq; q++)
		{
			name.clear ();
			std::format_to (std::back_inserter (name), "q{}_ux", q);
			uxs[q] = IloBoolVar{ _env , name.c_str () };
		}
		m_uxs = ImmutableArray<IloBoolVar>::take_ownership (uxs, nq);
		m_xs = ImmutableArray<std::optional<IloBoolVar>>::take_ownership (xs, ni * nq);
		m_ys = ImmutableArray<std::optional<IloBoolVar>>::take_ownership (ys, ni);
	}

	const std::optional<IloBoolVar>& VariableMatrix::x (int _i, int _q) const
	{
		if (_i < 0 || _q < 0 || _i >= m_problem.nIndices () || _q >= m_problem.nQueries ())
		{
			throw std::invalid_argument ("Out of range");
		}
		return m_xs[_i * m_problem.nQueries () + _q];
	}

	const std::optional<IloBoolVar>& VariableMatrix::y (int _i) const
	{
		return m_ys[_i];
	}

	const IloBoolVar& VariableMatrix::ux (int _i) const
	{
		return m_uxs[_i];
	}

	const Problem& VariableMatrix::problem () const
	{
		return m_problem;
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