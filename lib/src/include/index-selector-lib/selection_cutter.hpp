#ifndef INCLUDED_INDEX_SELECTOR_LIB_SELECTION_CUTTER
#define INCLUDED_INDEX_SELECTOR_LIB_SELECTION_CUTTER

#include <index-selector-lib/cutter.hpp>
#include <forward_list>

namespace IndexSelector
{

	class SelectionCutter final : public Cutter
	{

		struct SelectionIndex
		{
			IloBoolVar y;
			std::forward_list<IloBoolVar> xs;
		};

		std::forward_list<SelectionIndex> m_selectionIndices{};


	protected:

		void cut (Callback& _callback) override;
		void reportElapsedTime (double _time) const override;
		SelectionCutter* clone () const override;

	public:

		using Cutter::Cutter;

	};

}

#endif