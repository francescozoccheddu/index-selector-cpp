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
	problems = explodeIndicesAndQueryCount (problems, { 10, 20, 50, 100, 200, 300 }, { 1 });
	problems = explodeMaxSizeRatio (problems, { 0.05, 0.1, 0.25, 0.5 });
	ImmutableArray<Options> configs{
		Options{
				 .enableSelectionCuts{false},
				 .sizeCutMode{Options::ESizeCutMode::None},
				 .additionalCuts{Options::EAdditionalCuts::All}
			},
		Options{
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.sizeCutManagement{Options::ESizeCutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	EStatistics statistics{ cutsAndModelStatistics | problemSize | EStatistics::MaxSizeRatio };
	test (problems, configs, csv, statistics, std::cout, 1, 1);
	csv.close ();
	return 0;
}
