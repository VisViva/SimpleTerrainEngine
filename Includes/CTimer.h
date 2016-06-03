#ifndef CTIMER_H
#define CTIMER_H
#include "Main.h"

const unsigned long MAX_SAMPLE_COUNT = 50;

class CTimer
{
public:
	CTimer();
	virtual ~CTimer();
	void Tick(float fLockFPS = 0.0f);
	unsigned long GetFrameRate(LPTSTR lpszString = NULL) const;
	float GetTimeElapsed() const;
private:
	bool	m_PerfHardware;
	float	m_TimeScale;
	float	m_TimeElapsed;
	__int64	m_CurrentTime;
	__int64	m_LastTime;
	__int64	m_PerfFreq;
	float m_FrameTime[MAX_SAMPLE_COUNT];
	unsigned long m_SampleCount;
	unsigned long m_FrameRate;
	unsigned long m_FPSFrameCount;
	float m_FPSTimeElapsed;
};
#endif
