#ifndef INCLUDED_INDEX_SELECTOR_PROBLEM
#define INCLUDED_INDEX_SELECTOR_PROBLEM

#include <index-selector/types.hpp>
#include <index-selector/index.hpp>
#include <index-selector/immutable_array.hpp>

namespace IndexSelector
{

	struct Problem final
	{

		ImmutableArray<Real> unindexedQueryCosts;
		Real maxSize;
		ImmutableArray<Index> indices;

		int nQueries () const;
		void validate () const;

	};

}

#endif