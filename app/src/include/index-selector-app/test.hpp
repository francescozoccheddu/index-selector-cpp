#ifndef INCLUDED_INDEX_SELECTOR_APP_TEST
#define INCLUDED_INDEX_SELECTOR_APP_TEST

#include <index-selector-app/random_problem.hpp>
#include <index-selector/immutable_array.hpp>
#include <index-selector/options.hpp>
#include <index-selector/solution.hpp>
#include <ostream>
#include <iostream>

namespace IndexSelector::App
{

	enum class EProblemFields
	{
		nQueries = 1 << 0,
		nIndices = 1 << 1,
		nIndicesMaxSize = 1 << 2,
		indexFixedCostRatio = 1 << 3,
		indexQueryCostRatio = 1 << 4,
		indexFixedCostDev = 1 << 5,
		indexSizeDev = 1 << 6,
		queryCostDev = 1 << 7,
	};

	inline EProblemFields operator | (EProblemFields _lhs, EProblemFields _rhs)
	{
		using T = std::underlying_type_t <EProblemFields>;
		return static_cast<EProblemFields>(static_cast<T>(_lhs) | static_cast<T>(_rhs));
	}

	inline EProblemFields& operator |= (EProblemFields& _lhs, EProblemFields _rhs)
	{
		_lhs = _lhs | _rhs;
		return _lhs;
	}

	inline EProblemFields operator & (EProblemFields _lhs, EProblemFields _rhs)
	{
		using T = std::underlying_type_t <EProblemFields>;
		return static_cast<EProblemFields>(static_cast<T>(_lhs) & static_cast<T>(_rhs));
	}

	inline EProblemFields& operator &= (EProblemFields& _lhs, EProblemFields _rhs)
	{
		_lhs = _lhs & _rhs;
		return _lhs;
	}

	inline EProblemFields operator ~ (EProblemFields _x)
	{
		using T = std::underlying_type_t <EProblemFields>;
		return static_cast<EProblemFields>(~static_cast<T>(_x));
	}

	inline bool has (EProblemFields _src, EProblemFields _what)
	{
		return (_src & _what) == _what;
	}

	const EProblemFields problemNoFields{ 0 };
	const EProblemFields problemSizeFields{ EProblemFields::nQueries | EProblemFields::nIndices };
	const EProblemFields problemRatioFields{ EProblemFields::nIndicesMaxSize | EProblemFields::indexFixedCostRatio | EProblemFields::indexQueryCostRatio };
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

	inline ESolutionFields operator | (ESolutionFields _lhs, ESolutionFields _rhs)
	{
		using T = std::underlying_type_t <ESolutionFields>;
		return static_cast<ESolutionFields>(static_cast<T>(_lhs) | static_cast<T>(_rhs));
	}

	inline ESolutionFields& operator |= (ESolutionFields& _lhs, ESolutionFields _rhs)
	{
		_lhs = _lhs | _rhs;
		return _lhs;
	}

	inline ESolutionFields operator & (ESolutionFields _lhs, ESolutionFields _rhs)
	{
		using T = std::underlying_type_t <ESolutionFields>;
		return static_cast<ESolutionFields>(static_cast<T>(_lhs) & static_cast<T>(_rhs));
	}

	inline ESolutionFields& operator &= (ESolutionFields& _lhs, ESolutionFields _rhs)
	{
		_lhs = _lhs & _rhs;
		return _lhs;
	}

	inline ESolutionFields operator ~ (ESolutionFields _x)
	{
		using T = std::underlying_type_t <ESolutionFields>;
		return static_cast<ESolutionFields>(~static_cast<T>(_x));
	}

	inline bool has (ESolutionFields _src, ESolutionFields _what)
	{
		return (_src & _what) == _what;
	}

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

	void toCSV (ImmutableArray<ProblemResult> _results, EProblemFields _problemFields, ESolutionFields _solutionFields, std::ostream& _out);

	void toCSV (ImmutableArray<ProblemResult> _results, std::ostream& _out);

}

#endif