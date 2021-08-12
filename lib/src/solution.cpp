#include <index-selector/solution.hpp>

#include <stdexcept>

namespace IndexSelector
{

	void Solution::validate () const
	{
		//TODO
		statistics.validate ();
		problem.validate ();
		options.validate ();
	}

	void Solution::Statistics::validate () const
	{
		// TODO
	}

}