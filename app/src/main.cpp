
#include <index-selector-app/test.hpp>
#include <index-selector-app/test_utils.hpp>
#include <fstream>
#include <iostream>

using namespace IndexSelector;
using namespace IndexSelector::App;

#define INDEX_SELECTOR_APP_XSTR(x) INDEX_SELECTOR_APP_STR(x)
#define INDEX_SELECTOR_APP_STR(x) #x
#define INDEX_SELECTOR_APP_OUTPUT_DIR "C:/Users/franc/Desktop/results/"
#define INDEX_SELECTOR_APP_RESULTS_FILE(test) INDEX_SELECTOR_APP_OUTPUT_DIR INDEX_SELECTOR_APP_XSTR(test) "_results.csv"
#define INDEX_SELECTOR_APP_CONFIGS_FILE(test) INDEX_SELECTOR_APP_OUTPUT_DIR INDEX_SELECTOR_APP_XSTR(test) "_config.csv"

void test (const ImmutableArray<RandomProblemOptions>& _problems, const ImmutableArray<Options>& _configs, size_t _nTests, const std::string& _name)
{
	std::cout << _name << std::endl;
	const std::string dir{ "C:/Users/franc/Desktop/results/" };
	ImmutableArray<ProblemResult> results{ test (_problems, _configs, _nTests) };
	{
		std::ofstream csv;
		csv.open (dir + _name + "_results.csv");
		toCSV (results, csv, problemAllFields, solutionAllFields);
		csv.close ();
	}
	{
		std::ofstream csv;
		csv.open (dir + _name + "_configs.csv");
		toCSV (_configs, csv, configAllFields);
		csv.close ();
	}
	std::cout << std::endl;
}

void c2_opt_vs_heu ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 50, 75, 100 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Optimal},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{6},
				.nMaxSizeCuts{1000},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{11},
				.nMaxSizeCuts{1000},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 5, "c2_opt_vs_heu");
}


void c2_heu_vs_noheu ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 50, 75, 100 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{6},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{true},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{6},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{false},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{true},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{false},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 1, "c2_heu_vs_noheu");
}

void c1_vs_c2 ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 50, 75, 100 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::None},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{true},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{false},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 1, "c1_vs_c2");
}

void bc_vs_bb_all ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 50, 75, 100, 150, 200 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}
		},
		Options{
				.timeLimit{60*5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{false},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 1, "bc_vs_bb_all");
}


int main ()
{
	//c2_heu_vs_noheu ();
	c2_opt_vs_heu ();
	//c1_vs_c2 ();
	//bc_vs_bb_all ();
	return 0;
}