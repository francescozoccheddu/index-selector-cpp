#ifndef INCLUDED_INDEX_SELECTOR_INDEX
#define INCLUDED_INDEX_SELECTOR_INDEX

#include <index-selector/types.hpp>
#include <index-selector/immutable_array.hpp>

namespace IndexSelector
{

	struct Index final
	{

		Real size{};
		Real fixedCost{};
		ImmutableArray<Real> queryCosts{};

		size_t nQueries () const;
		void validate () const;

	};

}

#endif