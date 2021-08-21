#ifndef INCLUDED_INDEX_SELECTOR_APP_TEST
#define INCLUDED_INDEX_SELECTOR_APP_TEST

#include <index-selector-app/random_problem.hpp>
#include <index-selector/immutable_array.hpp>
#include <index-selector/options.hpp>
#include <ostream>
#include <iostream>
#include <type_traits>

namespace IndexSelector::App
{

	enum class EStatistics
	{
		TotalTime = 1 << 0,
		SelectionCutTime = 1 << 1,
		SizeCutTime = 1 << 2,
		Nodes = 1 << 3,
		Variables = 1 << 4,
		Constraints = 1 << 5,
		SelectionCutCount = 1 << 6,
		SizeCutCount = 1 << 7,
		Success = 1 << 8,
		QueriesCount = 1 << 9,
		IndicesCount = 1 << 10,
		MaxSizeRatio = 1 << 11,
		IndexFixedCostRatio = 1 << 12,
		IndexQueryCostRatio = 1 << 13,
		IndexSizeDev = 1 << 14,
		QueryCostDev = 1 << 15,
		IndexFixedCostDev = 1 << 16
	};

	inline EStatistics operator | (EStatistics _lhs, EStatistics _rhs)
	{
		using T = std::underlying_type_t <EStatistics>;
		return static_cast<EStatistics>(static_cast<T>(_lhs) | static_cast<T>(_rhs));
	}

	inline EStatistics& operator |= (EStatistics& _lhs, EStatistics _rhs)
	{
		_lhs = _lhs | _rhs;
		return _lhs;
	}

	inline EStatistics operator & (EStatistics _lhs, EStatistics _rhs)
	{
		using T = std::underlying_type_t <EStatistics>;
		return static_cast<EStatistics>(static_cast<T>(_lhs) & static_cast<T>(_rhs));
	}

	inline EStatistics& operator &= (EStatistics& _lhs, EStatistics _rhs)
	{
		_lhs = _lhs & _rhs;
		return _lhs;
	}

	inline EStatistics operator ~ (EStatistics _x)
	{
		using T = std::underlying_type_t <EStatistics>;
		return static_cast<EStatistics>(~static_cast<T>(_x));
	}

	inline bool has (EStatistics _src, EStatistics _what)
	{
		return (_src & _what) == _what;
	}

	const EStatistics problemSize{ EStatistics::IndicesCount | EStatistics::QueriesCount };
	const EStatistics successAndTime{ EStatistics::Success | EStatistics::TotalTime };
	const EStatistics selectionCutsCountAndTime{ EStatistics::SelectionCutCount | EStatistics::SelectionCutTime };
	const EStatistics sizeCutsCountAndTime{ EStatistics::SizeCutCount | EStatistics::SizeCutTime };
	const EStatistics cutsCountAndTime{ selectionCutsCountAndTime | sizeCutsCountAndTime };
	const EStatistics cutsCount{ EStatistics::SizeCutCount | EStatistics::SelectionCutCount };
	const EStatistics cutsTime{ EStatistics::SizeCutTime | EStatistics::SelectionCutTime };
	const EStatistics cutsStatistics{ successAndTime | cutsCountAndTime | EStatistics::Nodes };
	const EStatistics modelStatistics{ successAndTime | EStatistics::Variables | EStatistics::Constraints };
	const EStatistics cutsAndModelStatistics{ cutsStatistics | modelStatistics };
	const EStatistics allProblemStatistics{ problemSize | EStatistics::IndexFixedCostDev | EStatistics::IndexFixedCostRatio | EStatistics::IndexQueryCostRatio | EStatistics::IndexSizeDev | EStatistics::MaxSizeRatio | EStatistics::QueryCostDev };
	const EStatistics allStatistics{ allProblemStatistics | cutsAndModelStatistics };

	void test (const ImmutableArray<RandomProblemOptions>& _problems, const ImmutableArray<Options>& _configs, std::ostream& _csv, EStatistics _statistics, std::ostream& _progress = std::cout, int _nTests = 1, unsigned int _seed = 0);

	ImmutableArray<double> range (double _min, double _max, int _count);

	ImmutableArray<int> range (int _min, int _max, int _count);

	ImmutableArray<RandomProblemOptions> explodeMaxSizeRatio (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _maxSizeRatios);

	ImmutableArray<RandomProblemOptions> explodeIndicesCount (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<int> _indexCounts);

	ImmutableArray<RandomProblemOptions> explodeQueryCount (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<int> _queryCounts);

	ImmutableArray<RandomProblemOptions> explodeIndicesAndQueryCount (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<int> _indicesCounts, ImmutableArray<double> _queriesRatios);

	ImmutableArray<RandomProblemOptions> explodeIndexFixedCostRatio (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _fixedCostRatios);

	ImmutableArray<RandomProblemOptions> explodeIndexQueryCostRatio (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _queryCostRatios);

	ImmutableArray<RandomProblemOptions> explodeIndexFixedCostDev (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _indexFixedCostdevs);

	ImmutableArray<RandomProblemOptions> explodeQueryCostDev (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _queryCostDevs);

	ImmutableArray<RandomProblemOptions> explodeIndexSizeDev (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _indexSizeDevs);

}

#endif