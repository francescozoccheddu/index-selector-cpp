#include <iostream>
#include <iomanip>
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
			Index{.size {6}, .fixedCost{250}, .queryCosts{6200, 2000, 800, 2700, 4200, 750}}
		}
	};
}

Problem m1_problem ()
{
	return Problem{
		.maxSize{19},
		.unindexedQueryCosts{ 6200, 2000, 800, 6700, 5000, 2000, 3000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 1000, 900, 7000, 3000 },
		.indices{
			Index{.size {10}, .fixedCost{100}, .queryCosts{4200, 2000, 1000, 5000, 5000, 1000, 3000, 5000, 600, 1000, 6000, 2000, 1500, 2000, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {8},  .fixedCost{300}, .queryCosts{6200, 2000, 800,  7000, 5000, 3000, 3000, 5000, 500, 1200, 6000, 1000, 1400, 2000, 3000, 5000, 890,  600, 5000, 3000}},
			Index{.size {20}, .fixedCost{400}, .queryCosts{6200, 2000, 800,  5000, 4000, 2000, 1000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 3000, 2400, 1000, 900, 7000, 2000}},
			Index{.size {5},  .fixedCost{200}, .queryCosts{6200, 3000, 2000, 6700, 5000, 2000, 3000, 1000, 600, 1200, 4000, 2000, 1200, 1800, 4000, 5400, 900,  900, 7000, 3000}},
			Index{.size {12}, .fixedCost{50},  .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 3000, 5000, 600, 1200, 6000, 2000, 1400, 1000, 4000, 5400, 1000, 900, 7000, 2000}},
			Index{.size {16}, .fixedCost{400}, .queryCosts{6200, 2000, 800,  3000, 5000, 2000, 3000, 5000, 400, 1200, 5000, 1000, 1400, 2000, 2500, 6000, 1000, 900, 7000, 3000}},
			Index{.size {9},  .fixedCost{500}, .queryCosts{6200, 500,  800,  6700, 5000, 2000, 2000, 5000, 600, 1200, 6000, 2000, 1100, 2000, 4000, 5400, 1000, 400, 7000, 1000}},
			Index{.size {13}, .fixedCost{200}, .queryCosts{6200, 2000, 800,  6700, 4000, 2000, 3000, 3000, 300, 500,  6000, 2000, 1400, 2000, 2000, 3000, 800,  900, 7000, 3000}},
			Index{.size {6},  .fixedCost{300}, .queryCosts{6200, 2000, 400,  6700, 5000, 2000, 3000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {17}, .fixedCost{100}, .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 3000, 5000, 200, 1200, 6000, 2000, 1000, 2000, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {22}, .fixedCost{50},  .queryCosts{3000, 2000, 800,  2000, 4000, 1000, 3000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 2000, 900, 7000, 2000}},
			Index{.size {15}, .fixedCost{200}, .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 3000, 5000, 600, 400,  3000, 2000, 400,  2000, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {13}, .fixedCost{300}, .queryCosts{6200, 2000, 100,  6700, 5000, 2000, 3000, 4000, 100, 1200, 6000, 2000, 1400, 2000, 2100, 5400, 700,  900, 2000, 3000}},
			Index{.size {8},  .fixedCost{100}, .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 3000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 1000, 500, 5000, 2000}},
			Index{.size {9},  .fixedCost{250}, .queryCosts{6200, 2000, 800,  6700, 3000, 2000, 1000, 5000, 600, 1200, 2000, 2000, 800,  1000, 3400, 5400, 1000, 400, 7000, 3000}},
			Index{.size {19}, .fixedCost{200}, .queryCosts{6200, 2000, 800,  6700, 7000, 3000, 3000, 5000, 400, 1200, 6000, 3000, 1400, 1600, 4000, 2000, 1000, 900, 7000, 3000}},
			Index{.size {11}, .fixedCost{400}, .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 2500, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {13}, .fixedCost{500}, .queryCosts{2000, 2000, 800,  6700, 5000, 2000, 3000, 3000, 600, 1200, 2000, 2000, 900,  2000, 4000, 5400, 1000, 500, 1000, 3000}},
			Index{.size {8},  .fixedCost{600}, .queryCosts{6200, 2000, 800,  6700, 4000, 2000, 2000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {7},  .fixedCost{100}, .queryCosts{6200, 2000, 800,  6700, 5000, 1000, 3000, 5000, 500, 1000, 6000, 2000, 1400, 2000, 4000, 3500, 600,  900, 7000, 3000}},
			Index{.size {15}, .fixedCost{200}, .queryCosts{4200, 2000, 2000, 6700, 2000, 2000, 3000, 5000, 600, 1200, 6000, 1000, 1400, 2000, 3500, 5400, 1000, 900, 7000, 3000}},
			Index{.size {14}, .fixedCost{300}, .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 3000, 2000, 600, 1200, 4000, 2000, 300,  1400, 4000, 5400, 1000, 900, 7000, 3000}},
			Index{.size {16}, .fixedCost{50},  .queryCosts{6200, 1000, 800,  6700, 5000, 2000, 2000, 5000, 300, 1200, 6000, 2000, 1400, 2000, 4000, 2400, 400,  900, 7000, 1000}},
			Index{.size {8},  .fixedCost{100}, .queryCosts{6200, 2000, 200,  6700, 5000, 3000, 3000, 5000, 600, 1200, 6000, 200,  1400, 2000, 4000, 5400, 1000, 700, 3000, 3000}},
			Index{.size {9},  .fixedCost{200}, .queryCosts{6200, 2000, 800,  6700, 1000, 2000, 3000, 5000, 600, 1200, 5000, 2000, 1400, 2000, 3000, 5400, 500,  900, 7000, 3000}},
			Index{.size {14}, .fixedCost{400}, .queryCosts{6000, 3000, 800,  6700, 5000, 2000, 2500, 1000, 100, 500,  6000, 2000, 1400, 1500, 4000, 2400, 1000, 900, 7000, 3000}},
			Index{.size {16}, .fixedCost{100}, .queryCosts{6200, 2000, 300,  6700, 5000, 1000, 3000, 5000, 600, 1200, 6000, 1200, 1400, 2000, 4000, 5400, 1000, 900, 4000, 1000}},
			Index{.size {12}, .fixedCost{200}, .queryCosts{6200, 2000, 800,  6000, 5000, 800,  3000, 5000, 600, 300,  6000, 1300, 2000, 2000, 2000, 5400, 500,  800, 7000, 3000}},
			Index{.size {16}, .fixedCost{50},  .queryCosts{6200, 2000, 800,  5000, 5000, 2000, 3000, 5000, 400, 1200, 6000, 2000, 1400, 1200, 4000, 5400, 1000, 700, 7000, 2000}},
			Index{.size {9},  .fixedCost{100}, .queryCosts{6200, 2000, 1000, 6700, 5000, 1000, 1000, 5000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 3000, 3000, 900, 7000, 3000}},
			Index{.size {10}, .fixedCost{200}, .queryCosts{6200, 2000, 800,  6700, 5000, 2000, 3000, 4000, 600, 1200, 6000, 2000, 1400, 2000, 4000, 5400, 2000, 900, 7000, 3000}}
		}
	};
}

