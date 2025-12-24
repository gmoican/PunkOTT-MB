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
    
    // --- OTT Parameters Group --- CROSSOVERS ---
    auto crossovers = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                           "crossovers",   // Group ID (must be unique string)
                                                                           "Crossovers",   // Group Name (displayed in host)
                                                                           " | "           // Separator string for hosts that flatten the hierarchy
                                                                           );
    
    // Low Level (dB)
    crossovers->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::lowLevelId,
                                                                     Parameters::lowLevelName,
                                                                     juce::NormalisableRange<float>(Parameters::levelMin, Parameters::levelMax, 0.1f),
                                                                     Parameters::levelDefault
                                                                     )
                         );
    
    // Mid Level (dB)
    crossovers->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::midLevelId,
                                                                     Parameters::midLevelName,
                                                                     juce::NormalisableRange<float>(Parameters::levelMin, Parameters::levelMax, 0.1f),
                                                                     Parameters::levelDefault
                                                                     )
                         );
    
    // High Level (dB)
    crossovers->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::highLevelId,
                                                                     Parameters::highLevelName,
                                                                     juce::NormalisableRange<float>(Parameters::levelMin, Parameters::levelMax, 0.1f),
                                                                     Parameters::levelDefault
                                                                     )
                         );
    
    // Low Crossover (Hz)
    crossovers->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::lowCrossId,
                                                                     Parameters::lowCrossName,
                                                                     juce::NormalisableRange<float>(Parameters::lowCrossMin, Parameters::lowCrossMax, 1.f, 1.5f),
                                                                     Parameters::lowCrossDefault
                                                                     )
                         );
    
    // High Crossover (Hz)
    crossovers->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::highCrossId,
                                                                     Parameters::highCrossName,
                                                                     juce::NormalisableRange<float>(Parameters::highCrossMin, Parameters::highCrossMax, 1.f, 0.8f),
                                                                     Parameters::highCrossDefault
                                                                     )
                         );
    
    layout.add(std::move(crossovers));
    
    // --- OTT Parameters Group --- LOW BAND ---
    auto lowBand = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                        "lowBand",     // Group ID (must be unique string)
                                                                        "Low Band",    // Group Name (displayed in host)
                                                                        " | "          // Separator string for hosts that flatten the hierarchy
                                                                        );
    
    // Lifter Threshold (dB)
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowLifterThresId,
                                                                  Parameters::lowLifterThresName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1f),
                                                                  Parameters::lifterThresDefault
                                                                  )
                      );
    
    // Lifter Attack Time
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowLifterAttackId,
                                                                  Parameters::lowLifterAttackName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1f),
                                                                  Parameters::lifterAttackDefault
                                                                  )
                      );
    
    // Lifter Release Time
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowLifterReleaseId,
                                                                  Parameters::lowLifterReleaseName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1f),
                                                                  Parameters::lifterReleaseDefault
                                                                  )
                      );
    
    // Lifter Mix (Wet/Dry)
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowLifterMixId,
                                                                  Parameters::lowLifterMixName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterMixMin, Parameters::lifterMixMax, 1.0f),
                                                                  Parameters::lifterMixDefault
                                                                  )
                      );
    
    // Compressor Threshold (dB)
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowCompThresId,
                                                                  Parameters::lowCompThresName,
                                                                  juce::NormalisableRange<float>(Parameters::compThresMin, Parameters::compThresMax, 0.1f),
                                                                  Parameters::compThresDefault
                                                                  )
                      );
    
    // Compressor Attack Time
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowCompAttackId,
                                                                  Parameters::lowCompAttackName,
                                                                  juce::NormalisableRange<float>(Parameters::compAttackMin, Parameters::compAttackMax, 0.1f),
                                                                  Parameters::compAttackDefault
                                                                  )
                      );
    
    // Compressor Release Time
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowCompReleaseId,
                                                                  Parameters::lowCompReleaseName,
                                                                  juce::NormalisableRange<float>(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1f),
                                                                  Parameters::compReleaseDefault
                                                                  )
                      );
    
    // Compressor Mix (Wet/Dry)
    lowBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::lowCompMixId,
                                                                  Parameters::lowCompMixName,
                                                                  juce::NormalisableRange<float>(Parameters::compMixMin, Parameters::compMixMax, 1.0f),
                                                                  Parameters::compMixDefault
                                                                  )
                      );
    
    layout.add(std::move(lowBand));
    
    // --- OTT Parameters Group --- MID BAND ---
    auto midBand = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                        "midBand",     // Group ID (must be unique string)
                                                                        "Mid Band",    // Group Name (displayed in host)
                                                                        " | "          // Separator string for hosts that flatten the hierarchy
                                                                        );
    
    // Lifter Threshold (dB)
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midLifterThresId,
                                                                  Parameters::midLifterThresName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1f),
                                                                  Parameters::lifterThresDefault
                                                                  )
                      );
    
    // Lifter Attack Time
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midLifterAttackId,
                                                                  Parameters::midLifterAttackName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1f),
                                                                  Parameters::lifterAttackDefault
                                                                  )
                      );
    
    // Lifter Release Time
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midLifterReleaseId,
                                                                  Parameters::midLifterReleaseName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1f),
                                                                  Parameters::lifterReleaseDefault
                                                                  )
                      );
    
    // Lifter Mix (Wet/Dry)
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midLifterMixId,
                                                                  Parameters::midLifterMixName,
                                                                  juce::NormalisableRange<float>(Parameters::lifterMixMin, Parameters::lifterMixMax, 1.0f),
                                                                  Parameters::lifterMixDefault
                                                                  )
                      );
    
    // Compressor Threshold (dB)
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midCompThresId,
                                                                  Parameters::midCompThresName,
                                                                  juce::NormalisableRange<float>(Parameters::compThresMin, Parameters::compThresMax, 0.1f),
                                                                  Parameters::compThresDefault
                                                                  )
                      );
    
    // Compressor Attack Time
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midCompAttackId,
                                                                  Parameters::midCompAttackName,
                                                                  juce::NormalisableRange<float>(Parameters::compAttackMin, Parameters::compAttackMax, 0.1f),
                                                                  Parameters::compAttackDefault
                                                                  )
                      );
    
    // Compressor Release Time
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midCompReleaseId,
                                                                  Parameters::midCompReleaseName,
                                                                  juce::NormalisableRange<float>(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1f),
                                                                  Parameters::compReleaseDefault
                                                                  )
                      );
    
    // Compressor Mix (Wet/Dry)
    midBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                  Parameters::midCompMixId,
                                                                  Parameters::midCompMixName,
                                                                  juce::NormalisableRange<float>(Parameters::compMixMin, Parameters::compMixMax, 1.0f),
                                                                  Parameters::compMixDefault
                                                                  )
                      );
    
    layout.add(std::move(midBand));
    
    // --- OTT Parameters Group --- HIGH BAND ---
    auto highBand = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                         "highBand",     // Group ID (must be unique string)
                                                                         "High Band",    // Group Name (displayed in host)
                                                                         " | "           // Separator string for hosts that flatten the hierarchy
                                                                         );
    
    // Lifter Threshold (dB)
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highLifterThresId,
                                                                   Parameters::highLifterThresName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1f),
                                                                   Parameters::lifterThresDefault
                                                                   )
                       );
    
    // Lifter Attack Time
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highLifterAttackId,
                                                                   Parameters::highLifterAttackName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1f),
                                                                   Parameters::lifterAttackDefault
                                                                   )
                       );
    
    // Lifter Release Time
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highLifterReleaseId,
                                                                   Parameters::highLifterReleaseName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1f),
                                                                   Parameters::lifterReleaseDefault
                                                                   )
                       );
    
    // Lifter Mix (Wet/Dry)
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highLifterMixId,
                                                                   Parameters::highLifterMixName,
                                                                   juce::NormalisableRange<float>(Parameters::lifterMixMin, Parameters::lifterMixMax, 1.0f),
                                                                   Parameters::lifterMixDefault
                                                                   )
                       );
    
    // Compressor Threshold (dB)
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highCompThresId,
                                                                   Parameters::highCompThresName,
                                                                   juce::NormalisableRange<float>(Parameters::compThresMin, Parameters::compThresMax, 0.1f),
                                                                   Parameters::compThresDefault
                                                                   )
                       );
    
    // Compressor Attack Time
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highCompAttackId,
                                                                   Parameters::highCompAttackName,
                                                                   juce::NormalisableRange<float>(Parameters::compAttackMin, Parameters::compAttackMax, 0.1f),
                                                                   Parameters::compAttackDefault
                                                                   )
                       );
    
    // Compressor Release Time
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highCompReleaseId,
                                                                   Parameters::highCompReleaseName,
                                                                   juce::NormalisableRange<float>(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1f),
                                                                   Parameters::compReleaseDefault
                                                                   )
                       );
    
    // Compressor Mix (Wet/Dry)
    highBand->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::highCompMixId,
                                                                   Parameters::highCompMixName,
                                                                   juce::NormalisableRange<float>(Parameters::compMixMin, Parameters::compMixMax, 1.0f),
                                                                   Parameters::compMixDefault
                                                                   )
                       );
    
    layout.add(std::move(highBand));
    
    return layout;
}

