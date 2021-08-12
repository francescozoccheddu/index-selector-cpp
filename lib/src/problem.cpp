#include <index-selector/problem.hpp>

#include <stdexcept>

namespace IndexSelector
{

	Problem::Problem (Real _maxSize, ImmutableArray<Real> _unindexedQueryCosts, ImmutableArray<Index> _indices) : maxSize{ _maxSize }, unindexedQueryCosts{ _unindexedQueryCosts }, indices{ _indices }
	{
		if (_maxSize < 0)
		{
			throw std::invalid_argument{ "Negative max size" };
		}
		for (Real qc : _unindexedQueryCosts)
		{
			if (qc < 0)
			{
				throw std::invalid_argument{ "Negative query cost" };
			}
		}
		for (const Index& i : _indices)
		{
			if (i.size != _unindexedQueryCosts.size)
			{
				throw std::invalid_argument{ "Mismatched number of queries between indices" };
			}
		}
	}

	int Problem::nQueries () const
	{
		return unindexedQueryCosts.size;
	}

}