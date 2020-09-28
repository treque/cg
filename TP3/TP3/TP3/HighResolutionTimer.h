#ifndef HIGH_RESOLUTION_TIMER_H
#define HIGH_RESOLUTION_TIMER_H

#include <chrono>
#include <cstdint>

/**
 * @brief Provides a simple interface to start and stop a high-resolution chronometer-like timer and get
 * its average running time, its last running time or the time elapsed since it was last started. Currently at
 * microsecond resolution.
 */
class HighResolutionTimer
{
    using Clock      = std::chrono::high_resolution_clock;
    using Duration   = std::chrono::microseconds; //!< Resolution of the clock
    using DurationMs = std::chrono::duration<float, std::milli>;
    using Reading    = Clock::time_point;

public:
    HighResolutionTimer()
        : start_(Clock::now())
        , stop_(start_)
        , total_(0)
        , count_(0)
    {
    }

    inline void reset()
    {
        start_ = Clock::now();
        stop_  = start_;
        total_ = Duration::zero();
        count_ = 0;
    }

    inline void start()
    {
        start_ = Clock::now();
        count_++;
    }
    inline void stop()
    {
        stop_ = Clock::now();
        total_ += getDuration<Duration>(stop_ - start_);
    }

    inline float getLastTimeMs() const { return getNumTicks<DurationMs>(stop_ - start_); }
    inline float getAvgTimeMs() const { return getNumTicks<DurationMs>(total_) / count_; }
    inline float getTimeSinceLastStartMs() const { return getNumTicks<DurationMs>(Clock::now() - start_); }

private:
    Reading  start_;
    Reading  stop_;
    Duration total_; //!< Total time elapsed while this timer was running (not counting current, if still running)
    uint64_t count_;

private: // functions
    template <class T, class U>
    static inline float getNumTicks(const U& interval)
    {
        return getDuration<T>(interval).count();
    }

    template <class T, class U>
    static inline T getDuration(const U& interval)
    {
        return std::chrono::duration_cast<T>(interval);
    }
};

#endif // HIGH_RESOLUTION_TIMER_H
