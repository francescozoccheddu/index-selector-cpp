#include <index-selector/options.hpp>

#include <stdexcept>
#include <cmath>

namespace IndexSelector
{

	void Options::validate () const
	{
		if (heuristicSizeCutsMaxSize < 0)
		{
			throw std::out_of_range{ "Negative heuristic size cuts max size" };
		}
		if (heuristicSizeCutsMaxSize == 0 and sizeCutMode == ESizeCutMode::Heuristic)
		{
			throw std::out_of_range{ "Zero heuristic size cuts max size in heuristic mode" };
		}
		if (timeLimit <= 0 or std::isnan (timeLimit) or std::isinf(timeLimit))
		{
			throw std::out_of_range{ "Non-positive time limit" };
		}
	}

}