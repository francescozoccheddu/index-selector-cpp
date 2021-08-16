#include <index-selector/options.hpp>

#include <stdexcept>
#include <cmath>

namespace IndexSelector
{

	void Options::validate () const
	{
		if (heuristicSizeCutsMaxSize < 2)
		{
			throw std::out_of_range{ "heuristic size cuts max size must be greater than 1" };
		}
		if (timeLimit <= 0 or !std::isnormal (timeLimit))
		{
			throw std::out_of_range{ "Non-positive time limit" };
		}
		if (optimalSizeCutTimeLimit <= 0 or !std::isnormal (optimalSizeCutTimeLimit))
		{
			throw std::out_of_range{ "Non-positive optimal size cut time limit" };
		}
	}

}