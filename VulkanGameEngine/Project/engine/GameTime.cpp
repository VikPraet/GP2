#include "GameTime.h"

#include <algorithm>

void GameTime::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

void GameTime::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
	m_lastTime = currentTime;

	// Smooth the delta time
	m_DeltaTimeBuffer[m_BufferIndex] = m_DeltaTime;
	m_BufferIndex = (m_BufferIndex + 1) % m_DeltaTimeWindow;

	m_SmoothedDeltaTime = 0.0f;
	for (int i = 0; i < m_DeltaTimeWindow; ++i)
	{
		m_SmoothedDeltaTime += m_DeltaTimeBuffer[i];
	}
	m_SmoothedDeltaTime /= m_DeltaTimeWindow;

	// clamped to 50ms (20 FPS)
	m_SmoothedDeltaTime = std::clamp(m_SmoothedDeltaTime, 0.0f, 0.05f);
}
