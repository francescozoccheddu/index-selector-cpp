#include <index-selector/options.hpp>

#include <stdexcept>

namespace IndexSelector
{

	void Options::validate() const
	{
		if (heuristicSizeCutsMaxSize < 0)
		{
			throw std::out_of_range{ "Negative heuristic size cuts max size" };
		}
		if (heuristicSizeCutsMaxSize == 0 && !enableSizeOptimalCuts)
		{
			throw std::out_of_range{ "Zero heuristic size cuts max size in heuristic mode" };
		}
	}

}