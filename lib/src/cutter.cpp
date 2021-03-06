#include <index-selector-lib/cutter.hpp>
#include <chrono>
#include <stdexcept>

namespace IndexSelector
{

	Cutter::Callback::Callback (Cutter& _cutter, bool _owner, bool _shared) : IloCplex::UserCutCallbackI{ _cutter.manager.env }, m_cutter{ _cutter }, m_owner{ _owner }, m_shared{ _shared }
	{}

	void Cutter::Callback::main ()
	{
		m_cutter.manager.startCutter ();
		m_cutter.cut (*this);
		m_cutter.manager.endCutter ();
	}

	IloCplex::CallbackI* Cutter::Callback::duplicateCallback () const
	{
		if (m_shared)
		{
			return new (getEnv ()) Callback{ m_cutter, false, true };
		}
		else
		{
			return new (getEnv ()) Callback{ *m_cutter.clone (), true, false };
		}
	}

	Cutter::Callback::~Callback ()
	{
		if (m_owner)
		{
			delete& m_cutter;
		}
	}

	Real Cutter::Callback::getValue (IloBoolVar _var)
	{
		return static_cast<Real>(IloCplex::UserCutCallbackI::getValue (_var));
	}

	void Cutter::Callback::add (IloConstraint _constraint, Options::ECutManagement _management)
	{
		switch (_management)
		{
			case Options::ECutManagement::CannotPurge:
				IloCplex::UserCutCallbackI::add (_constraint, IloCplex::CutManagement::UseCutForce);
				break;
			case Options::ECutManagement::CanPurgeLater:
				IloCplex::UserCutCallbackI::add (_constraint, IloCplex::CutManagement::UseCutPurge);
				break;
			case Options::ECutManagement::CanFilter:
				IloCplex::UserCutCallbackI::add (_constraint, IloCplex::CutManagement::UseCutFilter);
				break;
			case Options::ECutManagement::Local:
				IloCplex::UserCutCallbackI::addLocal (_constraint);
				break;
		}
		m_cutter.manager.addCut ();
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
		if (m_shared)
		{
			lock ();
		}
	}

	void Cutter::Callback::unlockIfShared ()
	{
		if (m_shared)
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

	bool Cutter::shouldShare () const
	{
		return false;
	}

	IloCplex::Callback Cutter::createCallback (bool _own)
	{
		return IloCplex::Callback{ new (manager.env) Callback {*this, _own, shouldShare () } };
	}

	void Cutter::Manager::addCut ()
	{
		m_nCuts++;
	}

	void Cutter::Manager::startCutter ()
	{
		m_timeMutex.lock ();
		if (m_nRunningCutters++ == 0)
		{
			m_startTime = std::chrono::high_resolution_clock::now ();
		}
		m_timeMutex.unlock ();
	}

	void Cutter::Manager::endCutter ()
	{
		m_timeMutex.lock ();
		if (--m_nRunningCutters == 0)
		{
			std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now ();
			m_elapsedTime += endTime - m_startTime;
		}
		m_timeMutex.unlock ();
	}

	void Cutter::Manager::lock ()
	{
		m_dataMutex.lock ();
	}

	void Cutter::Manager::unlock ()
	{
		m_dataMutex.unlock ();
	}

	Cutter::Manager::Manager (IloEnv _env, const VariableMatrix& _variables, const Options& _options) : env{ _env }, variables{ _variables }, options{ _options }
	{
		options.validate ();
	}

	size_t Cutter::Manager::nCuts () const
	{
		return m_nCuts;
	}

	double Cutter::Manager::elapsedTime () const
	{
		return std::chrono::duration<double> (m_elapsedTime).count ();
	}

}