//==============================================================================
void PunkOTTProcessor::updateParameters()
{
    // --- 1. Utilities
    // const float inLeveldB = apvts.getRawParameterValue(Parameters::inId)->load();
    inGain = juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::inId)->load() );
    
    // const float gatedB = apvts.getRawParameterValue(Parameters::gateId)->load();
    gate.updateThres( apvts.getRawParameterValue(Parameters::gateId)->load() );
    
    clipperState = (bool) apvts.getRawParameterValue(Parameters::clipperId)->load();
    
    // const float outdB = apvts.getRawParameterValue(Parameters::outId)->load();
    outGain = juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::outId)->load() );
    
    // --- 2.1. OTT - LOW BAND
    // Lifter updates
    lowLifter.updateMix( apvts.getRawParameterValue(Parameters::lowLifterMixId)->load() );
    lowLifter.updateRange( apvts.getRawParameterValue(Parameters::lowLifterThresId)->load() );
    lowLifter.updateAttack( apvts.getRawParameterValue(Parameters::lowLifterAttackId)->load() );
    lowLifter.updateRelease( apvts.getRawParameterValue(Parameters::lowLifterReleaseId)->load() );
    // Compressor updates
    lowComp.updateMix( apvts.getRawParameterValue(Parameters::lowCompMixId)->load() );
    lowComp.updateThres( apvts.getRawParameterValue(Parameters::lowCompThresId)->load() );
    lowComp.updateAttack( apvts.getRawParameterValue(Parameters::lowCompAttackId)->load() );
    lowComp.updateRelease( apvts.getRawParameterValue(Parameters::lowCompReleaseId)->load() );
    
    // --- 2.2. OTT - MID BAND
    // Lifter updates
    midLifter.updateMix( apvts.getRawParameterValue(Parameters::midLifterMixId)->load() );
    midLifter.updateRange( apvts.getRawParameterValue(Parameters::midLifterThresId)->load() );
    midLifter.updateAttack( apvts.getRawParameterValue(Parameters::midLifterAttackId)->load() );
    midLifter.updateRelease( apvts.getRawParameterValue(Parameters::midLifterReleaseId)->load() );
    // Compressor updates
    midComp.updateMix( apvts.getRawParameterValue(Parameters::midCompMixId)->load() );
    midComp.updateThres( apvts.getRawParameterValue(Parameters::midCompThresId)->load() );
    midComp.updateAttack( apvts.getRawParameterValue(Parameters::midCompAttackId)->load() );
    midComp.updateRelease( apvts.getRawParameterValue(Parameters::midCompReleaseId)->load() );
    
    // --- 2.3. OTT - HIGH BAND
    // Lifter updates
    highLifter.updateMix( apvts.getRawParameterValue(Parameters::highLifterMixId)->load() );
    highLifter.updateRange( apvts.getRawParameterValue(Parameters::highLifterThresId)->load() );
    highLifter.updateAttack( apvts.getRawParameterValue(Parameters::highLifterAttackId)->load() );
    highLifter.updateRelease( apvts.getRawParameterValue(Parameters::highLifterReleaseId)->load() );
    // Compressor updates
    highComp.updateMix( apvts.getRawParameterValue(Parameters::highCompMixId)->load() );
    highComp.updateThres( apvts.getRawParameterValue(Parameters::highCompThresId)->load() );
    highComp.updateAttack( apvts.getRawParameterValue(Parameters::highCompAttackId)->load() );
    highComp.updateRelease( apvts.getRawParameterValue(Parameters::highCompReleaseId)->load() );
}

void PunkOTTProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::ignoreUnused(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    gate.prepare(spec);
    gate.updateAttack( 100.f );
    gate.updateRelease( 30.f );
    gate.updateMix( 80.f );
    
    masterLimiter.prepare(spec);
    masterLimiter.updateThres( -3.f );
    masterLimiter.updateKnee( 1.0f );
    masterLimiter.updateRatio( 20.0f );
    masterLimiter.updateAttack( 30.0f );
    masterLimiter.updateRelease(  100.0f );
    
    lowLifter.prepare(spec);
    lowLifter.updateRatio(6.f);
    midLifter.prepare(spec);
    midLifter.updateRatio(6.f);
    highLifter.prepare(spec);
    highLifter.updateRatio(6.f);
    
    lowComp.prepare(spec);
    lowComp.updateRatio(8.f);
    midComp.prepare(spec);
    midComp.updateRatio(8.f);
    highComp.prepare(spec);
    highComp.updateRatio(8.f);
    
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
    lowLifter.process(buffer);
    
    // 2. OTT - Comp
    lowComp.process(buffer);
    
    // 2. OTT - Safe limiter
    masterLimiter.process(buffer);
    
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
    // return new PluginEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
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
