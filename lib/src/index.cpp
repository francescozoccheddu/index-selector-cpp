#include <index-selector/index.hpp>

#include <stdexcept>

namespace IndexSelector
{

	void Index::validate () const
	{
		if (size < 0)
		{
			throw std::out_of_range{ "Negative size" };
		}
		if (fixedCost < 0)
		{
			throw std::out_of_range{ "Negative fixed cost" };
		}
		for (Real qc : queryCosts)
		{
			if (qc < 0)
			{
				throw std::out_of_range{ "Negative query cost" };
			}
		}
	}

	size_t Index::nQueries () const
	{
		return queryCosts.size ();
	}

}