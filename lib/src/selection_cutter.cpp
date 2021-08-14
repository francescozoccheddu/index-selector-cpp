#include <index-selector-lib/selection_cutter.hpp>
/*
#include <ilconcert/ilothread.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <forward_list>
#include <concepts>

namespace IndexSelector
{

	class CutCallbackI final : public IloCplex::UserCutCallbackI
	{
		friend class Cutter;
	};


	struct Cutter
	{

	private:

		CutCallbackI* m_pCallback;
		IloFastMutex m_mutex{};

		void set_callback (CutCallbackI& _callback)
		{
			m_pCallback = &_callback;
		}

		void run ()
		{
			m_mutex.lock ();
			using namespace std::chrono;
			steady_clock::time_point startTime = high_resolution_clock::now ();
			{
				cut ();
			}
			steady_clock::time_point endTime = high_resolution_clock::now ();
			duration<double> elapsedTime = endTime - startTime;
			reportElapsedTime (elapsedTime.count ());
			m_mutex.unlock ();
		}

	protected:

		virtual void reportElapsedTime (double _elapsedTime) const = 0;
		virtual void cut () = 0;

		IloConstraint add (IloConstraint _constraint, IloCplex::CutManagement _management = IloCplex::CutManagement::UseCutForce)
		{
			return m_pCallback->add (_constraint, _management);
		}

		IloConstraint addLocal (IloConstraint _constraint)
		{
			return m_pCallback->addLocal (_constraint);
		}

	};


	class CutCallbackI final : public IloCplex::UserCutCallbackI
	{


		std::shared_ptr<DataHolder> m_dataHolder;

	protected:

		const VariableMatrix& m_variables;
		const Options& m_options;
		Solution::Statistics& m_statistics;
		const bool m_shared;


		CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics, bool _share, Cutter* _pCutter)
			: IloCplex::UserCutCallbackI{ _env }, m_variables{ _variables }, m_options{ _options }, m_statistics{ _statistics }, m_shared{ _share }, m_dataHolder{ .data{_env, _variables, _options} }
		{
			_options.validate ();
		}

		IloCplex::CallbackI* duplicateCallback () const override
		{
			if (m_shared)
			{
				return new (getEnv ()) TCallback{ *this };
			}
			else
			{
				return new (getEnv ()) TCallback{ *this };
			}
		}

		void main () override final
		{
			if (m_shared)
			{
				m_dataHolder->mutex.lock ();
			}
			using namespace std::chrono;
			steady_clock::time_point startTime = high_resolution_clock::now ();
			{
				cut ();
			}
			steady_clock::time_point endTime = high_resolution_clock::now ();
			duration<double> elapsedTime = endTime - startTime;
			reportElapsedTime (elapsedTime.count ());
			if (m_shared)
			{
				m_dataHolder->mutex.unlock ();
			}
		}

		virtual void reportElapsedTime (double _elapsedTime) = 0;
		virtual void cut () = 

	};

	class SizeCutCallbackI final : public IloCplex::UserCutCallbackI
	{

		struct SelectionIndex
		{
			IloBoolVar y;
			std::forward_list<IloBoolVar> xs;
		};

		struct Data
		{

			std::forward_list<SelectionIndex> selectionIndices{};
			IloFastMutex mutex{};

			~Data ()
			{
				std::cout << "Data deleted";
			}

		};

		std::shared_ptr<Data> m_data;

		void add_selection_cuts ();

		void add_optimal_size_cut ();

		void add_heuristic_size_cuts ();

		CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);

		friend IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics);

	protected:

		void main () override;
		IloCplex::CallbackI* duplicateCallback () const override;

	};



	IloCplex::CallbackI* CutCallbackI::duplicateCallback () const
	{
		return new (getEnv ()) CutCallbackI{ *this };
	}

	CutCallbackI::CutCallbackI (IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics) : IloCplex::UserCutCallbackI{ _env }
	{
		_options.validate ();
		std::forward_list<SelectionIndex> selectionIndices{};
		if (_options.enableSelectionCuts)
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
					selectionIndices.emplace_front (std::move (si));
				}
			}
		}
		IloBoolVarArray sizeZs{};
		IloCplex sizeCplex;
		if (_options.sizeCutMode == Options::ESizeCutMode::Optimal)
		{

		}
	}

	void CutCallbackI::main ()
	{
		using namespace std::chrono;
		if (m_pOptions->enableSelectionCuts)
		{
			steady_clock::time_point startTime = high_resolution_clock::now ();
			{
				add_selection_cuts ();
			}
			steady_clock::time_point endTime = high_resolution_clock::now ();
			duration<double> elapsedTime = endTime - startTime;
			m_pStatistics->selectionCutElapsedTime += elapsedTime.count ();
		}
		if (m_pOptions->sizeCutMode != Options::ESizeCutMode::None)
		{
			steady_clock::time_point startTime = high_resolution_clock::now ();
			{
				switch (m_pOptions->sizeCutMode)
				{
					case Options::ESizeCutMode::Heuristic:
						add_heuristic_size_cuts ();
						break;
					case Options::ESizeCutMode::Optimal:
						add_optimal_size_cut ();
						break;
				}
			}
			steady_clock::time_point endTime = high_resolution_clock::now ();
			duration<double> elapsedTime = endTime - startTime;
			m_pStatistics->selectionCutElapsedTime += elapsedTime.count ();
		}
	}

	void CutCallbackI::add_selection_cuts ()
	{
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

	void CutCallbackI::add_optimal_size_cut ()
	{}

	void CutCallbackI::add_heuristic_size_cuts ()
	{}

	IloCplex::Callback create_cut_callback (const IloEnv _env, const VariableMatrix& _variables, const Options& _options, Solution::Statistics& _statistics)
	{
		return IloCplex::Callback{ new (_env) CutCallbackI {_env, _variables, _options, _statistics} };
	}

}
*/