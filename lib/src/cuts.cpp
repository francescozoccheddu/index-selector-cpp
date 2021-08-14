#include <index-selector-lib/cuts.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <forward_list>
#include <mutex>

namespace IndexSelector
{

	class CutCallbackI final : public IloCplex::UserCutCallbackI
	{

		const VariableMatrix* m_pVariables;
		const Options* m_pOptions;
		Solution::Statistics* m_pStatistics;

		struct SelectionIndex
		{
			IloBoolVar y;
			std::forward_list<IloBoolVar> xs;
		};

		std::forward_list<SelectionIndex> m_selectionIndices;
		std::mutex m_selectionMutex;

		void add_selection_cuts ();

		void add_size_cuts ();

		void add_optimal_size_cut ();

		void add_heuristic_size_cuts ();

		CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);
		CutCallbackI (const CutCallbackI&);

		friend IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);

	protected:

		void main () override;
		IloCplex::CallbackI* duplicateCallback () const override;

	};

	IloCplex::CallbackI* CutCallbackI::duplicateCallback () const
	{
		return new (getEnv ()) CutCallbackI{ *this };
	}

	CutCallbackI::CutCallbackI (const CutCallbackI& _copy) : IloCplex::UserCutCallbackI{ _copy.getEnv () }, m_pVariables{ _copy.m_pVariables }, m_pOptions{ _copy.m_pOptions }, m_pStatistics{ _copy.m_pStatistics }, m_selectionIndices{ _copy.m_selectionIndices } {}

	CutCallbackI::CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics) : IloCplex::UserCutCallbackI{ _env }, m_pVariables{ &_variables }, m_pOptions{ &_options }, m_pStatistics{ &_statistics }
	{
		m_pOptions->validate ();
		if (m_pOptions->enableSelectionCuts)
		{
			const size_t ni{ _variables.problem ().nIndices () }, nq{ _variables.problem ().nQueries () };
			for (size_t i{ 0 }; i < ni; i++)
			{
				const std::optional<IloBoolVar>& y{ _variables.y (i) };
				if (y)
				{
					SelectionIndex si{ .y{*y}, .xs{} };
					for (size_t q{ 0 }; q < nq; q++)
					{
						const std::optional<IloBoolVar>& x{ _variables.x (i, q) };
						if (x)
						{
							si.xs.push_front (*x);
						}
					}
					m_selectionIndices.emplace_front (std::move (si));
				}
			}
		}
	}

	void CutCallbackI::main ()
	{
		if (m_pOptions->enableSelectionCuts)
		{
			add_selection_cuts ();
		}
		if (m_pOptions->sizeCutMode != Options::ESizeCutMode::None)
		{
			add_size_cuts ();
		}
	}

	void CutCallbackI::add_selection_cuts ()
	{
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now ();
		{
			std::scoped_lock lock{ m_selectionMutex };
			std::forward_list<SelectionIndex>::iterator yoit = m_selectionIndices.before_begin (), yit = std::next (yoit);
			while (yit != m_selectionIndices.end ())
			{
				SelectionIndex& si{ *yit };
				IloNum vy = getValue (si.y);
				bool useful{ false };
				{
					std::forward_list<IloBoolVar>::iterator xoit = si.xs.before_begin (), xit = std::next (xoit);
					while (xit != si.xs.end ())
					{
						if (getValue (*xit) > vy)
						{
							add (*xit - si.y <= 0);
							m_pStatistics->nSelectionCuts++;
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
		std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		m_pStatistics->selectionCutElapsedTime += elapsedTime.count ();
	}

	void CutCallbackI::add_size_cuts ()
	{
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now ();
		switch (m_pOptions->sizeCutMode)
		{
			case Options::ESizeCutMode::Heuristic:
				add_heuristic_size_cuts ();
				break;
			case Options::ESizeCutMode::Optimal:
				add_optimal_size_cut ();
				break;
		}
		std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		m_pStatistics->sizeCutElapsedTime += elapsedTime.count ();
	}

	void CutCallbackI::add_optimal_size_cut ()
	{

	}

	void CutCallbackI::add_heuristic_size_cuts ()
	{

	}

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics)
	{
		return IloCplex::Callback{ new (_env) CutCallbackI {_env, _variables, _options, _statistics} };
	}

}