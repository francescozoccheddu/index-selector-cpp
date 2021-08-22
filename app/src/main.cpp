
#include <index-selector-app/test.hpp>
#include <index-selector-app/test_utils.hpp>
#include <fstream>

using namespace IndexSelector;
using namespace IndexSelector::App;

int main ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 50 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}},
		Options{
				.timeLimit{60},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{7},
				.nMaxSizeCuts{1},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	ImmutableArray<ProblemResult> results{ test (problems, configs) };
	std::ofstream csv;
	csv.open ("C:/Users/franc/desktop/results.csv");
	toCSV (results, csv);
	csv.close ();
	return 0;
}