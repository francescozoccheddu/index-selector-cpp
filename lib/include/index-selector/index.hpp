#ifndef INCLUDED_INDEX_SELECTOR_INDEX
#define INCLUDED_INDEX_SELECTOR_INDEX

#include <index-selector/types.hpp>
#include <index-selector/immutable_array.hpp>

namespace IndexSelector
{

	class Index
	{

	public:

		const ImmutableArray<Real> queryCosts;
		const Real size;
		const Real fixedCost;

		Index (Real _size, Real _fixedCost, ImmutableArray<Real> _queryCosts);

		int nQueries () const;

	};

}
	
#endif