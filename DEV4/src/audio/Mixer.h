/**
 * @brief Mixer that sums several audio buffers into one output buffer.
 *
 * Per the project brief, mixing two sources is simply adding their
 * samples together. We sum the 4 track buffers into the final output.
 */

#ifndef MIXER_H
#define MIXER_H

class Mixer {
public:
    /**
     * @brief Sum the 4 track buffers into the output buffer.
     *
     * @param output Output buffer to fill.
     * @param buf1   Buffer for track 1.
     * @param buf2   Buffer for track 2.
     * @param buf3   Buffer for track 3.
     * @param buf4   Buffer for track 4.
     * @param frames Number of frames to process (typically 256).
     */
    void process(float* output,
                 const float* buf1, const float* buf2,
                 const float* buf3, const float* buf4,
                 int frames);
};

#endif // MIXER_H
