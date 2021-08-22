#ifndef INCLUDED_INDEX_SELECTOR_APP_RANDOM_PROBLEM
#define INCLUDED_INDEX_SELECTOR_APP_RANDOM_PROBLEM

#include <index-selector/problem.hpp>
#include <concepts>

namespace IndexSelector::App
{

	struct RandomProblemOptions final
	{

		int nQueries{ 50 };
		int nIndices{ 40 };

		double nIndicesMaxSize{ 5 };
		double indexFixedCostRatio{ 0.1 };
		double indexQueryCostRatio{ 0.75 };

		double indexFixedCostDev{ 5 };
		double indexSizeDev{ 5 };
		double queryCostDev{ 5 };

		void validate () const;

	};

	Problem randomProblem (const RandomProblemOptions& _options, unsigned int _seed = 0);

}

#endif