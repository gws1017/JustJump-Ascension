#pragma once
class Timer
{
public:
	Timer() = default;

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	bool Initialize();
	void Update(float lock_fps = 60.0f);

public:

	float GetDeltaTime() const { return m_delta_time; }
	UINT GetFrameRate() const { return m_frame_rate; }

private:
	LARGE_INTEGER m_frequncy = { 0 };
	LARGE_INTEGER m_prev_time = { 0 };
	LARGE_INTEGER m_curr_time = { 0 };

	UINT m_frame_rate = 0;
	UINT m_frame_count = 0;
	float m_time_per_tick = 0.0f;
	float m_delta_time = 0.0f;
	float m_fps_delta_time = 0.0f;
};
