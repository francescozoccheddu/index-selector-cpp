#ifndef INCLUDED_INDEX_SELECTOR_APP_RANDOM_PROBLEM
#define INCLUDED_INDEX_SELECTOR_APP_RANDOM_PROBLEM

#include <index-selector/problem.hpp>
#include <concepts>

namespace IndexSelector::App
{

	struct RandomProblemOptions final
	{

		int nQueries{ 20 };
		int nIndices{ 30 };
		double indexSizeDev{ 10 };
		double maxSizeRatio{ 0.1 };
		double indexFixedCostDev{ 10 };
		double indexFixedCostRatio{ 0.4 };
		double queryCostDev{ 10 };
		double indexQueryCostRatio{ 0.9 };

		void validate () const;

	};

	Problem randomProblem (const RandomProblemOptions& _options, unsigned int _seed = 0);

}

#endif