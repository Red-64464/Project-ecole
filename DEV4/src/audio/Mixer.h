#ifndef MIXER_H
#define MIXER_H

/**
 * @brief Melange plusieurs buffers audio en un seul buffer de sortie.
 */
class Mixer {
public:
    /**
     * @brief Melange 4 buffers audio dans un seul  buffer de sortie.
     *
     * @param output Buffer de sortie.
     * @param buf1 Buffer  de la piste 1.
     * @param buf2 Buffer  de la piste 2.
     * @param buf3 Buffer  de la piste 3.
     * @param buf4 Buffer  de la piste 4.
     * @param frames Nombre de frames a traiter , par exemple 256 frames par buffer.
     */
    void process(float* output,
                 const float* buf1, const float* buf2,
                 const float* buf3, const float* buf4,
                 int frames);
};

#endif 