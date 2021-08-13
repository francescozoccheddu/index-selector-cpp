#include <index-selector-lib/variable_matrix.hpp>
#include <stdexcept>

namespace IndexSelector
{

	VariableMatrix::VariableMatrix (const IloEnv& _env, const Problem& _problem, bool _prune) : m_nIndices{ _problem.nIndices () }, m_nQueries{ _problem.nQueries () }
	{
		std::optional<IloBoolVar>* xs = new std::optional<IloBoolVar>[m_nIndices * m_nQueries];
		std::optional<IloBoolVar>* ys = new std::optional<IloBoolVar>[m_nIndices];
		IloBoolVar* uxs = new IloBoolVar[m_nQueries];
		for (size_t i{ 0 }; i < m_nIndices; i++)
		{
			const Index& index{ _problem.indices[i] };
			if (!_prune || index.size <= _problem.maxSize)
			{
				bool valid{ false };
				for (size_t q{ 0 }; q < m_nQueries; q++)
				{
					if (!_prune || index.queryCosts[q] < _problem.unindexedQueryCosts[q])
					{
						m_nActiveXs++;
						valid = true;
						xs[i * m_nQueries + q] = IloBoolVar{ _env , nullptr };
					}
				}
				if (!_prune || valid)
				{
					m_nActiveYs++;
					ys[i] = IloBoolVar{ _env , nullptr };
				}
			}
		}
		for (size_t q{ 0 }; q < m_nQueries; q++)
		{
			uxs[q] = IloBoolVar{ _env , nullptr };
		}
		m_uxs = ImmutableArray<IloBoolVar>::from_immutable_data (uxs, m_nQueries);
		m_xs = ImmutableArray<std::optional<IloBoolVar>>::from_immutable_data (xs, m_nIndices * m_nQueries);
		m_ys = ImmutableArray<std::optional<IloBoolVar>>::from_immutable_data (ys, m_nIndices);
	}

	const std::optional<IloBoolVar>& VariableMatrix::x (int _i, int _q) const
	{
		if (_i < 0 || _q < 0 || _i >= m_nIndices || _q >= m_nQueries)
		{
			throw std::invalid_argument ("Out of range");
		}
		return m_xs[_i * m_nQueries + _q];
	}

	const std::optional<IloBoolVar>& VariableMatrix::y (int _i) const
	{
		return m_ys[_i];
	}

	const IloBoolVar& VariableMatrix::ux (int _i) const
	{
		return m_uxs[_i];
	}

	size_t VariableMatrix::nIndices () const
	{
		return m_nIndices;
	}

	size_t VariableMatrix::nQueries () const
	{
		return m_nQueries;
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