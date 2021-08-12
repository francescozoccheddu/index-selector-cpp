#include <index-selector/index.hpp>

#include <stdexcept>

namespace IndexSelector
{

	Index::Index (Real _size, Real _fixedCost, ImmutableArray<Real> _queryCosts) : size{ _size }, fixedCost{ _fixedCost }, queryCosts{ _queryCosts }
	{
		if (_size < 0)
		{
			throw std::invalid_argument{ "Negative size" };
		}
		if (_fixedCost < 0)
		{
			throw std::invalid_argument{ "Negative fixed cost" };
		}
		for (Real qc : _queryCosts)
		{
			if (qc < 0)
			{
				throw std::invalid_argument{ "Negative query cost" };
			}
		}
	}

	int Index::nQueries () const
	{
		return queryCosts.size;
	}

}