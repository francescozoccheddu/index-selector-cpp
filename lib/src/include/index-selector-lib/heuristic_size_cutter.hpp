#ifndef INCLUDED_INDEX_SELECTOR_LIB_HEURISTIC_SIZE_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_HEURISTIC_SIZE_CUTTER

#include <index-selector-lib/cutter.hpp>
#include <index-selector/immutable_array.hpp>

namespace IndexSelector
{

	class HeuristicSizeCutter final : public SimpleCutter<HeuristicSizeCutter>
	{

	protected:

		void cut (Callback& _callback) override;

	public:

		HeuristicSizeCutter (Cutter::Manager& _manager);

	};

}

#endif