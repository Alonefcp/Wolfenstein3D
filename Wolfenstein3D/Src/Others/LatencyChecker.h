#pragma once
#include <queue>
#define LIMIT 2

class LatencyChecker {

private:
	static int _latency;
	static int _latLimit;
	
public:
	/// <summary>
	/// Initalization
	/// </summary>
	static void init();

	/// <summary>
	/// Adds one of latency
	/// </summary>
	static void addLatency();

	/// <summary>
	/// Substracts one of latency
	/// </summary>
	static void substractLatency();

	/// <summary>
	/// Returns the current latency
	/// </summary>
	/// <returns></returns>
	static int getLatency();

	/// <summary>
	/// Return the latency limit
	/// </summary>
	/// <returns></returns>
	static int getLatLimit();
};