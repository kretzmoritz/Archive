////////////////////////////////////////////////////////////
// Moritz Kretz
// System implementation of as3 style timer class
////////////////////////////////////////////////////////////

#ifndef SYL_TIMER
#define SYL_TIMER

#include <time.h>
#include <vector>

// Valid events
#define TimerTick 0
#define TimerComplete 1

class sylTimer
{
	class sylFunction
	{
		public:
			virtual void callFunction() {}
			virtual void* getContext() { return NULL; }
			virtual void* getMemberFunction() { return NULL; }
	};

	template<class T>
	class sylHelper : public sylFunction
	{
		public:
			void callFunction() { (_context->*_p)(); }
			void* getContext() { return (void*&)(_context); }
			void* getMemberFunction() { return (void*&)(_p); }
		
			T* _context; // Class instance
			void (T::*_p)(); // Member function
	};

	struct TimerEvent
	{
		unsigned char type;
		void (*function)(); // Static function
		sylFunction* member_function;
	};

public:
	sylTimer();
	sylTimer(float _time, unsigned int _tick = 0);
	~sylTimer();

	void operator()(float _time, unsigned int _tick = 0) // Functor to change timer settings
	{
		reset();

		m_iTimePerTick = (unsigned int)(_time * CLOCKS_PER_SEC);
		m_iNumberOfTicks = _tick;
	}

	void start();
	void stop();
	void reset();

	// Since timer start
	float getElapsedTime() const;
	unsigned int getTickCount() const;
	bool getStopped() const;

	// Use to add static function event: event type, function pointer
	void addEventListener(unsigned char _type, void (*_function)());
		
	// Use to remove static function event: event type, function pointer
	void removeEventListener(unsigned char _type, void (*_function)());

	template<class T>
	// Use to add member function event: event type, member function pointer, class instance
	void addEventListener(unsigned char _type, void (T::*_p)(), T* _context)
	{
		bool hasEvent = false;

		for(unsigned int i = 0; i < m_vEvents.size(); i++)
		{
			if(m_vEvents[i].type == _type && m_vEvents[i].function == NULL && m_vEvents[i].member_function->getMemberFunction() == (void*&)_p && m_vEvents[i].member_function->getContext() == _context)
			{
				hasEvent = true;
				break;
			}
		}

		if(!hasEvent)
		{
			sylHelper<T>* helper = new sylHelper<T>();
			helper->_context = _context;
			helper->_p = _p;

			TimerEvent temp;
			temp.type = _type;
			temp.function = NULL;
			temp.member_function = helper;
			helper = NULL;

			m_vEvents.push_back(temp);
		}
	}

	template<class T>
	// Use to remove member function event: event type, member function pointer, class instance
	void removeEventListener(unsigned char _type, void (T::*_p)(), T* _context)
	{
		for(unsigned int i = 0; i < m_vEvents.size(); i++)
		{
			if(m_vEvents[i].type == _type && m_vEvents[i].function == NULL && m_vEvents[i].member_function->getMemberFunction() == (void*&)_p && m_vEvents[i].member_function->getContext() == _context)
			{
				delete m_vEvents[i].member_function;
				m_vEvents.erase(m_vEvents.begin() + i);
				break;
			}
		}
	}

	// Call once in program loop to keep all timers updated
	static void Update();

private:
	static std::vector<sylTimer*> m_vUpdate;
	std::vector<TimerEvent> m_vEvents;

	bool m_bStopped;
	bool m_bReset;

	unsigned int m_iElapsedTime;
	unsigned int m_iPreviousClock;
	unsigned int m_iCurrentClock;

	unsigned int m_iTimePerTick;
	unsigned int m_iNumberOfTicks;

	unsigned int m_iCurrentTickCount;
};

#endif
