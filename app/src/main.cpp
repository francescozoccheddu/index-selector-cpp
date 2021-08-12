#include <iostream>
#include <index-selector/immutable_array.hpp>
#include <index-selector/index.hpp>
#include <index-selector/solve.hpp>
#include <vector>

using namespace std;

int main ()
{
	IndexSelector::ImmutableArray<float> c{ 1,2,3,4,5 };
	for (int q : c)
	{
		cout << q;
	}
	IndexSelector::Index i{ .size = 78, .fixedCost = 6, .queryCosts = c };
	return 0;
}
