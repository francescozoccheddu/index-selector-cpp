#include <iostream>
#include <iomanip>
#include <random>
#include <index-selector/solve.hpp>
#include <index-selector-app/random_problem.hpp>

using namespace IndexSelector;

template<typename TValue>
void print (const char* _pName, const TValue _value)
{
	std::cout << std::setw (10) << _pName << ": " << _value << std::endl;
}

int main ()
{
	const Solution s = solve (randomProblem (RandomProblemOptions{}), { .enableSelectionCuts{false}, .sizeCutMode{Options::ESizeCutMode::None} });
	print ("Cost", s.cost);
	print ("Time", s.statistics.totalElapsedTime);
	print ("Nodes", s.statistics.nNodes);
	print ("Variables", s.statistics.nVariables);
	print ("C1 cuts", s.statistics.nSelectionCuts);
	print ("C2 cuts", s.statistics.nSizeCuts);
	print ("C1 time", s.statistics.selectionCutElapsedTime);
	print ("C2 time", s.statistics.sizeCutElapsedTime);
	return 0;
}
