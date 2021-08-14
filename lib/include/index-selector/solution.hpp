#ifndef INCLUDED_INDEX_SELECTOR_SOLUTION
#define INCLUDED_INDEX_SELECTOR_SOLUTION

#include <index-selector/types.hpp>
#include <index-selector/immutable_array.hpp>
#include <index-selector/problem.hpp>
#include <index-selector/options.hpp>

namespace IndexSelector
{

	struct Solution final
	{

		struct Statistics final
		{

			size_t nNodes{};
			size_t nVariables{};
			size_t nConstraints{};
			size_t nSelectionCuts{};
			size_t nSizeCuts{};
			double totalElapsedTime{};
			double selectionCutElapsedTime{};
			double sizeCutElapsedTime{};

		};

		ImmutableArray<int> enabledIndices{};
		ImmutableArray<int> bestIndexPerQuery{};
		Real totalCost{};
		Problem problem{};
		Options options{};
		Statistics statistics{};

	};

}

#endif