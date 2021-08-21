#include <iostream>
#include <index-selector-app/test.hpp>
#include <fstream>

using namespace IndexSelector;
using namespace IndexSelector::App;

int main ()
{
	std::ofstream csv;
	csv.open ("C:/Users/franc/desktop/results.csv");
	test (
		explodeIndicesAndQueryCount ({ RandomProblemOptions{} }, range (30, 300, 10), { 1 }),
		ImmutableArray<Options>{
			Options{
				 .enableSelectionCuts{false},
				 .sizeCutMode{Options::ESizeCutMode::None},
				 .additionalCuts{Options::EAdditionalCuts::All}
			},
			Options{
				 .enableSelectionCuts{true},
				 .sizeCutMode{Options::ESizeCutMode::Heuristic},
				 .additionalCuts{Options::EAdditionalCuts::None}
			}
		},
		csv, 
			EStatistics::IndicesCount | EStatistics::QueriesCount | EStatistics::TotalTime
			);
	csv.close ();
	return 0;
}
