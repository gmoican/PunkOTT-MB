#include "Gate.h"

// Define a small constant to prevent division by zero near the noise floor.
static constexpr float minMagnitude = 0.000001f; // Approx -120 dB

Gate::Gate()
{
}

void Gate::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);
    
    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 0.0f); // Initialize gain to 0 dB
}

float Gate::calculateTimeCoeff(float sampleRate, float time_ms)
{
    return std::exp(-1.0f / (sampleRate * (time_ms / 1000.0f)));
}

void Gate::updateRatio(float newRatio)
{
    ratio = newRatio;
}

void Gate::updateThres(float newThres)
{
    thresdB = newThres;
    updateKneeRange();
}

void Gate::updateKnee(float newKnee)
{
    kneedB = newKnee;
    updateKneeRange();
}

void Gate::updateKneeRange()
{
    kneeStart = thresdB - (kneedB / 2.0f);
    kneeEnd = thresdB + (kneedB / 2.0f);
}

void Gate::updateAttack(float sampleRate, float newAttMs)
{
    attackCoeff = calculateTimeCoeff(sampleRate, newAttMs);
}

void Gate::updateRelease(float sampleRate, float newRelMs)
{
    releaseCoeff = calculateTimeCoeff(sampleRate, newRelMs);
}

void Gate::updateMix(float newMix)
{
    mix = newMix / 100.0f;
}

// --- Core Math Logic ---

float Gate::calculateTargetGain(float input_dB)
{
    if (input_dB < kneeStart)
        return (input_dB - thresdB) * expansionSlope;
    if (input_dB < kneeEnd)
    {
        const float x = input_dB - kneeEnd;
        return expansionSlope / (2.0f * kneedB) * (x * x);
    }
    
    return 0.0f;
}

float Gate::updateEnvelope(float targetGR_dB, float currentEnv_dB)
{
    // If target reduction is greater than current (Attack), or less (Release)
    float alpha = (-targetGR_dB > -currentGR_dB) ? attackCoeff : releaseCoeff;
    
    // Exponential smoothing: y[n] = a * y[n-1] + (1-a) * x[n]
    float smoothedReductionAmount = (alpha * -currentGR_dB) + ((1.0f - alpha) * -targetGR_dB);
    return -smoothedReductionAmount;
}

// --- Methods for GUI ---
float Gate::getGainReduction()
{
    return currentGR_dB;
}

// --- PROCESS ---
void Gate::process(juce::AudioBuffer<float>& inputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();
    
    // Ensure envelope vector is correctly sized (safety)
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 1.0f);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Pointers for reading input and writing wet output
        float* channelData = inputBuffer.getWritePointer(channel);
        currentGR_dB = envelope[channel];

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = channelData[sample];
            float magnitude = std::abs (inputSample);
            
            // Clamp magnitude to prevent log(0)
            magnitude = std::max(magnitude, minMagnitude);
            
            // 1. SIDECHAIN: Convert magnitude to dB
            const float inputDB = juce::Decibels::gainToDecibels(magnitude);

            // 2. Gain Computer & Ballistics
            float targetGR_dB = calculateTargetGain(inputDB);
            currentGR_dB = updateEnvelope(targetGR_dB, currentGR_dB);
            
            // 4. APPLY GAIN (in-place)
            const float gainReductionLinear = juce::Decibels::decibelsToGain(currentGR_dB);
            float processed = inputSample * gainReductionLinear;
            channelData[sample] = (processed * mix) + (inputSample * (1.0f - mix));
        }
        
        // Store the final envelope value for the start of the next block
        envelope[channel] = currentGR_dB; 
    }
}
