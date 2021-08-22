#include <index-selector-app/test.hpp>

#include <index-selector/solve.hpp>
#include <stdexcept>
#include <random>
#include <cmath>
#include <string>
#include <vector>

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

	static NullBuffer nullBuffer{};
	std::ostream nullStream{ &nullBuffer };

	ImmutableArray<ProblemResult> test (const ImmutableArray<RandomProblemOptions>& _problems, const ImmutableArray<Options>& _configs, size_t _nTests, unsigned int _seed, std::ostream& _out, Real _costTolerance)
	{
		if (_nTests < 0)
		{
			throw std::invalid_argument ("Negative test count");
		}
		std::mt19937 re{ _seed };
		std::uniform_int_distribution<unsigned int>  rs (0, 100000);
		std::vector<Problem> problems (_nTests);
		std::vector<Real> costs (_nTests);
		std::vector<bool> costInitialized (_nTests);
		ProblemResult* const problemResults{ new ProblemResult[_problems.size ()] };
		for (size_t ip{}; ip < _problems.size (); ip++)
		{
			ConfigResult* const configResults{ new ConfigResult[_configs.size ()] };
			problemResults[ip] = { _problems[ip], ImmutableArray<ConfigResult>::takeOwnership (configResults, _configs.size ()) };
			for (size_t it{}; it < _nTests; it++)
			{
				problems[it] = randomProblem (_problems[ip], rs (re));
				costInitialized[it] = false;
			}
			for (size_t ic{}; ic < _configs.size (); ic++)
			{
				ConfigResult& configResult{ configResults[ic] };
				configResult.succeeded = true;
				for (size_t it{}; it < _nTests; it++)
				{
					_out << "[" << ip * _nTests * _configs.size () + ic * _nTests + it << "/" << _nTests * _configs.size () * _problems.size () << "]";
					if (_problems.size () > 1)
						_out << " Problem " << ip + 1 << "/" << _problems.size ();
					if (_configs.size () > 1)
						_out << " Config " << ic + 1 << "/" << _configs.size ();
					if (_nTests > 1)
						_out << " Test " << it + 1 << "/" << _nTests;
					_out << " ... ";
					Solution s{ solve (problems[it],_configs[ic]) };
					if (s.succeeded)
					{
						if (costInitialized[it])
						{
							if (std::abs (costs[it] - s.cost) > _costTolerance)
							{
								throw std::runtime_error ("Cost does not match");
							}
						}
						else
						{
							costInitialized[it] = true;
							costs[it] = s.cost;
						}
					}
					configResult.succeeded &= s.succeeded;
					configResult.statistics.nConstraints += s.statistics.nConstraints;
					configResult.statistics.nNodes += s.statistics.nNodes;
					configResult.statistics.nSelectionCuts += s.statistics.nSelectionCuts;
					configResult.statistics.nSizeCuts += s.statistics.nSizeCuts;
					configResult.statistics.nVariables += s.statistics.nVariables;
					configResult.statistics.selectionCutElapsedTime += s.statistics.selectionCutElapsedTime;
					configResult.statistics.sizeCutElapsedTime += s.statistics.sizeCutElapsedTime;
					configResult.statistics.totalElapsedTime += s.statistics.totalElapsedTime;
					_out << s.statistics.totalElapsedTime << "s" << std::endl;
				}
			}
		}
		return ImmutableArray<ProblemResult>::takeOwnership (problemResults, _problems.size ());
	}

	template<typename TObject, typename TGetter>
	bool isChanging (ImmutableArray <TObject> _objects, TGetter _getter)
	{
		if (_objects.size () > 0)
		{
			auto value{ _getter (_objects[0]) };
			for (size_t i{ 1 }; i < _objects.size (); i++)
			{
				if (value != _getter (_objects[i]))
				{
					return true;
				}
			}
		}
		return false;
	}

	EProblemFields getChangingProblemFields (ImmutableArray<ProblemResult> _results)
	{
		EProblemFields fields{ problemNoFields };
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.indexFixedCostDev;
		}) ? EProblemFields::indexFixedCostDev : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.indexFixedCostRatio;
		}) ? EProblemFields::indexFixedCostRatio : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.indexQueryCostRatio;
		}) ? EProblemFields::indexQueryCostRatio : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.indexSizeDev;
		}) ? EProblemFields::indexSizeDev : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.nIndices;
		}) ? EProblemFields::nIndices : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.nIndicesMaxSize;
		}) ? EProblemFields::nIndicesMaxSize : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.nQueries;
		}) ? EProblemFields::nQueries : problemNoFields;
		fields |= isChanging (_results, [] (const ProblemResult& _result)
		{
			return _result.problem.queryCostDev;
		}) ? EProblemFields::queryCostDev : problemNoFields;
		return fields;
	}

	ESolutionFields getChangingSolutionFields (ImmutableArray<ProblemResult> _results)
	{
		ESolutionFields fields{ solutionNoFields };
		for (const ProblemResult& result : _results)
		{
			fields |= getChangingSolutionFields (result.configs);
		}
		return fields;
	}

	ESolutionFields getChangingSolutionFields (ImmutableArray<ConfigResult> _results)
	{
		ESolutionFields fields{ solutionNoFields };
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.succeeded;
		}) ? ESolutionFields::succeeded : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.nConstraints;
		}) ? ESolutionFields::succeeded : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.nNodes;
		}) ? ESolutionFields::nNodes : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.nSelectionCuts;
		}) ? ESolutionFields::nSelectionCuts : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.nSizeCuts;
		}) ? ESolutionFields::nSizeCuts : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.nVariables;
		}) ? ESolutionFields::nVariables : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.selectionCutElapsedTime;
		}) ? ESolutionFields::selectionCutElapsedTime : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.sizeCutElapsedTime;
		}) ? ESolutionFields::sizeCutElapsedTime : solutionNoFields;
		fields |= isChanging (_results, [] (const ConfigResult& _result)
		{
			return _result.statistics.totalElapsedTime;
		}) ? ESolutionFields::totalElapsedTime : solutionNoFields;
		return fields;
	}

	void toCSV (ImmutableArray<ProblemResult> _results, EProblemFields _problemFields, ESolutionFields _solutionFields, std::ostream& _out)
	{
		const size_t nc{ _results.size () == 0 ? 0 : _results[0].configs.size () };
		_out << "i";
		if (has (_problemFields, EProblemFields::nQueries)) _out << ",nQueries";
		if (has (_problemFields, EProblemFields::nIndices)) _out << ",nIndices";
		if (has (_problemFields, EProblemFields::nIndicesMaxSize)) _out << ",nIndicesMaxSize";
		if (has (_problemFields, EProblemFields::indexFixedCostRatio)) _out << ",indexFixedCostRatio";
		if (has (_problemFields, EProblemFields::indexQueryCostRatio)) _out << ",indexQueryCostRatio";
		if (has (_problemFields, EProblemFields::indexFixedCostDev)) _out << ",indexFixedCostDev";
		if (has (_problemFields, EProblemFields::indexSizeDev)) _out << ",indexSizeDev";
		if (has (_problemFields, EProblemFields::queryCostDev)) _out << ",queryCostDev";
		for (size_t c{ 1 }; c <= nc; c++)
		{
			if (has (_solutionFields, ESolutionFields::nConstraints)) _out << ",nConstraints_" << c;
			if (has (_solutionFields, ESolutionFields::nNodes)) _out << ",nNodes_" << c;
			if (has (_solutionFields, ESolutionFields::nSelectionCuts)) _out << ",nSelectionCuts_" << c;
			if (has (_solutionFields, ESolutionFields::nSizeCuts)) _out << ",nSizeCuts_" << c;
			if (has (_solutionFields, ESolutionFields::nVariables)) _out << ",nVariables_" << c;
			if (has (_solutionFields, ESolutionFields::selectionCutElapsedTime)) _out << ",selectionCutElapsedTime_" << c;
			if (has (_solutionFields, ESolutionFields::sizeCutElapsedTime)) _out << ",sizeCutElapsedTime_" << c;
			if (has (_solutionFields, ESolutionFields::totalElapsedTime)) _out << ",totalElapsedTime_" << c;
			if (has (_solutionFields, ESolutionFields::succeeded)) _out << ",succeeded_" << c;
		}
		_out << std::endl;
		size_t i{};
		for (const ProblemResult& problem : _results)
		{
			if (problem.configs.size () != nc)
			{
				throw std::invalid_argument ("Unmatched number of configs");
			}
			_out << i++;
			if (has (_problemFields, EProblemFields::nQueries)) _out << "," << problem.problem.nQueries;
			if (has (_problemFields, EProblemFields::nIndices)) _out << "," << problem.problem.nIndices;
			if (has (_problemFields, EProblemFields::nIndicesMaxSize)) _out << "," << problem.problem.nIndicesMaxSize;
			if (has (_problemFields, EProblemFields::indexFixedCostRatio)) _out << "," << problem.problem.indexFixedCostRatio;
			if (has (_problemFields, EProblemFields::indexQueryCostRatio)) _out << "," << problem.problem.indexQueryCostRatio;
			if (has (_problemFields, EProblemFields::indexFixedCostDev)) _out << "," << problem.problem.indexFixedCostDev;
			if (has (_problemFields, EProblemFields::indexSizeDev)) _out << "," << problem.problem.indexSizeDev;
			if (has (_problemFields, EProblemFields::queryCostDev)) _out << "," << problem.problem.queryCostDev;
			for (const ConfigResult& config : problem.configs)
			{
				if (has (_solutionFields, ESolutionFields::nConstraints)) _out << "," << config.statistics.nConstraints;
				if (has (_solutionFields, ESolutionFields::nNodes)) _out << "," << config.statistics.nNodes;
				if (has (_solutionFields, ESolutionFields::nSelectionCuts)) _out << "," << config.statistics.nSelectionCuts;
				if (has (_solutionFields, ESolutionFields::nSizeCuts)) _out << "," << config.statistics.nSizeCuts;
				if (has (_solutionFields, ESolutionFields::nVariables)) _out << "," << config.statistics.nVariables;
				if (has (_solutionFields, ESolutionFields::selectionCutElapsedTime)) _out << "," << config.statistics.selectionCutElapsedTime;
				if (has (_solutionFields, ESolutionFields::sizeCutElapsedTime)) _out << "," << config.statistics.sizeCutElapsedTime;
				if (has (_solutionFields, ESolutionFields::totalElapsedTime)) _out << "," << config.statistics.totalElapsedTime;
				if (has (_solutionFields, ESolutionFields::succeeded)) _out << "," << config.succeeded;
			}
			_out << std::endl;
		}
	}

	void toCSV (ImmutableArray<ProblemResult> _results, std::ostream& _out)
	{
		toCSV (_results, getChangingProblemFields (_results), getChangingSolutionFields (_results), _out);
	}

}