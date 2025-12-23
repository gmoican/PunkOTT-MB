#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Gate
 * @brief Implements a smooth downward expander (intended to be used as a Noise Gate).
 *
 * All core dynamics processing, including sidechain detection, gain computation,
 * and envelope smoothing, is contained here.
 */
class Gate
{
public:
    Gate();
    ~Gate() = default;

    void prepare(double sampleRate, int totalNumChannels);
    
    float calculateTimeCoeff(float sampleRate, float time_ms);

    void updateRatio(float newRatio);
    void updateThres(float newThres);
    void updateKnee(float newKnee);
    void updateKneeRange();
    void updateAttack(float sampleRate, float newAttMs);
    void updateRelease(float sampleRate, float newRelMs);
    void updateMix(float newMix);
    
    float calculateTargetGain(float input_dB);
    float updateEnvelope(float targetGR_dB, float currentEnv_dB);
    
    float getGainReduction();

    /**
     * @brief Processes the audio buffer in-place, applying a Downward Expander topology.
     *
     * @param inputBuffer The buffer containing the signal to be processed.
     */
    void process(juce::AudioBuffer<float>& inputBuffer);

private:
    // --- Internal State ---
    std::vector<float> envelope; // Stores the current applied linear gain factor
    
    // Adjustable parameters
    float ratio = 6.0f;             // Linear ratio
    float thresdB = -80.0f;         // Threshold in dB
    float kneedB = 9.0f;            // Knee width in dB
    float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)
    float mix = 1.0f;               // Mix (dry/wet)
    
    // Other params
    float expansionSlope = ratio - 1.0f;
    float kneeStart = thresdB - (kneedB / 2.0f);
    float kneeEnd = thresdB + (kneedB / 2.0f);
    float currentGR_dB = 0.0f;

    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gate)
};
