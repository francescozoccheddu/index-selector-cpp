
#include <index-selector-app/test.hpp>
#include <index-selector-app/test_utils.hpp>
#include <fstream>
#include <iostream>
#include <format>

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
	{
		std::ofstream csv;
		csv.open (dir + _name + "_configs.csv");
		toCSV (_configs, csv, configAllFields);
		csv.close ();
	}
	const std::string format{ _nTests > 1 ? "_{}" : "" };
	for (size_t t{ 1 }; t <= _nTests; t++)
	{
		if (_nTests > 1)
		{
			std::cout << "Pass " << t << std::endl;
		}
		ImmutableArray<ProblemResult> results{ test (_problems, _configs, 1, t + 120, std::cout, 1) };
		{
			std::ofstream csv;
			csv.open (dir + _name + "_results" + std::format (format, t) + ".csv");
			toCSV (results, csv, problemAllFields, solutionAllFields);
			csv.close ();
		}
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
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Optimal},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{6},
				.nMaxSizeCuts{1000},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{9},
				.nMaxSizeCuts{1000},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{11},
				.nMaxSizeCuts{1000},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 10, "c2_opt_vs_heu");
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
				.timeLimit{60 * 5},
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
				.timeLimit{60 * 5},
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
				.timeLimit{60 * 5},
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
				.timeLimit{60 * 5},
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
	test (problems, configs, 10, "c2_heu_vs_noheu");
}

void c1_vs_c2 ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{
		.nIndicesMaxSize{3},
		.nFasterQueriesPerIndex{10},
	} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 50, 75, 100 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 3, 4 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndicesMaxSize = _v;
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::None},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{5},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{true},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 5},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{5},
				.nMaxSizeCuts{1000},
				.enableHeuristicSizeCutHeuristics{false},
				.sizeCutManagement{Options::ECutManagement::CanFilter},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 10, "c1_vs_c2");
}

void global_vs_local ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{
		.nIndicesMaxSize{5},
		.nFasterQueriesPerIndex{20}
	} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 10, 25, 50, 75, 100 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60 * 15},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::None},
				.nMaxHeuristicSizeCutVars{4},
				.nMaxSizeCuts{20},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 15},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::None},
				.nMaxHeuristicSizeCutVars{4},
				.nMaxSizeCuts{20},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CanFilter},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
	};
	test (problems, configs, 5, "global_vs_local");
}

void bc_vs_bb_all ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{
		.nIndicesMaxSize{5},
		.nFasterQueriesPerIndex{20}
	} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 10, 25, 50, 75, 100 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60 * 15},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::None}
		},
		Options{
				.timeLimit{60 * 15},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}
		},
		Options{
				.timeLimit{60 * 15},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{4},
				.nMaxSizeCuts{20},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 5, "bc_vs_bb_all");
}

void bc_vs_all_hi ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{
		.nIndicesMaxSize{3},
		.nFasterQueriesPerIndex{30}
	} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 150, 200, 250, 300 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndices = _v;
		_o.nQueries = static_cast<int>(std::round (_v * 5.0 / 3.0));
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60 * 10},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}
		},
		Options{
				.timeLimit{60 * 10},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{5},
				.sizeCutTimeLimit{2},
				.nMaxSizeCuts{20},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 1, "bc_vs_all_hi_dense");
}

void range_maxSize ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{
		.nQueries{100},
		.nIndices {60},
		.nFasterQueriesPerIndex{30}
	} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 3, 5, 10, 20 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nIndicesMaxSize = _v;
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{60 * 10},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}
		},
		Options{
				.timeLimit{60 * 10},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{5},
				.nMaxSizeCuts{20},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 5, "range_maxSize");
}

void range_density ()
{
	ImmutableArray<RandomProblemOptions> problems{ RandomProblemOptions{
		.nQueries{120},
		.nIndices {72},
		.nIndicesMaxSize{5}
	} };
	problems = explode<RandomProblemOptions, int> (problems, ImmutableArray<int>{ 10, 20, 30, 40, 50 }, [] (RandomProblemOptions& _o, int _v)
	{
		_o.nFasterQueriesPerIndex = _v;
	});
	ImmutableArray<Options> configs{
		Options{
				.timeLimit{5 * 60},
				.enableSelectionCuts{false},
				.sizeCutMode{Options::ESizeCutMode::None},
				.additionalCuts{Options::EAdditionalCuts::All}
		},
		Options{
				.timeLimit{5 * 60},
				.enableSelectionCuts{true},
				.sizeCutMode{Options::ESizeCutMode::Heuristic},
				.nMaxHeuristicSizeCutVars{5},
				.nMaxSizeCuts{20},
				.sizeCutManagement{Options::ECutManagement::CannotPurge},
				.selectionCutManagement{Options::ECutManagement::CannotPurge},
				.additionalCuts{Options::EAdditionalCuts::None}
		}
	};
	test (problems, configs, 5, "range_density");
}


int main ()
{
	//c2_heu_vs_noheu ();
	//c2_opt_vs_heu ();
	//c1_vs_c2 ();
	//bc_vs_bb_all ();
	//range_maxSize ();
	//range_density ();
	//bc_vs_all_hi ();
	global_vs_local ();
	return 0;
}