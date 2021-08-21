#include <index-selector-lib/selection_cutter.hpp>

namespace IndexSelector
{

	SelectionCutter::SelectionCutter (Cutter::Manager& _manager) : SimpleCutter{ _manager }
	{
		if (!manager.options.enableSelectionCuts)
		{
			throw std::logic_error{ "selection cuts are disabled in options" };
		}
		const size_t ni{ _manager.variables.problem ().nIndices () }, nq{ _manager.variables.problem ().nQueries () };
		for (size_t i{ 0 }; i < ni; i++)
		{
			const std::optional<IloBoolVar>& y{ _manager.variables.y (i) };
			if (y)
			{
				SelectionIndex si{ .y{*y}, .xs{} };
				for (size_t q{ 0 }; q < nq; q++)
				{
					const std::optional<IloBoolVar>& x{ _manager.variables.x (i, q) };
					if (x)
					{
						si.xs.push_front (*x);
					}
				}
				m_selectionIndices.emplace_front (std::move (si));
			}
		}
	}

	void SelectionCutter::cut (Callback& _callback)
	{
		_callback.lockIfShared ();
		std::forward_list<SelectionIndex>::iterator yoit = m_selectionIndices.before_begin (), yit = std::next (yoit);
		while (yit != m_selectionIndices.end ())
		{
			SelectionIndex& si{ *yit };
			Real vy = _callback.getValue (si.y);
			bool useful{ false };
			{
				std::forward_list<IloBoolVar>::iterator xoit = si.xs.before_begin (), xit = std::next (xoit);
				while (xit != si.xs.end ())
				{
					bool gone{ false };
					if (_callback.getValue (*xit) > vy)
					{
						_callback.add (*xit - si.y <= 0, IloCplex::CutManagement::UseCutForce);
						gone = manager.options.selectionCutManagement == Options::ECutManagement::CannotPurge;
					}
					if (gone)
					{
						xit = si.xs.erase_after (xoit);
					}
					else
					{
						useful = true;
						xoit = xit++;
					}
				}
			}
			if (!useful)
			{
				yit = m_selectionIndices.erase_after (yoit);
			}
			else
			{
				yoit = yit++;
			}
		}
		_callback.unlockIfShared ();
	}

	bool SelectionCutter::shouldShare () const
	{
		return manager.options.shareSelectionCutter or manager.options.selectionCutManagement != Options::ECutManagement::CannotPurge;
	}

}