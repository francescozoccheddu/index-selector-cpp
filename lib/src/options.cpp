#include <index-selector/options.hpp>

#include <stdexcept>
#include <cmath>

namespace IndexSelector
{

	void Options::validate () const
	{
		if (nMaxHeuristicSizeCutVars < 2)
		{
			throw std::out_of_range{ "Number of heuristic size cuts max vars must be greater than 1" };
		}
		if (nMaxSizeCuts < 1)
		{
			throw std::out_of_range{ "Non-positive number of heuristic size cuts" };
		}
		if (!std::isnormal (timeLimit))
		{
			throw std::out_of_range{ "Non-normal time limit" };
		}
		if (!std::isnormal (sizeCutTimeLimit))
		{
			throw std::out_of_range{ "Non-normal size cut time limit" };
		}
		if (timeLimit < 0.1 or timeLimit > 60.0 * 60.0 * 24.0 * 365.0)
		{
			throw std::out_of_range{ "Time limit out of range" };
		}
		if (sizeCutTimeLimit < 0.1 or sizeCutTimeLimit > 60.0 * 60.0 * 24.0 * 365.0)
		{
			throw std::out_of_range{ "Size cut time limit out of range" };
		}
	}

}