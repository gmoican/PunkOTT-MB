#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkOTTProcessor::PunkOTTProcessor()
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ), apvts(*this, nullptr, "Parameters", createParams())
{
}

PunkOTTProcessor::~PunkOTTProcessor()
{
}

//==============================================================================
const juce::String PunkOTTProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PunkOTTProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PunkOTTProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PunkOTTProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PunkOTTProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PunkOTTProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PunkOTTProcessor::getCurrentProgram()
{
    return 0;
}

void PunkOTTProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PunkOTTProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PunkOTTProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout PunkOTTProcessor::createParams()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // --- Utility Parameters Group ---
    auto utilsGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                           "utils",       // Group ID (must be unique string)
                                                                           "Utilities",   // Group Name (displayed in host)
                                                                           " | "          // Separator string for hosts that flatten the hierarchy
                                                                           );
    
    // Input Level
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::inId,
                                                                     Parameters::inName,
                                                                     juce::NormalisableRange<float>(Parameters::inMin, Parameters::inMax, 0.1f),
                                                                     Parameters::inDefault
                                                                     )
                         );
    
    // Input Noise Gate
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::gateId,
                                                                     Parameters::gateName,
                                                                     juce::NormalisableRange<float>(Parameters::gateMin, Parameters::gateMax, 0.1f),
                                                                     Parameters::gateDefault
                                                                     )
                         );
    
    // Clipper
    utilsGroup->addChild(std::make_unique<juce::AudioParameterBool>(Parameters::clipperId,
                                                                    Parameters::clipperName,
                                                                    Parameters::clipperDefault
                                                                    )
                         );
    
    // Output Level
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::outId,
                                                                     Parameters::outName,
                                                                     juce::NormalisableRange<float>(Parameters::outMin, Parameters::outMax, 0.1f),
                                                                     Parameters::outDefault
                                                                     )
                         );
    
    layout.add(std::move(utilsGroup));
    
    // --- OTT Parameters Group ---
    
    auto dynGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                         "dyn",         // Group ID (must be unique string)
                                                                         "Dynamics",    // Group Name (displayed in host)
                                                                         " | "          // Separator string for hosts that flatten the hierarchy
                                                                         );
    
    // Lifter Threshold (dB)
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::lifterThresId,
                                                                   Parameters::lifterThresName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1f),
                                                                   Parameters::lifterThresDefault
                                                                   )
                       
                       );
    
    // Lifter Attack Time
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::lifterAttackId,
                                                                   Parameters::lifterAttackName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1f),
                                                                   Parameters::lifterAttackDefault
                                                                   )
                       );
    
    // Lifter Release Time
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::lifterReleaseId,
                                                                   Parameters::lifterReleaseName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1f),
                                                                   Parameters::lifterReleaseDefault
                                                                   )
                       );
    
    // Lifter Mix (Wet/Dry)
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::lifterMixId,
                                                                     Parameters::lifterMixName,
                                                                     juce::NormalisableRange<float>(Parameters::lifterMixMin, Parameters::lifterMixMax, 1.0f),
                                                                     Parameters::lifterMixDefault
                                                                     )
                         );
    
    // Compressor Threshold (dB)
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::compThresId,
                                                                   Parameters::compThresName,
                                                                   juce::NormalisableRange<float>(Parameters::compThresMin, Parameters::compThresMax, 0.1f),
                                                                   Parameters::compThresDefault
                                                                   )
                       
                       );
    
    // Compressor Attack Time
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::compAttackId,
                                                                   Parameters::compAttackName,
                                                                   juce::NormalisableRange<float>(Parameters::compAttackMin, Parameters::compAttackMax, 0.1f),
                                                                   Parameters::compAttackDefault
                                                                   )
                       );
    
    // Compressor Release Time
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::compReleaseId,
                                                                   Parameters::compReleaseName,
                                                                   juce::NormalisableRange<float>(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1f),
                                                                   Parameters::compReleaseDefault
                                                                   )
                       );
    
    // Compressor Mix (Wet/Dry)
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::compMixId,
                                                                     Parameters::compMixName,
                                                                     juce::NormalisableRange<float>(Parameters::compMixMin, Parameters::compMixMax, 1.0f),
                                                                     Parameters::compMixDefault
                                                                     )
                         );
    
    layout.add(std::move(dynGroup));
    
    return layout;
}

