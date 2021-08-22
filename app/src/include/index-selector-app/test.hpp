#ifndef INCLUDED_INDEX_SELECTOR_APP_TEST
#define INCLUDED_INDEX_SELECTOR_APP_TEST

#include <index-selector-app/random_problem.hpp>
#include <index-selector/immutable_array.hpp>
#include <index-selector/options.hpp>
#include <index-selector/solution.hpp>
#include <ostream>
#include <iostream>
#include <type_traits>

namespace IndexSelector::App
{

	template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value>::type* = nullptr>
	inline TEnum operator | (TEnum _lhs, TEnum _rhs)
	{
		using T = std::underlying_type_t <TEnum>;
		return static_cast<TEnum>(static_cast<T>(_lhs) | static_cast<T>(_rhs));
	}

	template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value>::type* = nullptr>
	inline TEnum& operator |= (TEnum& _lhs, TEnum _rhs)
	{
		_lhs = _lhs | _rhs;
		return _lhs;
	}

	template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value>::type* = nullptr>
	inline TEnum operator & (TEnum _lhs, TEnum _rhs)
	{
		using T = std::underlying_type_t <TEnum>;
		return static_cast<TEnum>(static_cast<T>(_lhs) & static_cast<T>(_rhs));
	}

	template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value>::type* = nullptr>
	inline TEnum& operator &= (TEnum& _lhs, TEnum _rhs)
	{
		_lhs = _lhs & _rhs;
		return _lhs;
	}

	template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value>::type* = nullptr>
	inline TEnum operator ~ (TEnum _x)
	{
		using T = std::underlying_type_t <TEnum>;
		return static_cast<TEnum>(~static_cast<T>(_x));
	}

	template<typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value>::type* = nullptr>
	inline bool has (TEnum _src, TEnum _what)
	{
		return (_src & _what) == _what;
	}

	enum class EProblemFields
	{
		nQueries = 1 << 0,
		nIndices = 1 << 1,
		nIndicesMaxSize = 1 << 2,
		indexFixedCostRatio = 1 << 3,
		indexQueryMaxSpeedUp = 1 << 4,
		nFasterQueriesPerIndex = 1 << 5,
		indexFixedCostDev = 1 << 6,
		indexSizeDev = 1 << 7,
		queryCostDev = 1 << 8,
	};

	const EProblemFields problemNoFields{ 0 };
	const EProblemFields problemSizeFields{ EProblemFields::nQueries | EProblemFields::nIndices };
	const EProblemFields problemRatioFields{ EProblemFields::nIndicesMaxSize | EProblemFields::indexFixedCostRatio | EProblemFields::indexQueryMaxSpeedUp | EProblemFields::indexQueryMaxSpeedUp };
	const EProblemFields problemDevFields{ EProblemFields::indexFixedCostDev | EProblemFields::indexSizeDev | EProblemFields::queryCostDev };
	const EProblemFields problemAllFields{ problemSizeFields | problemRatioFields | problemDevFields };

	enum class ESolutionFields
	{
		nConstraints = 1 << 0,
		nNodes = 1 << 1,
		nSelectionCuts = 1 << 2,
		nSizeCuts = 1 << 3,
		nVariables = 1 << 4,
		selectionCutElapsedTime = 1 << 5,
		sizeCutElapsedTime = 1 << 6,
		totalElapsedTime = 1 << 7,
		succeeded = 1 << 8,
	};

