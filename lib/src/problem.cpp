#include <index-selector/problem.hpp>

#include <stdexcept>

namespace IndexSelector
{

	void Problem::validate() const
	{
		if (maxSize < 0)
		{
			throw std::out_of_range{ "Negative max size" };
		}
		if (unindexedQueryCosts.size () == 0)
		{
			throw std::out_of_range{ "Empty unindexed query costs array" };
		}
		for (Real qc : unindexedQueryCosts)
		{
			if (qc < 0)
			{
				throw std::invalid_argument{ "Negative query cost" };
			}
		}
		for (const Index& i : indices)
		{
			i.validate ();
			if (i.size != unindexedQueryCosts.size())
			{
				throw std::invalid_argument{ "Mismatched number of queries between indices" };
			}
		}
	}

	int Problem::nQueries () const
	{
		return unindexedQueryCosts.size();
	}

}