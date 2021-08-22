#include <index-selector-app/random_problem.hpp>

#include <stdexcept>
#include <random>

namespace IndexSelector::App
{

	void RandomProblemOptions::validate () const
	{
		if (nIndices < 0)
		{
			throw std::out_of_range ("nIndices < 0");
		}
		if (nQueries < 0)
		{
			throw std::out_of_range ("nQueries < 0");
		}
		if (indexSizeDev < 1)
		{
			throw std::out_of_range ("indexSizeDev < 1");
		}
		if (nIndicesMaxSize < 0)
		{
			throw std::out_of_range ("nIndicesMaxSize < 0");
		}
		if (indexFixedCostDev < 1)
		{
			throw std::out_of_range ("indexFixedCostDev < 1");
		}
		if (indexFixedCostRatio < 0)
		{
			throw std::out_of_range ("indexFixedCostRatio < 0");
		}
		if (queryCostDev < 1)
		{
			throw std::out_of_range ("queryCostDev < 1");
		}
		if (indexQueryMaxSpeedUp < 1)
		{
			throw std::out_of_range ("indexQueryMaxSpeedUp < 1");
		}
		if (nFasterQueriesPerIndex < 0)
		{
			throw std::out_of_range ("nFasterQueriesPerIndex < 0");
		}
	}

	Problem randomProblem (const RandomProblemOptions& _options, unsigned int _seed)
	{
		_options.validate ();
		std::mt19937 re{ _seed };
		Problem problem{};
		{
			const std::uniform_real_distribution r = std::uniform_real_distribution<double>{ 1, _options.queryCostDev };
			Real* const ucs = new Real[_options.nQueries];
			for (int q{ 0 }; q < _options.nQueries; q++)
			{
				ucs[q] = static_cast<Real>(r (re));
			}
			problem.unindexedQueryCosts = ImmutableArray<Real>::takeOwnership (ucs, _options.nQueries);
		}
		{
			const std::uniform_real_distribution fqp = std::uniform_real_distribution<double>{ 0, 1 };
			const std::uniform_real_distribution qcsu = std::uniform_real_distribution<double>{ 1.0,  _options.indexQueryMaxSpeedUp };
			const std::uniform_real_distribution fcr = std::uniform_real_distribution<double>{ _options.indexFixedCostRatio,  _options.indexFixedCostDev * _options.indexFixedCostRatio };
			const std::uniform_real_distribution sr = std::uniform_real_distribution<double>{ 1,  _options.indexSizeDev };
			Index* const idxs = new Index[_options.nIndices];
			double totalSize{};
			for (int i{ 0 }; i < _options.nIndices; i++)
			{
				Index& index = idxs[i];
				Real* const cs = new Real[_options.nQueries];
				for (int q{ 0 }; q < _options.nQueries; q++)
				{
					if (fqp (re) <= _options.nFasterQueriesPerIndex / _options.nQueries)
					{
						cs[q] = static_cast<Real>(problem.unindexedQueryCosts[q] / qcsu (re));
					}
					else
					{
						cs[q] = problem.unindexedQueryCosts[q];
					}
				}
				index.queryCosts = ImmutableArray<Real>::takeOwnership (cs, _options.nQueries);
				index.fixedCost = static_cast<Real>(fcr (re));
				double size{ sr (re) };
				index.size = static_cast<Real>(size);
				totalSize += size;
			}
			problem.indices = ImmutableArray<Index>::takeOwnership (idxs, _options.nIndices);
			problem.maxSize = static_cast<Real>((totalSize / _options.nIndices + (sr.a () + sr.b ()) / 2.0) / 2.0 * _options.nIndicesMaxSize);
		}
		return problem;
	}

}