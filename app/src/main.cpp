#include <iostream>
#include <index-selector-app/test.hpp>
#include <fstream>

using namespace IndexSelector;

int main ()
{
	std::ofstream csv;
	csv.open ("C:/Users/franc/desktop/results.csv");
	App::test ({
		.csvOut{&csv},
		.configs{
				   Options{
						.enableSelectionCuts{false},
						.sizeCutMode{Options::ESizeCutMode::None},
					},
				   Options{
						.enableSelectionCuts{true},
						.sizeCutMode{Options::ESizeCutMode::Optimal}
					}
			   },
			   });
	csv.close ();
	return 0;
}
