#include <index-selector-app/test.hpp>

#include <index-selector/solve.hpp>
#include <stdexcept>
#include <random>
#include <cmath>

namespace IndexSelector::App
{

	class NullBuffer final : public std::streambuf
	{
	public:
		int overflow (int _c)
		{
			return _c;
		}
	};

	void test (const ImmutableArray<RandomProblemOptions>& _problems, const ImmutableArray<Options>& _configs, std::ostream& _csv, EStatistics _statistics, std::ostream& _progress, int _nTests, unsigned int _seed)
	{
		if (_nTests < 0)
		{
			throw std::invalid_argument ("Negative test count");
		}
		std::mt19937 re{ _seed };
		std::uniform_int_distribution<unsigned int>  rs (0, 100000);
		NullBuffer nullBuffer{};
		std::ostream nullStream{ &nullBuffer };
		std::ostream& csv{ _csv ? _csv : nullStream };
		std::ostream& prg{ _progress ? _progress : nullStream };
		csv << "i";
		if (has (_statistics, EStatistics::IndicesCount))
			csv << ",nIndices";
		if (has (_statistics, EStatistics::QueriesCount))
			csv << ",nQueries";
		if (has (_statistics, EStatistics::IndexFixedCostDev))
			csv << ",indexFixedCostDev";
		if (has (_statistics, EStatistics::IndexFixedCostRatio))
			csv << ",indexFixedCostRatio";
		if (has (_statistics, EStatistics::IndexQueryCostRatio))
			csv << ",indexQueryCostRatio";
		if (has (_statistics, EStatistics::IndexSizeDev))
			csv << ",indexSizeDev";
		if (has (_statistics, EStatistics::MaxSizeRatio))
			csv << ",maxSizeRatio";
		if (has (_statistics, EStatistics::QueryCostDev))
			csv << ",queryCostDev";
		for (int c{ 1 }; c <= _configs.size (); c++)
		{
			if (has (_statistics, EStatistics::Success))
				csv << ",succeeded_" << c;
			if (has (_statistics, EStatistics::Variables))
				csv << ",nVariables_" << c;
			if (has (_statistics, EStatistics::Constraints))
				csv << ",nConstraints_" << c;
			if (has (_statistics, EStatistics::Nodes))
				csv << ",nNodes_" << c;
			if (has (_statistics, EStatistics::TotalTime))
				csv << ",totalElapsedTime_" << c;
			if (has (_statistics, EStatistics::SelectionCutTime))
				csv << ",selectionCutElapsedTime_" << c;
			if (has (_statistics, EStatistics::SizeCutTime))
				csv << ",sizeCutElapsedTime_" << c;
			if (has (_statistics, EStatistics::SelectionCutCount))
				csv << ",nSelectionCuts_" << c;
			if (has (_statistics, EStatistics::SizeCutCount))
				csv << ",nSizeCuts_" << c;
		}
		csv << std::endl;
		std::vector<Problem> problems (_nTests);
		for (int ip{}; ip < _problems.size (); ip++)
		{
			for (int it{}; it < _nTests; it++)
			{
				problems[it] = randomProblem (_problems[ip], rs (re));
			}
			csv << ip;
			if (has (_statistics, EStatistics::IndicesCount))
				csv << "," << _problems[ip].nIndices;
			if (has (_statistics, EStatistics::QueriesCount))
				csv << "," << _problems[ip].nQueries;
			if (has (_statistics, EStatistics::IndexFixedCostDev))
				csv << "," << _problems[ip].indexFixedCostDev;
			if (has (_statistics, EStatistics::IndexFixedCostRatio))
				csv << "," << _problems[ip].indexFixedCostRatio;
			if (has (_statistics, EStatistics::IndexQueryCostRatio))
				csv << "," << _problems[ip].indexQueryCostRatio;
			if (has (_statistics, EStatistics::IndexSizeDev))
				csv << "," << _problems[ip].indexSizeDev;
			if (has (_statistics, EStatistics::MaxSizeRatio))
				csv << "," << _problems[ip].maxSizeRatio;
			if (has (_statistics, EStatistics::QueryCostDev))
				csv << "," << _problems[ip].queryCostDev;
			for (int ic{}; ic < _configs.size (); ic++)
			{
				size_t nNodes{}, nVariables{}, nConstraints{}, nSelectionCuts{}, nSizeCuts{};
				double totalElapsedTime{}, selectionCutElapsedTime{}, sizeCutElapsedTime{};
				int succeeded{};
				for (int it{}; it < _nTests; it++)
				{
					prg << "[" << ip * _nTests * _configs.size() + ic * _nTests + it << "/" << _nTests * _configs.size () * _problems.size () << "]";
					prg << " Problem " << ip + 1 << "/" << _problems.size ();
					prg << " Config " << ic + 1 << "/" << _configs.size ();
					prg << " Test " << it + 1 << "/" << _nTests;
					prg << " ... ";
					Solution s{ solve (problems[it],_configs[ic]) };
					nNodes += s.statistics.nNodes;
					nVariables += s.statistics.nVariables;
					nConstraints += s.statistics.nConstraints;
					nSelectionCuts += s.statistics.nSelectionCuts;
					nSizeCuts += s.statistics.nSizeCuts;
					totalElapsedTime += s.statistics.totalElapsedTime;
					selectionCutElapsedTime += s.statistics.selectionCutElapsedTime;
					sizeCutElapsedTime += s.statistics.sizeCutElapsedTime;
					succeeded += s.succeeded ? 1 : 0;
					prg << s.statistics.totalElapsedTime << "s" << std::endl;
				}
				if (has (_statistics, EStatistics::Success))
					csv << "," << succeeded;
				if (has (_statistics, EStatistics::Variables))
					csv << "," << nVariables;
				if (has (_statistics, EStatistics::Constraints))
					csv << "," << nConstraints;
				if (has (_statistics, EStatistics::Nodes))
					csv << "," << nNodes;
				if (has (_statistics, EStatistics::TotalTime))
					csv << "," << totalElapsedTime;
				if (has (_statistics, EStatistics::SelectionCutTime))
					csv << "," << selectionCutElapsedTime;
				if (has (_statistics, EStatistics::SizeCutTime))
					csv << "," << sizeCutElapsedTime;
				if (has (_statistics, EStatistics::SelectionCutCount))
					csv << "," << nSelectionCuts;
				if (has (_statistics, EStatistics::SizeCutCount))
					csv << "," << nSizeCuts;
			}
			csv << std::endl;
		}

	}

