#include "..\..\include\Game\CTimer.h"

//Definition of static vector
std::vector<CTimer*> CTimer::m_vUpdate;

CTimer::CTimer() :
    m_bStopped(true),
    m_bReset(true),

    m_fElapsedTime(0.0f),
    m_fTimePerTick(0.0f),
    
	m_lNumberOfTicks(0)
{
    m_vUpdate.push_back(this);
}

CTimer::CTimer(float _time, unsigned long _tick)
{
	m_bStopped = true;
	m_bReset = true;
	
	m_fElapsedTime = 0.0f;
	m_fTimePerTick = _time;

	m_lNumberOfTicks = _tick;

	m_vUpdate.push_back(this);
}

CTimer::~CTimer()
{
	for(unsigned int i = 0; i < m_vUpdate.size(); i++)
	{
		if(m_vUpdate[i] == this)
		{
			m_vUpdate.erase(m_vUpdate.begin() + i);
			break;
		}
	}
}

void CTimer::start()
{
	m_bStopped = false;
	
	if(m_bReset)
	{
		m_lCurrentTickCount = 0;
		m_fElapsedTime = 0.0f;
		m_bReset = false;
	}
}

void CTimer::stop()
{
	m_bStopped = true;
}

void CTimer::reset()
{
	m_bStopped = true;
	m_bReset = true;
}

float CTimer::getElapsedTime() const
{
	return m_fElapsedTime;
}

unsigned long CTimer::getTickCount() const
{
	return m_lCurrentTickCount;
}

bool CTimer::getStopped() const
{
	return m_bStopped;
}

void CTimer::changeTimerSettings(float _time, unsigned long _tick)
{
	reset();

	m_fTimePerTick = _time;
	m_lNumberOfTicks = _tick;
}

void CTimer::addEventListener(int _type, void (*_function)())
{
	bool hasEvent = false;
	
	for(unsigned int i = 0; i < m_vEvents.size(); i++)
	{
		if(m_vEvents[i].type == _type && m_vEvents[i].function == _function && m_vEvents[i].context == NULL)
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
		temp.context = NULL;

		m_vEvents.push_back(temp);
	}
}

void CTimer::addEventListener(int _type, handler_t _h, const void *_context)
{
	bool hasEvent = false;
	
	for(unsigned int i = 0; i < m_vEvents.size(); i++)
	{
		if(m_vEvents[i].type == _type && m_vEvents[i].function == _h && m_vEvents[i].context == _context)
		{
			hasEvent = true;
			break;
		}
	}

	if(!hasEvent)
	{
		TimerEvent temp;
		temp.type = _type;
		temp.function = _h;
		temp.context = _context;

		m_vEvents.push_back(temp);
	}
}

void CTimer::removeEventListener(int _type, void (*_function)())
{
	for(unsigned int i = 0; i < m_vEvents.size(); i++)
	{
		if(m_vEvents[i].type == _type && m_vEvents[i].function == _function && m_vEvents[i].context == NULL)
		{
			m_vEvents.erase(m_vEvents.begin() + i);
			break;
		}
	}
}

void CTimer::removeEventListener(int _type, handler_t _h, const void *_context)
{
	for(unsigned int i = 0; i < m_vEvents.size(); i++)
	{
		if(m_vEvents[i].type == _type && m_vEvents[i].function == _h && m_vEvents[i].context == _context)
		{
			m_vEvents.erase(m_vEvents.begin() + i);
			break;
		}
	}
}

void CTimer::update()
{
	for(unsigned int i = 0; i < m_vUpdate.size(); i++)
	{
		//Timer management
		if(!m_vUpdate[i]->m_bStopped)
		{
			m_vUpdate[i]->m_fElapsedTime += glGraphics->GetLastFrameTime();

			if(m_vUpdate[i]->m_lNumberOfTicks != 0) //Specific number of ticks
			{
				if(m_vUpdate[i]->getElapsedTime() >= m_vUpdate[i]->m_fTimePerTick && m_vUpdate[i]->m_lCurrentTickCount < m_vUpdate[i]->m_lNumberOfTicks)
				{
					//Timer tick event
					for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
					{
						if(m_vUpdate[i]->m_vEvents[j].type == TimerTick)
						{
							if(m_vUpdate[i]->m_vEvents[j].context == NULL)
							{
								((eventFunction)m_vUpdate[i]->m_vEvents[j].function)();
							}
							else
							{
								((handler_t)m_vUpdate[i]->m_vEvents[j].function)(m_vUpdate[i]->m_vEvents[j].context);
							}
						}
					}
					
					m_vUpdate[i]->m_lCurrentTickCount++;
					m_vUpdate[i]->m_fElapsedTime -= m_vUpdate[i]->m_fTimePerTick;
				}
				
				if(m_vUpdate[i]->m_lCurrentTickCount == m_vUpdate[i]->m_lNumberOfTicks)
				{
					//Timer complete event
					for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
					{
						if(m_vUpdate[i]->m_vEvents[j].type == TimerComplete)
						{
							if(m_vUpdate[i]->m_vEvents[j].context == NULL)
							{
								((eventFunction)m_vUpdate[i]->m_vEvents[j].function)();
							}
							else
							{
								((handler_t)m_vUpdate[i]->m_vEvents[j].function)(m_vUpdate[i]->m_vEvents[j].context);
							}
						}
					}
					
					m_vUpdate[i]->reset();
				}
			}
			else //Infinite number of ticks
			{
				if(m_vUpdate[i]->getElapsedTime() >= m_vUpdate[i]->m_fTimePerTick)
				{
					//Timer tick event
					for(unsigned int j = 0; j < m_vUpdate[i]->m_vEvents.size(); j++)
					{
						if(m_vUpdate[i]->m_vEvents[j].type == TimerTick)
						{
							if(m_vUpdate[i]->m_vEvents[j].context == NULL)
							{
								((eventFunction)m_vUpdate[i]->m_vEvents[j].function)();
							}
							else
							{
								((handler_t)m_vUpdate[i]->m_vEvents[j].function)(m_vUpdate[i]->m_vEvents[j].context);
							}
						}
					}
					
					m_vUpdate[i]->m_lCurrentTickCount++;
					m_vUpdate[i]->m_fElapsedTime -= m_vUpdate[i]->m_fTimePerTick;
				}
			}
		}
	}
}