#ifndef INCLUDED_INDEX_SELECTOR_SOLVE
#define INCLUDED_INDEX_SELECTOR_SOLVE

#include <index-selector/problem.hpp>
#include <index-selector/solution.hpp>
#include <index-selector/options.hpp>

namespace IndexSelector
{

	Solution solve (const Problem& _problem, const Options& _options);

}

#endif