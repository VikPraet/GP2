#pragma once
#include <chrono>

class GameTime final
{
public:
    static void Start();
    static void Update();

    static float GetDeltaTime() { return m_SmoothedDeltaTime * m_TimeScale; }
    static float GetUnScaledDeltaTime() { return m_SmoothedDeltaTime; }
    static float GetFixedDeltaTime() { return m_FixedTimeStep; }

    static void SetTimeScale(float timeScale) { m_TimeScale = timeScale; }
    static float GetTimeScale() { return m_TimeScale; }

private:
    GameTime() = default;

    static inline std::chrono::high_resolution_clock::time_point m_startTime;
    static inline std::chrono::high_resolution_clock::time_point m_lastTime;
    static inline float m_DeltaTime{};
    static inline float m_TimeScale{ 1.0f };
    static inline constexpr float m_FixedTimeStep = 0.02f;

    // Smoothing variables
    static inline float m_SmoothedDeltaTime{};
    static const int m_DeltaTimeWindow{ 15 };
    static inline float m_DeltaTimeBuffer[15]{};
    static inline int m_BufferIndex{};
};
