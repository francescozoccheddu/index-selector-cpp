#ifndef INCLUDED_INDEX_SELECTOR_LIB_SELECTION_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_SELECTION_CUTTER

#include <index-selector-lib/cutter.hpp>
#include <forward_list>

namespace IndexSelector
{

	class SelectionCutter final : public SimpleCutter<SelectionCutter>
	{

		struct SelectionIndex
		{
			IloBoolVar y;
			std::forward_list<IloBoolVar> xs;
		};

		std::forward_list<SelectionIndex> m_selectionIndices{};


	protected:

		void cut (Callback& _callback) override;
		bool shouldShare () const override;

	public:

		SelectionCutter (Cutter::Manager& _manager);

	};

}

#endif