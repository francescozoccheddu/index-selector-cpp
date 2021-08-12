#include <iostream>
#include <index-selector/immutable_array.hpp>
#include <vector>

using namespace std;

int main ()
{
	IndexSelector::ImmutableArray<int> c{ 1,2,3,4,5 };
	for (int q : c)
	{
		cout << q;
	}
	return 0;
}