//==============================================================================
void PunkOTTProcessor::updateParameters()
{
    // --- 1. Utilities
    const float inLeveldB = apvts.getRawParameterValue(Parameters::inId)->load();
    inGain = juce::Decibels::decibelsToGain(inLeveldB);
    
    const float gatedB = apvts.getRawParameterValue(Parameters::gateId)->load();
    gate.updateThres(gatedB);
    
    clipperState = (bool) apvts.getRawParameterValue(Parameters::clipperId)->load();
    
    const float outdB = apvts.getRawParameterValue(Parameters::outId)->load();
    outGain = juce::Decibels::decibelsToGain(outdB);
    
    // --- 2. OTT
    float sampleRate = (float) getSampleRate();
    
    // Lifter updates
    const float lifterMix = apvts.getRawParameterValue(Parameters::lifterMixId)->load();
    const float rangedB = apvts.getRawParameterValue(Parameters::lifterThresId)->load();
    const float lifterAttackMS = apvts.getRawParameterValue(Parameters::lifterAttackId)->load();
    const float lifterReleaseMS = apvts.getRawParameterValue(Parameters::lifterReleaseId)->load();
    float lifterCompensationGain = (rangedB > -40.0f) ? juce::jmap(rangedB, -40.0f, 0.0f, 0.0f, -8.0f) : 0.0f;
    lifterCompensationGain = juce::Decibels::decibelsToGain(lifterCompensationGain);
    
    lifter.updateMix(lifterMix);
    lifter.updateRange(rangedB);
    lifter.updateAttack(sampleRate, lifterAttackMS);
    lifter.updateRelease(sampleRate, lifterReleaseMS);
    outGain = outGain * (lifterCompensationGain * lifterMix / 100.f + (1.f - lifterMix / 100.f));
    
    // Compressor updates
    const float compMix = apvts.getRawParameterValue(Parameters::compMixId)->load();
    const float thresdB = apvts.getRawParameterValue(Parameters::compThresId)->load();
    const float compAttackMS = apvts.getRawParameterValue(Parameters::compAttackId)->load();
    const float compReleaseMS = apvts.getRawParameterValue(Parameters::compReleaseId)->load();
    
    compressor.updateMix(compMix);
    compressor.updateThres(thresdB);
    compressor.updateAttack(sampleRate, compAttackMS);
    compressor.updateRelease(sampleRate, compReleaseMS);
}

void PunkOTTProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::ignoreUnused(sampleRate);
    
    gate.prepare(sampleRate, samplesPerBlock);
    gate.updateAttack((float) sampleRate, 100.f);
    gate.updateRelease((float) sampleRate, 30.f);
    gate.updateMix(80.f);
    
    masterLimiter.prepare(sampleRate, samplesPerBlock);
    masterLimiter.updateThres(-3.f);
    masterLimiter.updateKnee(1.0f);
    masterLimiter.updateRatio(20.0f);
    masterLimiter.updateAttack((float) sampleRate, 30.0f);
    masterLimiter.updateRelease((float) sampleRate, 100.0f);
    
    lifter.prepare(sampleRate, samplesPerBlock);
    lifter.updateRatio(6.f);
    compressor.prepare(sampleRate, samplesPerBlock);
    compressor.updateRatio(8.f);
    
    clipper.setGainFactor(1.7f);
    
    updateParameters();
}

void PunkOTTProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PunkOTTProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}

void PunkOTTProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear any unused output channel
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Update params
    updateParameters();
    
    //GUI - Input level meters
    if (totalNumInputChannels > 0)
        levelMeters.rmsInputLeft.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples())));
    if (totalNumInputChannels > 1)
        levelMeters.rmsInputRight.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples())));
    
    // 1. UTILITIES - INPUT GAIN & GATE
    buffer.applyGain(inGain);
    gate.process(buffer);
    
    // 2. OTT - Lifter
    lifter.process(buffer, false);
    
    // 2. OTT - Comp
    compressor.process(buffer, false);
    
    // 2. OTT - Safe limiter
    masterLimiter.process(buffer, true);
    
    // 3. UTILITIES - Clipper
    if (clipperState) {
        clipper.applyTanhClipper(buffer);
    }
    
    // 3. UTILITIES - OUTPUT GAIN
    buffer.applyGain(outGain);
    
    // GUI - Output level meters
    if (totalNumOutputChannels > 0)
        levelMeters.rmsOutputLeft.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples())));
    if (totalNumOutputChannels > 1)
        levelMeters.rmsOutputRight.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples())));
}

//==============================================================================
bool PunkOTTProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PunkOTTProcessor::createEditor()
{
    return new PluginEditor (*this);
    // return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void PunkOTTProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PunkOTTProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PunkOTTProcessor();
}
