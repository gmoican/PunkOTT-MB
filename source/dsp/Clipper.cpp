#include "Clipper.h"

Clipper::Clipper()
{
}

// --- --- PARAMETER UPDATES --- --
void Clipper::setGainFactor(float newGain)
{
    gainFactor = newGain;
}

void Clipper::setParamFactor(float newParam)
{
    param = newParam;
}

// --- --- SAMPLE PROCESSING --- ---
float Clipper::applySoftClipper(float sample)
{
    return gainFactor * sample / (std::abs(sample) + 1.0f);
}

float Clipper::applyHardClipper(float sample)
{
    if (sample > 1.0f)
        return gainFactor * 2.0f / 3.0f;
    else if (sample < -1.0f)
        return gainFactor * -2.0f / 3.0f;
    else
        return gainFactor * (sample - std::pow(sample, 3.0f) / 3.0f);
}

float Clipper::applyTanhClipper(float sample)
{
    return gainFactor * 2.0f / juce::MathConstants<float>::pi * juce::dsp::FastMathApproximations::tanh(sample);
}

float Clipper::applyATanClipper(float sample)
{
    return gainFactor * 2.0f / juce::MathConstants<float>::pi * std::atan(sample);
}

// --- --- BUFFER PROCESSING --- ---
void Clipper::applySoftClipper(juce::AudioBuffer<float>& inputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = inputBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
            channelData[sample] = applySoftClipper(channelData[sample]);
    }
}

void Clipper::applyHardClipper(juce::AudioBuffer<float>& inputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = inputBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
            channelData[sample] = applyHardClipper(channelData[sample]);
    }
}

void Clipper::applyTanhClipper(juce::AudioBuffer<float>& inputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = inputBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
            channelData[sample] = applyTanhClipper(channelData[sample]);
    }
}

void Clipper::applyATanClipper(juce::AudioBuffer<float>& inputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = inputBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
            channelData[sample] = applyATanClipper(channelData[sample]);
    }
}