	ImmutableArray<double> range (double _min, double _max, int _count)
	{
		if (_count < 0)
		{
			throw new std::invalid_argument ("Negative count");
		}
		double* const r{ new double[_count] };
		for (int i{}; i < _count; i++)
		{
			r[i] = (_max - _min) * i / std::max<int> (_count - 1, 0) + _min;
		}
		return ImmutableArray<double>::takeOwnership (r, _count);
	}

	ImmutableArray<int> range (int _min, int _max, int _count)
	{
		if (_count < 0)
		{
			throw new std::invalid_argument ("Negative count");
		}
		int* const r{ new int[_count] };
		for (int i{}; i < _count; i++)
		{
			r[i] = static_cast<int>(std::round ((static_cast<double>(_max) - _min)) * i / std::max<int> (_count - 1, 0) + _min);
		}
		return ImmutableArray<int>::takeOwnership (r, _count);
	}

	template<typename TSetter>
	ImmutableArray<RandomProblemOptions> explode (ImmutableArray<RandomProblemOptions> _options, int _count, TSetter _setter)
	{
		if (_count < 0)
		{
			throw new std::invalid_argument ("Negative count");
		}
		RandomProblemOptions* const r{ new RandomProblemOptions[_count * _options.size ()] };
		int fi{ };
		for (RandomProblemOptions o : _options)
		{
			for (int c{}; c < _count; c++)
			{
				_setter (o, c);
				r[fi++] = o;
			}
		}
		return ImmutableArray<RandomProblemOptions>::takeOwnership (r, _count * _options.size ());
	}

	ImmutableArray<RandomProblemOptions> explodeMaxSizeRatio (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _maxSizeRatios)
	{
		return explode (_options, _maxSizeRatios.size (), [_maxSizeRatios] (RandomProblemOptions& _o, int _c)
		{
			_o.maxSizeRatio = _maxSizeRatios[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeIndicesCount (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<int> _indexCounts)
	{
		return explode (_options, _indexCounts.size (), [_indexCounts] (RandomProblemOptions& _o, int _c)
		{
			_o.nIndices = _indexCounts[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeQueryCount (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<int> _queryCounts)
	{
		return explode (_options, _queryCounts.size (), [_queryCounts] (RandomProblemOptions& _o, int _c)
		{
			_o.nQueries = _queryCounts[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeIndicesAndQueryCount (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<int> _indicesCounts, ImmutableArray<double> _queriesRatios)
	{
		_options = explodeIndicesCount (_options, _indicesCounts);
		return explode (_options, _queriesRatios.size (), [_queriesRatios] (RandomProblemOptions& _o, int _c)
		{
			_o.nQueries = static_cast<int>(std::round (_o.nIndices * _queriesRatios[_c]));
		});
	}

	ImmutableArray<RandomProblemOptions> explodeIndexFixedCostRatio (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _fixedCostRatios)
	{
		return explode (_options, _fixedCostRatios.size (), [_fixedCostRatios] (RandomProblemOptions& _o, int _c)
		{
			_o.indexFixedCostRatio = _fixedCostRatios[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeIndexQueryCostRatio (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _queryCostRatios)
	{
		return explode (_options, _queryCostRatios.size (), [_queryCostRatios] (RandomProblemOptions& _o, int _c)
		{
			_o.indexQueryCostRatio = _queryCostRatios[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeIndexFixedCostDev (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _indexFixedCostdevs)
	{
		return explode (_options, _indexFixedCostdevs.size (), [_indexFixedCostdevs] (RandomProblemOptions& _o, int _c)
		{
			_o.indexFixedCostDev = _indexFixedCostdevs[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeQueryCostDev (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _queryCostDevs)
	{
		return explode (_options, _queryCostDevs.size (), [_queryCostDevs] (RandomProblemOptions& _o, int _c)
		{
			_o.queryCostDev = _queryCostDevs[_c];
		});
	}

	ImmutableArray<RandomProblemOptions> explodeIndexSizeDev (ImmutableArray<RandomProblemOptions> _options, ImmutableArray<double> _indexSizeDevs)
	{
		return explode (_options, _indexSizeDevs.size (), [_indexSizeDevs] (RandomProblemOptions& _o, int _c)
		{
			_o.indexSizeDev = _indexSizeDevs[_c];
		});
	}

}