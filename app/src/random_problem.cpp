#include <index-selector-app/random_problem.hpp>

#include <stdexcept>
#include <random>

namespace IndexSelector::App
{

	void RandomProblemOptions::validate () const
	{
		if (nIndices < 0)
		{
			throw std::out_of_range ("Negative number of indices");
		}
		if (nQueries < 0)
		{
			throw std::out_of_range ("Negative number of queries");
		}
		if (indexSizeDev < 1)
		{
			throw std::out_of_range ("Index size deviance must be at least 1");
		}
		if (nIndicesMaxSize < 0)
		{
			throw std::out_of_range ("Max size indices count cannot be negative");
		}
		if (indexFixedCostDev < 1)
		{
			throw std::out_of_range ("Index fixed cost deviance must be at least 1");
		}
		if (indexFixedCostRatio < 0)
		{
			throw std::out_of_range ("Index fixed cost ratio cannot be negative");
		}
		if (queryCostDev < 1)
		{
			throw std::out_of_range ("Query cost deviance must be at least 1");
		}
		if (indexQueryCostRatio < 0)
		{
			throw std::out_of_range ("Indexed query cost ratio cannot be negative");
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
			const std::uniform_real_distribution qcr = std::uniform_real_distribution<double>{ _options.indexQueryCostRatio,  _options.queryCostDev * _options.indexQueryCostRatio };
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
					cs[q] = static_cast<Real>(qcr (re));
				}
				index.queryCosts = ImmutableArray<Real>::takeOwnership (cs, _options.nQueries);
				index.fixedCost = static_cast<Real>(fcr (re));
				index.size = static_cast<Real>(totalSize += sr (re));
			}
			problem.indices = ImmutableArray<Index>::takeOwnership (idxs, _options.nIndices);
			problem.maxSize = static_cast<Real>((sr.a () + sr.b ()) / 2.0 * _options.nIndicesMaxSize);
		}
		return problem;
	}

}