#include <index-selector-lib/cutter.hpp>
#include <chrono>
#include <stdexcept>

namespace IndexSelector
{

	Cutter::Callback::Callback (Cutter& _cutter, bool _owner) : IloCplex::UserCutCallbackI{ _cutter.manager.env }, m_cutter{ _cutter }, m_owner{ _owner }
	{
		std::cout << "Callback created" << std::endl;
	}

	void Cutter::Callback::main ()
	{
		m_cutter.manager.startCutter ();
		m_cutter.cut (*this);
		m_cutter.manager.endCutter ();
	}

	IloCplex::CallbackI* Cutter::Callback::duplicateCallback () const
	{
		std::cout << "Callback cloned" << std::endl;
		if (m_cutter.manager.options.shareCutters)
		{
			return new (getEnv ()) Callback{ m_cutter, false };
		}
		else
		{
			return new (getEnv ()) Callback{ *m_cutter.clone (), true };
		}
	}

	Cutter::Callback::~Callback ()
	{
		if (m_owner)
		{
			delete& m_cutter;
		}
		std::cout << "Callback destroyed" << std::endl;
	}

	Real Cutter::Callback::getValue (IloBoolVar _var)
	{
		return static_cast<Real>(IloCplex::UserCutCallbackI::getValue (_var));
	}

	IloConstraint Cutter::Callback::add (IloConstraint _constraint, IloCplex::CutManagement _management)
	{
		m_cutter.manager.addCut ();
		return IloCplex::UserCutCallbackI::add (_constraint, _management);
	}

	IloConstraint Cutter::Callback::addLocal (IloConstraint _constraint)
	{
		m_cutter.manager.addCut ();
		return IloCplex::UserCutCallbackI::addLocal (_constraint);
	}

	void Cutter::Callback::lock ()
	{
		m_cutter.manager.lock ();
	}

	void Cutter::Callback::unlock ()
	{
		m_cutter.manager.unlock ();
	}

	void Cutter::Callback::lockIfShared ()
	{
		if (m_cutter.manager.options.shareCutters)
		{
			lock ();
		}
	}

	void Cutter::Callback::unlockIfShared ()
	{
		if (m_cutter.manager.options.shareCutters)
		{
			unlock ();
		}
	}

	Cutter::Cutter (Cutter::Manager& _manager) : manager{ _manager }
	{}

	Cutter* Cutter::clone () const
	{
		throw std::logic_error ("Cutter must be shared since it does not implement clone()");
	}

	IloCplex::Callback Cutter::createCallback (bool _own)
	{
		return IloCplex::Callback{ new (manager.env) Callback {*this, _own} };
	}

	void Cutter::Manager::addCut ()
	{
		m_nCuts++;
	}

	void Cutter::Manager::startCutter ()
	{
#ifdef INDEX_SELECTOR_MEASURE_TIME
		m_timeMutex.lock ();
		if (m_nRunningCutters++ == 0)
		{
			m_startTime = std::chrono::high_resolution_clock::now ();
		}
		m_timeMutex.unlock ();
#endif
	}

	void Cutter::Manager::endCutter ()
	{
#ifdef INDEX_SELECTOR_MEASURE_TIME
		m_timeMutex.lock ();
		if (--m_nRunningCutters == 0)
		{
			std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
			m_elapsedTime += endTime - m_startTime;
		}
		m_timeMutex.unlock ();
#endif
	}

	void Cutter::Manager::lock ()
	{
		m_dataMutex.lock ();
	}

	void Cutter::Manager::unlock ()
	{
		m_dataMutex.unlock ();
	}

	Cutter::Manager::Manager (IloEnv _env, const VariableMatrix& _variables, const Options& _options) : env{ _env }, variables{ _variables }, options{ _options } {}

	size_t Cutter::Manager::nCuts () const
	{
		return m_nCuts;
	}

	double Cutter::Manager::elapsedTime () const
	{
#ifdef INDEX_SELECTOR_MEASURE_TIME
		return std::chrono::duration<double> (m_elapsedTime).count ();
#else
		return 0;
#endif
	}

}