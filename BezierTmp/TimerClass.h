#pragma once
#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_
#include <windows.h>
#include <string>
#include <stdio.h>

class TimerClass 
{
public:
	TimerClass(void);
	TimerClass(const TimerClass&);
	~TimerClass(void);

	bool Initialize();
	void Frame();

	float GetTime();
	static void GetSystemTime(char* time, char* date = nullptr);

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif