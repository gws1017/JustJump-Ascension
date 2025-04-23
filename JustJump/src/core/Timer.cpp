#include "Global.h"
#include "core/Timer.h"

bool Timer::Initialize()
{
	if (QueryPerformanceFrequency(static_cast<LARGE_INTEGER*>(&m_frequncy)))
	{
		QueryPerformanceCounter(&m_prev_time);
		m_time_per_tick = 1.0f / static_cast<float>(m_frequncy.QuadPart);
		return true;
	}
	else
	{
		std::cout << "Timer is not supprted" << std::endl;
		return false;
	}
	return false;
}

void Timer::Update(float lock_fps)
{
	QueryPerformanceCounter(&m_curr_time);

	m_delta_time = (m_curr_time.QuadPart - m_prev_time.QuadPart) * m_time_per_tick;

	if (lock_fps > 0.0f)
	{
		while (m_delta_time < (1.0f / lock_fps))
		{
			QueryPerformanceCounter(&m_curr_time);
			m_delta_time = (m_curr_time.QuadPart - m_prev_time.QuadPart) * m_time_per_tick;
		}
	}
	m_prev_time = m_curr_time;

	++m_frame_count;
	m_fps_delta_time += m_delta_time;

	if (m_fps_delta_time >= 1.0f)
	{
		m_fps_delta_time -= 1.0f;
		m_frame_rate = m_frame_count;
		m_frame_count = 0;
	}
}
