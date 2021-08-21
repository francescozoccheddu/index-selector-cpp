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
	problems = explodeIndicesAndQueryCount (problems, { 10, 20, 50, 100, 150 }, { 1 });
	problems = explodeMaxSizeRatio (problems, { 0.05, 0.1, 0.25, 0.5 });
	ImmutableArray<Options> configs{
		Options{
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::None},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}},
		Options{
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Optimal},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}},
		Options{
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{6},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}},
		Options{
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{10},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	EStatistics statistics{ cutsAndModelStatistics | problemSize | EStatistics::MaxSizeRatio };
	test (problems, configs, csv, statistics, std::cout, 1, 1);
	csv.close ();
	return 0;
}