	const ESolutionFields solutionNoFields{ 0 };
	const ESolutionFields solutionSizeFields{ ESolutionFields::nNodes | ESolutionFields::totalElapsedTime };
	const ESolutionFields solutionModelFields{ ESolutionFields::nConstraints | ESolutionFields::nVariables };
	const ESolutionFields solutionTimeFields{ ESolutionFields::totalElapsedTime | ESolutionFields::selectionCutElapsedTime | ESolutionFields::sizeCutElapsedTime };
	const ESolutionFields solutionSelectionCutFields{ ESolutionFields::selectionCutElapsedTime | ESolutionFields::nSelectionCuts };
	const ESolutionFields solutionSizeCutFields{ ESolutionFields::sizeCutElapsedTime | ESolutionFields::nSizeCuts };
	const ESolutionFields solutionCutTimeFields{ ESolutionFields::selectionCutElapsedTime | ESolutionFields::sizeCutElapsedTime };
	const ESolutionFields solutionCutCountFields{ ESolutionFields::nSelectionCuts | ESolutionFields::nSizeCuts };
	const ESolutionFields solutionCutFields{ solutionSelectionCutFields | solutionSizeCutFields };
	const ESolutionFields solutionAllFields{ solutionSizeFields | solutionModelFields | solutionCutFields | ESolutionFields::succeeded };

	enum class EConfigFields
	{
		timeLimit = 1 << 0,
		reduceVariables = 1 << 1,
		shareSelectionCutter = 1 << 2,
		shareSizeCutter = 1 << 3,
		enableSelectionCuts = 1 << 4,
		sizeCutMode = 1 << 5,
		nMaxHeuristicSizeCutVars = 1 << 6,
		sizeCutTimeLimit = 1 << 7,
		nMaxSizeCuts = 1 << 8,
		enableHeuristicSizeCutHeuristics = 1 << 9,
		sizeCutManagement = 1 << 10,
		selectionCutManagement = 1 << 11,
		additionalCuts = 1 << 12,
		presolve = 1 << 13,
		enableHeuristics = 1 << 14,
		disableNonPrimalLP = 1 << 15,
	};

	const EConfigFields configNoFields{ 0 };
	const EConfigFields configSelectionCutFields{ EConfigFields::selectionCutManagement | EConfigFields::shareSelectionCutter | EConfigFields::enableSelectionCuts };
	const EConfigFields configSizeCutFields{ EConfigFields::sizeCutManagement | EConfigFields::sizeCutTimeLimit | EConfigFields::sizeCutMode | EConfigFields::shareSizeCutter  };
	const EConfigFields configHeuristicSizeCutFields{ EConfigFields::enableHeuristicSizeCutHeuristics | EConfigFields::nMaxHeuristicSizeCutVars | EConfigFields::nMaxSizeCuts  };
	const EConfigFields configMIPFields{ EConfigFields::presolve | EConfigFields::enableHeuristics | EConfigFields::disableNonPrimalLP | EConfigFields::additionalCuts | EConfigFields::timeLimit | EConfigFields::reduceVariables };
	const EConfigFields configAllFields{ configMIPFields | configSelectionCutFields | configSizeCutFields | configHeuristicSizeCutFields };

	struct ConfigResult final
	{
		bool succeeded;
		Solution::Statistics statistics;
	};

	struct ProblemResult final
	{
		RandomProblemOptions problem;
		ImmutableArray<ConfigResult> configs;
	};

	extern std::ostream nullStream;

	ImmutableArray<ProblemResult> test (const ImmutableArray<RandomProblemOptions>& _problems, const ImmutableArray<Options>& _options, size_t _nTests = 1, unsigned int _seed = 0, std::ostream& _out = std::cout, Real _costTolerance = 0.0000001);

	EProblemFields getChangingProblemFields (ImmutableArray<ProblemResult> _results);
	ESolutionFields getChangingSolutionFields (ImmutableArray<ConfigResult> _results);
	ESolutionFields getChangingSolutionFields (ImmutableArray<ProblemResult> _results);
	EConfigFields getChangingConfigFields (ImmutableArray<Options> _configs);

	void toCSV (ImmutableArray<ProblemResult> _results, std::ostream& _out, EProblemFields _problemFields, ESolutionFields _solutionFields);

	void toCSV (ImmutableArray<ProblemResult> _results, std::ostream& _out);

	void toCSV (ImmutableArray<Options> _configs, std::ostream& _out, EConfigFields _configFields);

	void toCSV (ImmutableArray<Options> _configs, std::ostream& _out);

}

#endif