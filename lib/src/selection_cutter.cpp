#include <index-selector-lib/selection_cutter.hpp>

namespace IndexSelector
{

	void SelectionCutter::cut (Callback& _callback)
	{
		std::forward_list<SelectionIndex>::iterator yoit = m_selectionIndices.before_begin (), yit = std::next (yoit);
		while (yit != m_selectionIndices.end ())
		{
			SelectionIndex& si{ *yit };
			IloNum vy = _callback.getValue (si.y);
			bool useful{ false };
			{
				std::forward_list<IloBoolVar>::iterator xoit = si.xs.before_begin (), xit = std::next (xoit);
				while (xit != si.xs.end ())
				{
					if (_callback.getValue (*xit) > vy)
					{
						_callback.add (*xit - si.y <= 0);
						statistics.nSelectionCuts++;
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
	}

	void SelectionCutter::reportElapsedTime (double _time) const
	{
		statistics.selectionCutElapsedTime += _time;
	}

	SelectionCutter* SelectionCutter::clone () const
	{
		return new SelectionCutter{ *this };
	}

}