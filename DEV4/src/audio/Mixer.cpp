/**
 * @brief Mixer implementation.
 *
 * Sums the 4 per-track buffers into the output buffer.
 * The result is clamped to [-1.0, 1.0] to prevent clipping
 * when several tracks play loudly at the same time.
 */

#include "Mixer.h"
#include "core/Constants.h" // NUM_CHANNELS
#include <algorithm>        // std::clamp

void Mixer::process(float* output,
                    const float* buf1, const float* buf2,
                    const float* buf3, const float* buf4,
                    int frames) {
    int totalSamples = frames * NUM_CHANNELS;

    for (int i = 0; i < totalSamples; i++) {
        // Sum every track and clamp to [-1.0, 1.0] to avoid distortion
        // when all tracks play near their maximum level simultaneously.
        float mixed = buf1[i] + buf2[i] + buf3[i] + buf4[i];
        output[i] = std::clamp(mixed, -1.0f, 1.0f);
    }
}
