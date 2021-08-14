#include <iostream>
#include <random>
#include <index-selector/solve.hpp>

using namespace IndexSelector;

Problem eb_problem ()
{
	return Problem{
		.maxSize{19},
		.unindexedQueryCosts{ 6200,2000,800,6700,5000,2000 },
		.indices{
			Index{.size {10}, .fixedCost{200}, .queryCosts{1300, 900, 800, 6700, 5000, 2000}},
			Index{.size {5}, .fixedCost{1200}, .queryCosts{6200, 700, 800, 6700, 5000, 2000}},
			Index{.size {10}, .fixedCost{400}, .queryCosts{6200, 2000, 800, 1700, 2200, 2000}},
			Index{.size {8}, .fixedCost{2400}, .queryCosts{6200, 2000, 800, 6700, 1200, 2000}},
			Index{.size {8}, .fixedCost{250}, .queryCosts{6200, 2000, 800, 2700, 4200, 750}}
		}
	};
}

Problem rand_problem (size_t _nIndices, size_t _nQueries, Real _sizeRatio = 0.1, Real _fixedCostRatio = 0.1, Real _maxMinRatio = 5, Real _unindexedIndexedRatio = 2, unsigned int _seed = 0)
{
	std::mt19937 gen{ _seed };
	std::uniform_real_distribution r = std::uniform_real_distribution<Real>{ 1, _maxMinRatio };
	Problem problem{};
	{
		Real* const ucs = new Real[_nQueries];
		for (int q{ 0 }; q < _nQueries; q++)
		{
			ucs[q] = r (gen) * _unindexedIndexedRatio;
		}
		problem.unindexedQueryCosts = ImmutableArray<Real>::take_ownership (ucs, _nQueries);
	}
	{
		Index* const idxs = new Index[_nIndices];
		Real totalSize{};
		for (int i{ 0 }; i < _nIndices; i++)
		{
			Index& index = idxs[i];
			Real* const cs = new Real[_nQueries];
			for (int q{ 0 }; q < _nQueries; q++)
			{
				cs[q] = r (gen);
			}
			index.queryCosts = ImmutableArray<Real>::take_ownership (cs, _nQueries);
			index.fixedCost = r (gen) * _fixedCostRatio;
			index.size = r (gen);
			totalSize += index.size;
		}
		problem.indices = ImmutableArray<Index>::take_ownership (idxs, _nIndices);
		problem.maxSize = totalSize * _sizeRatio;
	}
	return problem;
}

int main ()
{
	const Solution s = solve (rand_problem (120, 80, 0.02), { .shareCutters{false}, .enableSelectionCuts{false}, .sizeCutMode{Options::ESizeCutMode::Optimal} });
	std::cout << std::endl << s.totalCost << " in " << s.statistics.totalElapsedTime << "s" << std::endl;
	return 0;
}
