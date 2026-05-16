/**
 * @brief Delay (echo) effect using a ring buffer.
 *
 * Adds a delayed copy of the audio signal to the original signal.
 * The delayed samples are kept in a circular buffer so we can
 * read them back after the chosen delay time.
 */

#ifndef DELAY_H
#define DELAY_H

#include <vector>
#include "core/Constants.h"

class Delay {
public:
    /**
     * @brief Build the delay effect.
     *
     * Initializes the circular buffer with zeros.
     */
    Delay();

    /**
     * @brief Apply the delay effect to the audio buffer.
     *
     * @param buffer    Audio buffer to modify (interleaved stereo).
     * @param frames    Number of frames to process.
     * @param delayTime Delay duration in seconds (0.0 to 1.0).
     * @param delayMix  Echo amplitude (0.0 = no echo, 1.0 = max).
     * @param enabled   true to apply the effect, false to leave the audio alone.
     */
    void process(float* buffer, int frames, float delayTime,
                 float delayMix, bool enabled);

private:
    /// Ring buffer holding the past samples used to produce the echo.
    std::vector<float> circularBuffer;

    /// Next index where a sample will be written in @c circularBuffer.
    int writeIndex{0};
};

#endif // DELAY_H
