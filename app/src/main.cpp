#include <iostream>
#include <index-selector/solve.hpp>

using namespace IndexSelector;

Problem eb ()
{
	return Problem{
		.maxSize{19},
		.unindexedQueryCosts{ 6200,2000,800,6700,5000,2000 },
		.indices{
			Index{.size {10}, .fixedCost{200}, .queryCosts{1300, 900, 800, 6700, 5000, 2000}},
			Index{.size {5}, .fixedCost{1200}, .queryCosts{6200, 700, 800, 6700, 5000, 2000}},
			Index{.size {10}, .fixedCost{400}, .queryCosts{6200, 2000, 800, 1700, 2200, 2000}},
			Index{.size {8}, .fixedCost{2400}, .queryCosts{6200, 2000, 800, 6700, 1200, 2000}},
			Index{.size {8}, .fixedCost{250}, .queryCosts{6200, 2000, 800, 2700, 4200, 750}}
		}
	};
}

int main ()
{
	solve (eb (), {});
	return 0;
}
