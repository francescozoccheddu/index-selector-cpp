#ifndef INCLUDED_INDEX_SELECTOR_APP_RANDOM_PROBLEM
#define INCLUDED_INDEX_SELECTOR_APP_RANDOM_PROBLEM

#include <index-selector/problem.hpp>
#include <concepts>

namespace IndexSelector
{

	struct RandomProblemOptions final
	{

		int nQueries{ 20 };
		int nIndices{ 30 };
		double indexSizeDev{ 5 };
		double maxSizeRatio{ 0.1 };
		double indexFixedCostDev{ 5 };
		double indexFixedCostRatio{ 0.5 };
		double queryCostDev{ 5 };
		double indexedQueryCostRatio{ 0.75 };

		void validate () const;

	};

	Problem randomProblem (const RandomProblemOptions& _options, unsigned int _seed = 0);

}

#endif