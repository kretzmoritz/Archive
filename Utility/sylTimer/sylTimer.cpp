#include "sylTimer.h"

// Definition of static vector
std::vector<sylTimer*> sylTimer::m_vUpdate;

sylTimer::sylTimer() :
	m_bStopped(true),
	m_bReset(true),
	m_iElapsedTime(0),
	m_iTimePerTick(1000),
	m_iNumberOfTicks(0),
	m_iCurrentTickCount(0)
{
	m_vUpdate.push_back(this);
}

sylTimer::sylTimer(float _time, unsigned int _tick)
{
	m_bStopped = true;
	m_bReset = true;

	m_iElapsedTime = 0;

	m_iTimePerTick = (unsigned int)(_time * CLOCKS_PER_SEC);
	m_iNumberOfTicks = _tick;

	m_iCurrentTickCount = 0;

	m_vUpdate.push_back(this);
}

sylTimer::~sylTimer()
{
	for(unsigned int i = 0; i < m_vUpdate.size(); i++)
	{
		if(m_vUpdate[i] == this)
		{
			for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
			{
				delete m_vUpdate[i]->m_vEvents[j].member_function;
			}
			
			m_vUpdate.erase(m_vUpdate.begin() + i);
			break;
		}
	}
}

void sylTimer::start()
{
	if(m_bStopped)
	{
		m_iPreviousClock = clock();
		m_bStopped = false;
	}

	if(m_bReset)
	{
		m_iCurrentTickCount = 0;
		m_iElapsedTime = 0;
		m_bReset = false;
	}
}

void sylTimer::stop()
{
	m_bStopped = true;
}

void sylTimer::reset()
{
	m_bStopped = true;
	m_bReset = true;
}

float sylTimer::getElapsedTime() const
{
	return (float)m_iElapsedTime / CLOCKS_PER_SEC;
}

unsigned int sylTimer::getTickCount() const
{
	return m_iCurrentTickCount;
}

bool sylTimer::getStopped() const
{
	return m_bStopped;
}

void sylTimer::addEventListener(unsigned char _type, void (*_function)())
{
	bool hasEvent = false;

	for(unsigned int i = 0; i < m_vEvents.size(); i++)
	{
		if(m_vEvents[i].type == _type && m_vEvents[i].function == _function)
		{
			hasEvent = true;
			break;
		}
	}

	if(!hasEvent)
	{
		TimerEvent temp;
		temp.type = _type;
		temp.function = _function;

		m_vEvents.push_back(temp);
	}
}

void sylTimer::removeEventListener(unsigned char _type, void (*_function)())
{
	for(unsigned int i = 0; i < m_vEvents.size(); i++)
	{
		if(m_vEvents[i].type == _type && m_vEvents[i].function == _function)
		{
			m_vEvents.erase(m_vEvents.begin() + i);
			break;
		}
	}
}

void sylTimer::Update()
{
	int iTickAmount;
	
	for(unsigned int i = 0; i < m_vUpdate.size(); i++)
	{
		// Timer management
		if(!m_vUpdate[i]->m_bStopped)
		{
			m_vUpdate[i]->m_iCurrentClock = clock();
			m_vUpdate[i]->m_iElapsedTime += m_vUpdate[i]->m_iCurrentClock - m_vUpdate[i]->m_iPreviousClock;
			m_vUpdate[i]->m_iPreviousClock = m_vUpdate[i]->m_iCurrentClock;

			if(m_vUpdate[i]->m_iNumberOfTicks != 0) // Specific number of ticks
			{
				if(m_vUpdate[i]->m_iElapsedTime >= (m_vUpdate[i]->m_iCurrentTickCount + 1) * m_vUpdate[i]->m_iTimePerTick && m_vUpdate[i]->m_iCurrentTickCount < m_vUpdate[i]->m_iNumberOfTicks)
				{
					iTickAmount = 1 + (m_vUpdate[i]->m_iElapsedTime - (m_vUpdate[i]->m_iCurrentTickCount + 1) * m_vUpdate[i]->m_iTimePerTick) / m_vUpdate[i]->m_iTimePerTick;

					for(int k = 0; k < iTickAmount; k++)
					{
						// Timer tick event
						for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
						{
							if(m_vUpdate[i]->m_vEvents[j].type == TimerTick)
							{
								if(m_vUpdate[i]->m_vEvents[j].function != NULL)
								{
									m_vUpdate[i]->m_vEvents[j].function();
								}
								else
								{
									m_vUpdate[i]->m_vEvents[j].member_function->callFunction();
								}
							}
						}

						m_vUpdate[i]->m_iCurrentTickCount++;
					}
				}

				if(m_vUpdate[i]->m_iCurrentTickCount == m_vUpdate[i]->m_iNumberOfTicks)
				{
					// Timer complete event
					for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
					{
						if(m_vUpdate[i]->m_vEvents[j].type == TimerComplete)
						{
							if(m_vUpdate[i]->m_vEvents[j].function != NULL)
							{
								m_vUpdate[i]->m_vEvents[j].function();
							}
							else
							{
								m_vUpdate[i]->m_vEvents[j].member_function->callFunction();
							}
						}
					}

					m_vUpdate[i]->reset();

					// Remove all events
					for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
					{
						delete m_vUpdate[i]->m_vEvents[j].member_function;
					}

					m_vUpdate[i]->m_vEvents.clear();
				}
			}
			else // Infinite number of ticks
			{
				if(m_vUpdate[i]->m_iElapsedTime >= (m_vUpdate[i]->m_iCurrentTickCount + 1) * m_vUpdate[i]->m_iTimePerTick)
				{
					iTickAmount = 1 + (m_vUpdate[i]->m_iElapsedTime - (m_vUpdate[i]->m_iCurrentTickCount + 1) * m_vUpdate[i]->m_iTimePerTick) / m_vUpdate[i]->m_iTimePerTick;
					
					for(int k = 0; k < iTickAmount; k++)
					{
						// Timer tick event
						for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
						{
							if(m_vUpdate[i]->m_vEvents[j].type == TimerTick)
							{
								if(m_vUpdate[i]->m_vEvents[j].function != NULL)
								{
									m_vUpdate[i]->m_vEvents[j].function();
								}
								else
								{
									m_vUpdate[i]->m_vEvents[j].member_function->callFunction();
								}
							}
						}

						m_vUpdate[i]->m_iCurrentTickCount++;
					}
				}
			}
		}
	}
}