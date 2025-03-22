#include "LatencyChecker.h"
int LatencyChecker::_latency;
int LatencyChecker::_latLimit;

void LatencyChecker::init()
{
	_latency = 0;
	_latLimit = LIMIT;
}

void LatencyChecker::addLatency()
{
	++_latency;
}

void LatencyChecker::substractLatency()
{
	--_latency;
}

int LatencyChecker::getLatency()
{
	return _latency;
}

int LatencyChecker::getLatLimit()
{
	return _latLimit;
}