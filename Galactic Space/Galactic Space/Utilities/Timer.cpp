/**********************************************************
* Filename:  		Timer.cpp
* Date:      		11/24/2012
* Mod. Date: 		11/24/2012
* Mod. Initials:	CB
* Author:    		Chad Bradbury
* Purpose:   		Implements timer class
**********************************************************/

#include "Timer.h"
#include <Windows.h>

__int64		CTimer::myCpuSpeed = -1;

void CTimer::GetProcessorSpeed()
{
	if (myCpuSpeed != -1)
		return;

    //CPUInfo* pInfo = this;
    LARGE_INTEGER qwWait, qwStart, qwCurrent;
    QueryPerformanceCounter(&qwStart);
    QueryPerformanceFrequency(&qwWait);
    qwWait.QuadPart >>= 5;
    unsigned __int64 Start = __rdtsc();
    do
    {
        QueryPerformanceCounter(&qwCurrent);
    }while(qwCurrent.QuadPart - qwStart.QuadPart < qwWait.QuadPart);

	myCpuSpeed = (long long)(((__rdtsc() - Start) << 5) / 1000.f);
}

void CTimer::Init()
{
	//QueryPerformanceFrequency((LARGE_INTEGER*)&myCpuSpeed);
	GetProcessorSpeed();
	clockTickStart = GetCpuClocks();
	clockTickNow = GetCpuClocks();
	deltaTime = 0.0;
	elapsedTime = 0.0;
	maxDeltaTime = 0.0;
	minDeltaTime = 999.999;
	averageDeltaTime = 0.0;
}

//Taken from http://stackoverflow.com/questions/275004/c-timer-function-to-provide-time-in-nano-seconds
__int64 CTimer::GetCpuClocks()
{

    // Counter
    struct { int low, high; } counter;

    // Use RDTSC instruction to get clocks count
    __asm push EAX
    __asm push EDX
    __asm __emit 0fh __asm __emit 031h // RDTSC
    __asm mov counter.low, EAX
    __asm mov counter.high, EDX
    __asm pop EDX
    __asm pop EAX

    // Return result
    return *(__int64 *)(&counter);

}

void CTimer::Update()
{
	clockTickNow = GetCpuClocks();
	deltaClockTicks = (clockTickNow - clockTickStart);
	clockTickStart = clockTickNow;

	deltaTime = (double(deltaClockTicks)) / myCpuSpeed;	
	elapsedTime += deltaTime;

	if (deltaTime>maxDeltaTime)
		(maxDeltaTime) ? maxDeltaTime = deltaTime:maxDeltaTime = 0.1;

	if (deltaTime<minDeltaTime)
		minDeltaTime = deltaTime;

	(averageDeltaTime) ? averageDeltaTime = (averageDeltaTime+deltaTime) * 0.5 : averageDeltaTime += deltaTime;

	if (deltaTime > 33) 
		deltaTime = 33;
	else if(deltaTime < 17)
		deltaTime = 17;

	deltaTimeOverload = -1;
}

__int64 CTimer::GetDeltaClockTicks()
{
	return deltaClockTicks;
}

double CTimer::GetElapsedTime()
{
	return elapsedTime;
}

double CTimer::GetDeltaTime()
{
	if (deltaTimeOverload != -1)
		return deltaTimeOverload;

	return deltaTime;
}

float CTimer::GetDeltaTimeFloat()
{
	if (deltaTimeOverload != -1)
		return (float)deltaTimeOverload;

	return (float)deltaTime;
}

double CTimer::GetDeltaTimeInSeconds()
{
	if (deltaTimeOverload != -1)
		return deltaTimeOverload / 1000.f;

	return deltaTime / 1000.0f;
}

void CTimer::Reset()
{
	clockTickStart = GetCpuClocks();
	clockTickNow = GetCpuClocks();
	deltaTime		  = 0.0;
	elapsedTime		  = 0.0;
	maxDeltaTime	  = 0.0;
	minDeltaTime	  = 999.999;
	averageDeltaTime  = 0.0;
	deltaTimeOverload = -1;
}

void CTimer::SetDeltaTime(float _timeInSeconds)
{
	deltaTimeOverload = (double)(_timeInSeconds * 1000.f);
}
