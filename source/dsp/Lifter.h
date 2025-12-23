#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Lifter
 * @brief Implements a standard, stable, feed-forward upward compressor.
 *
 * All core dynamics processing, including sidechain detection, gain computation,
 * and envelope smoothing, is contained here.
 */
class Lifter
{
public:
    Lifter();
    ~Lifter() = default;

    void prepare(double sampleRate, int totalNumChannels);
    
    float calculateTimeCoeff(float sampleRate, float time_ms);
    
    void updateRatio(float newRatio);
    void updateRange(float newRange);
    void updateKnee(float newKnee);
    void updateKneeRange();
    void updateAttack(float sampleRate, float newAttMs);
    void updateRelease(float sampleRate, float newRelMs);
    void updateMakeUp(float newMakeUp_dB);
    void updateMix(float newMix);
    
    float calculateTargetGain(float inputDB);
    float updateEnvelope(float targetGR_lin, float currentEnv_lin);
    
    float getGainAddition();
    
    /**
     * @brief Processes the audio buffer, applying the upward compression.
     *
     * @param inputBuffer This buffer is overwritten with the fully wet, compressed signal.
     * @param useFeedForward Select when the sidechain signal is measured
     */
    void process(juce::AudioBuffer<float>& inputBuffer, bool useFeedForward);

private:
    // --- Internal State ---
    std::vector<float> envelope; // Stores the current applied linear gain factor
    
    // Adjustable parameters
    float ratio = 4.0f;             // Linear ratio (e.g., 4.0 for 4:1)
    float rangedB = -40.0f;         // Threshold in dB
    float kneedB = 6.0f;            // Knee width in dB
    float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)
    float makeUpGain_linear = 1.0f; // Compensation gain after the compression takes place
    float mix = 1.0f;               // Mix (dry/wet)
    
    // Other params
    float compressionSlope = 1.0f - (1.0f / ratio);
    float kneeStart = rangedB - (kneedB / 2.0f);
    float kneeEnd = rangedB + (kneedB / 2.0f);
    float currentGA_linear = 1.0f;  // Gain reduction being applied currently
    
    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lifter)
};
