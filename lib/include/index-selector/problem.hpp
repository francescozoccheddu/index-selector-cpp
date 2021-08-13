#ifndef INCLUDED_INDEX_SELECTOR_PROBLEM
#define INCLUDED_INDEX_SELECTOR_PROBLEM

#include <index-selector/types.hpp>
#include <index-selector/index.hpp>
#include <index-selector/immutable_array.hpp>

namespace IndexSelector
{

	struct Problem final
	{

		Real maxSize{};
		ImmutableArray<Real> unindexedQueryCosts{};
		ImmutableArray<Index> indices{};

		size_t nQueries () const;
		size_t nIndices () const;
		void validate () const;

	};

}

#endif