Problem rand_problem (size_t _nIndices, size_t _nQueries, Real _sizeRatio = 4, Real _fixedCostRatio = 0.1, Real _maxMinRatio = 5, Real _unindexedIndexedRatio = 1.5, unsigned int _seed = 0)
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
		}
		problem.indices = ImmutableArray<Index>::take_ownership (idxs, _nIndices);
		problem.maxSize = _maxMinRatio * _sizeRatio / 2.0;
	}
	return problem;
}

template<typename TValue>
void print (const char* _pName, const TValue _value)
{
	std::cout << std::setw (10) << _pName << ": " << _value << std::endl;
}

int main ()
{
	const Solution s = solve (rand_problem (40, 20, 10), { .shareCutters{false}, .enableSelectionCuts{true}, .sizeCutMode{Options::ESizeCutMode::Optimal} });
	//const Solution s = solve (m1_problem (), { .shareCutters{false}, .enableSelectionCuts{false}, .sizeCutMode{Options::ESizeCutMode::Heuristic} });
	print ("Cost", s.cost);
	print ("Time", s.statistics.totalElapsedTime);
	print ("Nodes", s.statistics.nNodes);
	print ("Variables", s.statistics.nVariables);
	print ("C1 cuts", s.statistics.nSelectionCuts);
	print ("C2 cuts", s.statistics.nSizeCuts);
	print ("C1 time", s.statistics.selectionCutElapsedTime);
	print ("C2 time", s.statistics.sizeCutElapsedTime);
	return 0;
}
