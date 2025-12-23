#include "Compressor.h"

// Define a floor for magnitude detection to prevent log(0) and instability.
static constexpr float minMagnitude = 0.00001f;     // Approx -100 dB

Compressor::Compressor()
{
}

void Compressor::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);

    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 0.0f); // Initialize gain to 0 dB
}

// --- Parameter Updates ---

float Compressor::calculateTimeCoeff(float sampleRate, float time_ms)
{
    // 1-pole filter coefficient calculation (alpha = e^(-1 / (sampleRate * time_in_seconds)))
    // We use -1.0f/tau as the exponent for the exponential smoothing factor.
    return std::exp(-1.0f / (sampleRate * (time_ms / 1000.0f)));
}

void Compressor::updateRatio(float newRatio)
{
    ratio = newRatio;
    compressionSlope = 1.0f - (1.0f / newRatio);
}

void Compressor::updateThres(float newThres)
{
    thresdB = newThres;
    updateKneeRange();
}

void Compressor::updateKnee(float newKnee)
{
    kneedB = newKnee;
    updateKneeRange();
}

void Compressor::updateKneeRange()
{
    kneeStart = thresdB - (kneedB / 2.0f);
    kneeEnd = thresdB + (kneedB / 2.0f);
}

void Compressor::updateAttack(float sampleRate, float newAttMs)
{
    attackCoeff = calculateTimeCoeff(sampleRate, newAttMs);
}

void Compressor::updateRelease(float sampleRate, float newRelMs)
{
    releaseCoeff = calculateTimeCoeff(sampleRate, newRelMs);
}

void Compressor::updateMakeUp(float newMakeUp_dB)
{
    makeUpGaindB = newMakeUp_dB;
}

void Compressor::updateMix(float newMix)
{
    mix = newMix / 100.0f;
}

// --- Core Math Logic ---

float Compressor::calculateTargetGain(float inputDB)
{
    if (inputDB > kneeEnd)
        return (inputDB - thresdB) * compressionSlope;
    
    if (inputDB > kneeStart)
    {
        const float x = inputDB - kneeStart;
        return (compressionSlope / (2.0f * kneedB)) * (x * x);
    }

    return 0.0f;
}

float Compressor::updateEnvelope(float targetGR_dB, float currentEnv_dB)
{
    // If target reduction is greater than current (Attack), or less (Release)
    float alpha = (targetGR_dB > -currentEnv_dB) ? attackCoeff : releaseCoeff;
    
    // Exponential smoothing: y[n] = a * y[n-1] + (1-a) * x[n]
    float smoothedReduction = (alpha * -currentEnv_dB) + ((1.0f - alpha) * targetGR_dB);
    return -smoothedReduction;
}

// --- Methods for GUI ---
float Compressor::getGainReduction()
{
    return currentGR_dB;
}

void Compressor::process(juce::AudioBuffer<float>& inputBuffer, bool useFeedForward)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();
    
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 0.0f);
    
    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* channelData = inputBuffer.getWritePointer(ch);
        currentGR_dB = envelope[ch];
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float input = channelData[sample];
            
            // 1. Identify Sidechain Input based on Topology
            float sidechainInput = input;
            
            if (!useFeedForward)
            {
                // Feed-back uses the PREVIOUS output (estimated by current envelope)
                float prevGain = juce::Decibels::decibelsToGain(currentGR_dB + makeUpGaindB);
                sidechainInput = input * prevGain;
            }
            
            // 2. Detection (Sidechain)
            float mag = std::max(std::abs(sidechainInput), minMagnitude);
            float inputDB = juce::Decibels::gainToDecibels(mag);
            
            // 3. Gain Computer & Ballistics
            float targetGR = calculateTargetGain(inputDB);
            currentGR_dB = updateEnvelope(targetGR, currentGR_dB);
            
            // 4. Apply Gain
            float totalGainDB = currentGR_dB + makeUpGaindB;
            float gainLinear = juce::Decibels::decibelsToGain(totalGainDB);
            
            float processed = input * gainLinear;
            channelData[sample] = (processed * mix) + (input * (1.0f - mix));
        }
        
        envelope[ch] = currentGR_dB;
    }
}
