
#include <index-selector-app/test.hpp>
#include <index-selector-app/test_utils.hpp>
#include <fstream>

using namespace IndexSelector;
using namespace IndexSelector::App;

int main ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 60 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{80},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}},
		Options{
				.timeLimit{80},
				.shareSelectionCutter{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{7},
				.nMaxSizeCuts{50},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	ImmutableArray<ProblemResult> results{ test (problems, configs, 1) };
	std::ofstream csv;
	csv.open ("C:/Users/franc/desktop/results.csv");
	toCSV (results, csv, problemNoFields, solutionTimeFields | solutionCutFields);
	csv.close ();
	return 0;
}