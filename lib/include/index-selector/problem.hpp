#ifndef INCLUDED_INDEX_SELECTOR_PROBLEM
#define INCLUDED_INDEX_SELECTOR_PROBLEM

#include <index-selector/types.hpp>
#include <index-selector/index.hpp>
#include <index-selector/immutable_array.hpp>

namespace IndexSelector
{

	class Problem
	{

	public:

		const ImmutableArray<Real> unindexedQueryCosts;
		const Real maxSize;
		const ImmutableArray<Index> indices;

		Problem (Real _maxSize, ImmutableArray<Real> _unindexedQueryCosts, ImmutableArray<Index> _indices);

		int nQueries () const;

	};

}
	
#endif