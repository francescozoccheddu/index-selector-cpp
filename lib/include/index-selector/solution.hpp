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

			int nNodes{};
			int nVariables{};
			int nOriginalConstraints{};
			int nC1Constraints{};
			int nC2Constraints{};
			double totalElapsedTime{};
			double C1ElapsedTime{};
			double C2ElapsedTime{};

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