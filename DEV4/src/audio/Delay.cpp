/**
 * @brief Delay effect implementation.
 *
 * Implements the algorithm described in the project brief:
 *   out = out + delay_mix * buffer[index_read]
 *   buffer[index_write] = out
 *
 * The ring buffer always stays one second of stereo audio long, so the
 * delay time can range from 0 to 1 second without ever overflowing.
 */

#include "Delay.h"
#include <algorithm> // std::clamp

Delay::Delay() : circularBuffer(DELAY_BUFFER_SIZE, 0.0f) {}

void Delay::process(float* buffer, int frames, float delayTime,
                    float delayMix, bool enabled) {
    if (!enabled) return;

    // Convert the delay time (in seconds) to a sample offset in the
    // ring buffer. Multiply by NUM_CHANNELS so the offset stays
    // sample-accurate in the interleaved buffer.
    int delaySamples = static_cast<int>(delayTime * SAMPLE_RATE) * NUM_CHANNELS;

    // Clamp the offset to a safe range to avoid reading samples that
    // have not been written yet or going out of bounds.
    delaySamples = std::clamp(delaySamples, 2, DELAY_BUFFER_SIZE - 2);

    // Compute the read index: writeIndex - delaySamples, wrapped around.
    int readIndex = (writeIndex - delaySamples + DELAY_BUFFER_SIZE)
                    % DELAY_BUFFER_SIZE;

    int totalSamples = frames * NUM_CHANNELS;

    for (int i = 0; i < totalSamples; i++) {
        // Read the echoed sample from the past.
        float delayed = circularBuffer[readIndex];

        // Algorithm from the project brief (section 4.2.8):
        //   out = out + delay_mix * buffer[index_read]
        //   buffer[index_write] = out
        // Feeding the mixed signal back into the buffer is what creates
        // the natural decaying echo when delay_mix < 1.0.
        buffer[i] = buffer[i] + delayMix * delayed;
        circularBuffer[writeIndex] = buffer[i];

        // Advance both indices, wrapping around the ring buffer.
        readIndex  = (readIndex  + 1) % DELAY_BUFFER_SIZE;
        writeIndex = (writeIndex + 1) % DELAY_BUFFER_SIZE;
    }
}
