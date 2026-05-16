/**
 * @file Constants.h
 * @brief Project-wide constants.
 *
 * Centralizes every fixed value used in the application so that a single
 * place needs to be edited when tuning the drum machine.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/// Number of independent tracks in the drum machine.
constexpr int NUM_TRACKS = 4;

/// Number of steps (cells) per track in the sequencer grid.
constexpr int NUM_STEPS = 16;

/// Audio sample rate in Hz (number of stereo samples read per second).
constexpr int SAMPLE_RATE = 44100;

/// Number of frames produced per audio callback (smaller = lower latency).
constexpr int FRAMES_PER_BUFFER = 256;

/// Number of audio channels (2 = stereo: left + right).
constexpr int NUM_CHANNELS = 2;

/// Total number of interleaved float samples in one audio buffer
/// (256 frames * 2 channels = 512 samples).
constexpr int BUFFER_SIZE = FRAMES_PER_BUFFER * NUM_CHANNELS;

/// Capacity of the delay ring buffer in samples
/// (1 second of stereo audio at 44100 Hz).
constexpr int DELAY_BUFFER_SIZE = SAMPLE_RATE * NUM_CHANNELS;

/// Duration of one sequencer step expressed in beats
/// (0.25 = a quarter beat, so 4 beats = 16 steps).
constexpr double STEP_DURATION = 0.25;

#endif // CONSTANTS_H
