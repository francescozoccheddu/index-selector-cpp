#include <iostream>
#include <index-selector/immutable_array.hpp>
#include <index-selector/index.hpp>
#include <vector>

using namespace std;

int main ()
{
	IndexSelector::ImmutableArray<float> c{ 1,2,3,4,5 };
	for (int q : c)
	{
		cout << q;
	}
	IndexSelector::Index i{ 0,0,c };
	return 0;
}
