#include <index-selector-lib/heuristic_size_cutter.hpp>
#include <limits>
#include <cmath>
#include <algorithm>

namespace IndexSelector
{

	HeuristicSizeCutter::HeuristicSizeCutter (Cutter::Manager& _manager) : SimpleCutter{ _manager }
	{
		if (manager.options.sizeCutMode != Options::ESizeCutMode::Heuristic)
		{
			throw std::logic_error{ "heuristic size cuts are disabled in options" };
		}
		const size_t nAy{ _manager.variables.nActiveYs () };
		m_candidates = std::vector<Candidate>{ nAy };
		{
			size_t ai{ 0 }, i{ 0 };
			while (ai < nAy)
			{
				const std::optional<IloBoolVar>& y{ manager.variables.y (i) };
				if (y)
				{
					m_candidates[ai++] = Candidate{ .size {_manager.variables.problem ().indices[i].size }, .y{ *y }, .minK{0} };
				}
				i++;
			}
			std::sort (m_candidates.begin (), m_candidates.end (), [] (const Candidate& _a, const Candidate& _b)
			{
				return _a.size > _b.size;
			});
		}
		m_maxK = std::min<size_t> (_manager.options.heuristicSizeCutsMaxSize, nAy);
		std::vector<size_t> nCandidates (std::max<size_t> (m_maxK - 1, 0));
		Real topSum{}, kSum{};
		size_t ai{}, sai{};
		const Real maxSize{ _manager.variables.problem ().maxSize };
		for (size_t k{ 2 }; k <= m_maxK; k++)
		{
			topSum += m_candidates[k - 2].size;
			Real minSize{ maxSize - topSum };
			while (sai < k)
			{
				kSum += m_candidates[sai].size;
				sai++;
			}
			ai = std::max<size_t> (ai, sai);
			while (ai < nAy and m_candidates[ai].size >= minSize)
			{
				ai++;
			}
			if (kSum >= maxSize)
			{
				if (!m_minK)
				{
					m_minK = k;
				}
				size_t mm{ ai };
				while (mm > 0 and m_candidates[--mm].minK == 0)
				{
					m_candidates[mm].minK = k;
				}
			}
		}
		if (m_minK)
		{
			m_valueSortedCandidates = std::vector<Candidate*> (m_candidates.size ());
			m_kCandidates = std::vector<Candidate*> (m_candidates.size ());
			m_chosen = std::vector<Candidate*> (m_maxK);
		}
		else
		{
			m_maxK = 0;
			m_candidates.resize (0);
		}
	}

	void HeuristicSizeCutter::addKCuts (Callback& _callback, size_t _k)
	{

	}

	void HeuristicSizeCutter::cut (Callback& _callback)
	{
		if (!m_maxK)
		{
			return;
		}
		_callback.lockIfShared ();
		m_valueSortedCandidates.clear ();
		for (Candidate& c : m_candidates)
		{
			c.value = _callback.getValue (c.y);
			if (c.value > 0)
			{
				m_valueSortedCandidates.push_back (&c);
			}
		}
		if (!m_valueSortedCandidates.empty ())
		{
			std::sort (m_valueSortedCandidates.begin (), m_valueSortedCandidates.end (), [] (const Candidate* _a, const Candidate* _b)
			{
				return _a->value > _b->size;
			});
			for (size_t k{ m_minK }; k <= m_maxK; k++)
			{
				m_kCandidates.clear ();
				Real topSum{};
				for (Candidate* c : m_valueSortedCandidates)
				{
					if (m_kCandidates.size () >= k - 1 and c->value < (topSum - k + 1))
					{
						break;
					}
					if (k >= c->minK)
					{
						if (m_kCandidates.size () < k - 1)
						{
							topSum += c->value;
						}
						m_kCandidates.push_back (c);
					}
				}
				addKCuts (_callback, k);
			}
		}
		_callback.unlockIfShared ();
	}

	bool HeuristicSizeCutter::shouldShare () const
	{
		return false;
	}

}