#pragma once

#include <chrono>

class OdaTimer
{
public:

	OdaTimer()
	{
		last = std::chrono::steady_clock::now();
	}
	/// @brief  Returns the time elapsed since the last time Mark() was called, then updates mark point
	float Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = last - old;
		return frameTime.count();
	}
	/// @brief  Returns the time passed since the last time Mark() was called without resetting the mark point
	float Peek() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}

private:
	std::chrono::steady_clock::time_point last;
};