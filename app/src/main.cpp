#include <iostream>
#include <index-selector-app/test.hpp>
#include <fstream>
#include <iostream>


using namespace IndexSelector;
using namespace IndexSelector::App;

int main ()
{
	std::ofstream csv;
	csv.open ("C:/Users/franc/desktop/results.csv");
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explodeIndicesAndQueryCount (problems, { 150, 225, 300 }, { 5.0 / 3.0 });
	problems = explodeMaxSizeRatio (problems, { 0.2 });
	problems = explodeIndexQueryCostRatio (problems, { 0.5 });
	problems = explodeIndexFixedCostRatio (problems, { 0.2 });
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}},
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Optimal},
				.nMaxHeuristicSizeCutVars{7},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	EStatistics statistics{ cutsAndModelStatistics | problemSize | EStatistics::MaxSizeRatio | EStatistics::IndexQueryCostRatio | EStatistics::IndexFixedCostRatio };
	test (problems, configs, csv, statistics, std::cout, 1, 3);
	csv.close ();
	return 0;
}
