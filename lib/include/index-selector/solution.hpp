#ifndef INCLUDED_INDEX_SELECTOR_SOLUTION
#define INCLUDED_INDEX_SELECTOR_SOLUTION

#include <index-selector/types.hpp>
#include <index-selector/immutable_array.hpp>
#include <index-selector/problem.hpp>
#include <index-selector/options.hpp>

#define INDEX_SELECTOR_MEASURE_TIME 1

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
#if INDEX_SELECTOR_MEASURE_TIME
			double selectionCutElapsedTime{};
			double sizeCutElapsedTime{};
#endif

		};

		ImmutableArray<size_t> enabledIndices{};
		ImmutableArray<size_t> bestIndexPerQuery{};
		bool succeeded{};
		Real cost{};
		Problem problem{};
		Options options{};
		Statistics statistics{};

	};

}

#endif