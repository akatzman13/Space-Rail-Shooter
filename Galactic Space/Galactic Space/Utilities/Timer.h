/**********************************************************
* Filename:  		Timer.h
* Date:      		11/24/2012
* Mod. Date: 		11/24/2012
* Mod. Initials:	CB
* Author:    		Chad Bradbury & Mike Suwayda
* Purpose:   		Timer class header
**********************************************************/

#ifndef TIMER_H
#define TIMER_H

class CTimer
{
private:

	__int64 clockTickStart;
	__int64 clockTickNow;
	
	__int64 deltaClockTicks;

	double deltaTime;
	double elapsedTime;
	double maxDeltaTime;
	double minDeltaTime;
	double averageDeltaTime;

	double deltaTimeOverload;

public:

	static __int64 myCpuSpeed;

	CTimer() {};
	~CTimer() {};

	void Init();
	void Update();

	__int64 GetDeltaClockTicks();
	double GetElapsedTime();
	double GetDeltaTime();
	float GetDeltaTimeFloat();
	double GetDeltaTimeInSeconds();
	void Reset();

	void SetDeltaTime(float _timeInSeconds);

private:
	__int64 GetCpuClocks();
	void GetProcessorSpeed();
};

#endif