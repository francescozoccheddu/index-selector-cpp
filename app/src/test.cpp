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

	const TestOptions::EStatistics TestOptions::defaultStatistics{ EStatistics::TotalTime | EStatistics::SelectionCutTime | EStatistics::SizeCutTime | EStatistics::SelectionCutCount | EStatistics::SizeCutCount | EStatistics::Nodes | EStatistics::Variables | EStatistics::Success };

	void TestOptions::validate () const
	{
		problem.validate ();
		for (const Options& o : configs)
		{
			o.validate ();
		}
		if (nMinIndices < 0 or nMaxIndices < 0)
		{
			throw std::out_of_range ("Negative number of indices");
		}
		if (nMaxIndices < nMinIndices)
		{
			throw std::out_of_range ("Max number of indices must be greater or equal to the min number of indices");
		}
		if (nTests < 0)
		{
			throw std::out_of_range ("Negative number of tests");
		}
		if (queriesRatio < 0)
		{
			throw std::out_of_range ("Queries ratio cannot be negative");
		}
	}

	void test (const TestOptions& _options)
	{
		_options.validate ();
		NullBuffer nullBuffer{};
		std::ostream nullStream{ &nullBuffer };
		std::ostream& csv{ _options.csvOut ? *_options.csvOut : nullStream };
		std::ostream& prg{ _options.progressOut ? *_options.progressOut : nullStream };
		csv << "nIndices,nQueries";
		for (int c{ 1 }; c <= _options.configs.size (); c++)
		{
			if (has (_options.statistics, TestOptions::EStatistics::Variables))
				csv << ",nVars" << c;
			if (has (_options.statistics, TestOptions::EStatistics::Nodes))
				csv << ",nNodes_" << c;
			if (has (_options.statistics, TestOptions::EStatistics::TotalTime))
				csv << ",TotalTime_" << c;
			if (has (_options.statistics, TestOptions::EStatistics::SelectionCutTime))
				csv << ",C1Time_" << c;
			if (has (_options.statistics, TestOptions::EStatistics::SizeCutTime))
				csv << ",C2Time_" << c;
			if (has (_options.statistics, TestOptions::EStatistics::SelectionCutCount))
				csv << ",nC1Cuts_" << c;
			if (has (_options.statistics, TestOptions::EStatistics::SizeCutCount))
				csv << ",nC2Cuts_" << c;
			if (has (_options.statistics, TestOptions::EStatistics::Success))
				csv << ",success_" << c;
		}
		csv << std::endl;
		for (int t{}; t < _options.nTests; t++)
		{
			RandomProblemOptions problemOptions{ _options.problem };
			problemOptions.nIndices = static_cast<int>(std::round (static_cast<double>(_options.nMaxIndices - _options.nMinIndices) / std::max<int> (_options.nTests - 1, 0) * (_options.nTests - t - 1) + _options.nMinIndices));
			problemOptions.nQueries = static_cast<int>(std::round (problemOptions.nIndices * _options.queriesRatio));
			const Problem problem{ randomProblem (problemOptions) };
			csv << problem.nIndices () << "," << problem.nQueries ();
			int ic{ 1 };
			for (const Options& o : _options.configs)
			{
				prg << "Test " << t + 1 << "/" << _options.nTests << ", Config " << ic++ << "/" << _options.configs.size () << "...";
				const Solution s{ solve (problem, o) };
				if (has (_options.statistics, TestOptions::EStatistics::Variables))
					csv << "," << s.statistics.nVariables;
				if (has (_options.statistics, TestOptions::EStatistics::Nodes))
					csv << "," << s.statistics.nNodes;
				if (has (_options.statistics, TestOptions::EStatistics::TotalTime))
					csv << "," << s.statistics.totalElapsedTime;
				if (has (_options.statistics, TestOptions::EStatistics::SelectionCutTime))
					csv << "," << s.statistics.selectionCutElapsedTime;
				if (has (_options.statistics, TestOptions::EStatistics::SizeCutTime))
					csv << "," << s.statistics.sizeCutElapsedTime;
				if (has (_options.statistics, TestOptions::EStatistics::SelectionCutCount))
					csv << "," << s.statistics.nSelectionCuts;
				if (has (_options.statistics, TestOptions::EStatistics::SizeCutCount))
					csv << "," << s.statistics.nSizeCuts;
				if (has (_options.statistics, TestOptions::EStatistics::Success))
					csv << "," << s.succeeded ? 1 : 0;
				prg << " " << s.statistics.totalElapsedTime << "s" << std::endl;
			}
			csv << std::endl;
		}
	}

}