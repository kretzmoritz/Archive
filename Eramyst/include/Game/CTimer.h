////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/10/2012
// System implementation of as3 style timer class
////////////////////////////////////////////////////////////

#ifndef TIMER
#define TIMER

#define PWUSEGLOBALSINGLETONS
#include "../pwGraphicsSystem.hpp"

#include <vector>

//Valid events
#define TimerTick 0
#define TimerComplete 1

class CTimer
{
	typedef struct TimerEvent
	{
		int type;
		void *function;
		const void *context;
	};

	typedef void (*eventFunction)();
	typedef void (*handler_t)(const void *context); // Enables member function usage

	public:
        CTimer();      
        CTimer(float _time, unsigned long _tick = 0);
		~CTimer();

		void start();
		void stop();
		void reset();
		
		float getElapsedTime() const;
		unsigned long getTickCount() const;
		bool getStopped() const;

		void changeTimerSettings(float _time, unsigned long _tick = 0);

		void addEventListener(int _type, void (*_function)());
		void addEventListener(int _type, handler_t _h, const void *_context);
		void removeEventListener(int _type, void (*_function)());
		void removeEventListener(int _type, handler_t _h, const void *_context);

		static void update();
	private:
		static std::vector<CTimer*> m_vUpdate;
		std::vector<TimerEvent> m_vEvents;

		bool m_bStopped;
		bool m_bReset;
		
		float m_fElapsedTime;
		float m_fTimePerTick;

		unsigned long m_lNumberOfTicks;
		unsigned long m_lCurrentTickCount;
};

#endif