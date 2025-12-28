#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkOTT_MB_Processor::PunkOTT_MB_Processor()
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

PunkOTT_MB_Processor::~PunkOTT_MB_Processor()
{
}

//==============================================================================
const juce::String PunkOTT_MB_Processor::getName() const
{
    return JucePlugin_Name;
}

bool PunkOTT_MB_Processor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PunkOTT_MB_Processor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PunkOTT_MB_Processor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PunkOTT_MB_Processor::getTailLengthSeconds() const
{
    return 0.0;
}

int PunkOTT_MB_Processor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PunkOTT_MB_Processor::getCurrentProgram()
{
    return 0;
}

void PunkOTT_MB_Processor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PunkOTT_MB_Processor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PunkOTT_MB_Processor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout PunkOTT_MB_Processor::createParams()
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
                                                                     Parameters::lowmid_CrossId,
                                                                     Parameters::lowmid_CrossName,
                                                                     juce::NormalisableRange<float>(Parameters::lowmid_CrossMin, Parameters::lowmid_CrossMax, 1.f, 1.5f),
                                                                     Parameters::lowmid_CrossDefault
                                                                     )
                         );
    
    // High Crossover (Hz)
    crossovers->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::midhigh_CrossId,
                                                                     Parameters::midhigh_CrossName,
                                                                     juce::NormalisableRange<float>(Parameters::midhigh_CrossMin, Parameters::midhigh_CrossMax, 1.f, 0.8f),
                                                                     Parameters::midhigh_CrossDefault
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
void PunkOTT_MB_Processor::updateParameters()
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
    float lowLiftMix = apvts.getRawParameterValue(Parameters::lowLifterMixId)->load();
    float lowLiftRange = apvts.getRawParameterValue(Parameters::lowLifterThresId)->load();
    lowLifter.updateMix( lowLiftMix );
    lowLifter.updateRange( lowLiftRange );
    lowLifter.updateAttack( apvts.getRawParameterValue(Parameters::lowLifterAttackId)->load() );
    lowLifter.updateRelease( apvts.getRawParameterValue(Parameters::lowLifterReleaseId)->load() );
    // Level
    float lowLifterCompensationGain = (lowLiftRange > -40.0f) ? juce::jmap(lowLiftRange, -40.0f, 0.0f, 0.0f, -8.0f) : 0.0f;
    lowLifterCompensationGain = juce::Decibels::decibelsToGain(lowLifterCompensationGain);
    float newLowLevel = juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::lowLevelId)->load() );
    lowLevel = newLowLevel * ( lowLifterCompensationGain * lowLiftMix / 100.f + (1.f - lowLiftMix / 100.f) );
    // Compressor updates
    lowComp.updateMix( apvts.getRawParameterValue(Parameters::lowCompMixId)->load() );
    lowComp.updateThres( apvts.getRawParameterValue(Parameters::lowCompThresId)->load() );
    lowComp.updateAttack( apvts.getRawParameterValue(Parameters::lowCompAttackId)->load() );
    lowComp.updateRelease( apvts.getRawParameterValue(Parameters::lowCompReleaseId)->load() );
    
    // --- 2.2. OTT - MID BAND
    // Lifter updates
    float midLiftMix = apvts.getRawParameterValue(Parameters::midLifterMixId)->load();
    float midLiftRange = apvts.getRawParameterValue(Parameters::midLifterThresId)->load();
    midLifter.updateMix( midLiftMix );
    midLifter.updateRange( midLiftRange );
    midLifter.updateAttack( apvts.getRawParameterValue(Parameters::midLifterAttackId)->load() );
    midLifter.updateRelease( apvts.getRawParameterValue(Parameters::midLifterReleaseId)->load() );
    // Level
    float midLifterCompensationGain = (midLiftRange > -40.0f) ? juce::jmap(midLiftRange, -40.0f, 0.0f, 0.0f, -8.0f) : 0.0f;
    midLifterCompensationGain = juce::Decibels::decibelsToGain(midLifterCompensationGain);
    float newMidLevel = juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::midLevelId)->load() );
    midLevel = newMidLevel * ( midLifterCompensationGain * midLiftMix / 100.f + (1.f - midLiftMix / 100.f) );
    // Compressor updates
    midComp.updateMix( apvts.getRawParameterValue(Parameters::midCompMixId)->load() );
    midComp.updateThres( apvts.getRawParameterValue(Parameters::midCompThresId)->load() );
    midComp.updateAttack( apvts.getRawParameterValue(Parameters::midCompAttackId)->load() );
    midComp.updateRelease( apvts.getRawParameterValue(Parameters::midCompReleaseId)->load() );
    
    // --- 2.3. OTT - HIGH BAND
    // Lifter updates
    float highLiftMix = apvts.getRawParameterValue(Parameters::highLifterMixId)->load();
    float highLiftRange = apvts.getRawParameterValue(Parameters::highLifterThresId)->load();
    highLifter.updateMix( highLiftMix );
    highLifter.updateRange( highLiftRange );
    highLifter.updateAttack( apvts.getRawParameterValue(Parameters::highLifterAttackId)->load() );
    highLifter.updateRelease( apvts.getRawParameterValue(Parameters::highLifterReleaseId)->load() );
    // Level
    float highLifterCompensationGain = (highLiftRange > -40.0f) ? juce::jmap(highLiftRange, -40.0f, 0.0f, 0.0f, -8.0f) : 0.0f;
    highLifterCompensationGain = juce::Decibels::decibelsToGain(highLifterCompensationGain);
    float newHighLevel = juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::highLevelId)->load() );
    highLevel = newHighLevel * ( highLifterCompensationGain * highLiftMix / 100.f + (1.f - highLiftMix / 100.f) );
    // Compressor updates
    highComp.updateMix( apvts.getRawParameterValue(Parameters::highCompMixId)->load() );
    highComp.updateThres( apvts.getRawParameterValue(Parameters::highCompThresId)->load() );
    highComp.updateAttack( apvts.getRawParameterValue(Parameters::highCompAttackId)->load() );
    highComp.updateRelease( apvts.getRawParameterValue(Parameters::highCompReleaseId)->load() );
    
    // --- 2.4. OTT - CROSSOVER FILTERS
    lowPassFilter1.setCutoffFrequency( apvts.getRawParameterValue(Parameters::lowmid_CrossId)->load() );
    lowPassFilter2.setCutoffFrequency( apvts.getRawParameterValue(Parameters::midhigh_CrossId)->load() );
    highPassFilter1.setCutoffFrequency( apvts.getRawParameterValue(Parameters::lowmid_CrossId)->load() );
    highPassFilter2.setCutoffFrequency( apvts.getRawParameterValue(Parameters::midhigh_CrossId)->load() );
    allPassFilter.setCutoffFrequency( apvts.getRawParameterValue(Parameters::lowmid_CrossId)->load() );
}

void PunkOTT_MB_Processor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::ignoreUnused(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    gate.prepare(spec);
    gate.updateAttack( 100.f );
    gate.updateRelease( 30.f );
    gate.updateMix( 90.f );
    
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
    
    // Prepare all filters
    lowPassFilter1.prepare(spec);
    lowPassFilter2.prepare(spec);
    highPassFilter1.prepare(spec);
    highPassFilter2.prepare(spec);
    allPassFilter.prepare(spec);
    
    // Set filter types and frequencies
    lowPassFilter1.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowPassFilter2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    highPassFilter1.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highPassFilter2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    allPassFilter.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    
    // Allocate band buffers
    lowBand.setSize(spec.numChannels, samplesPerBlock);
    midBand.setSize(spec.numChannels, samplesPerBlock);
    highBand.setSize(spec.numChannels, samplesPerBlock);
    
    clipper.setOutGain(1.7f);
    
    updateParameters();
}

void PunkOTT_MB_Processor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PunkOTT_MB_Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PunkOTT_MB_Processor::processBlock (juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    // Clear any unused output channel
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
    // Update params
    updateParameters();
    
    //GUI - Input level meters
    if (totalNumInputChannels > 0)
        levelMeters.rmsInputLeft.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples)));
    if (totalNumInputChannels > 1)
        levelMeters.rmsInputRight.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples)));
    
    // 1. UTILITIES - INPUT GAIN & GATE
    buffer.applyGain(inGain);
    gate.process(buffer);
    
    // 2. OTT - BAND SPLITTING
    // Copy input to band buffers
    for (int ch = 0; ch < numChannels; ++ch)
    {
        lowBand.copyFrom(ch, 0, buffer, ch, 0, numSamples);
        midBand.copyFrom(ch, 0, buffer, ch, 0, numSamples);
        highBand.copyFrom(ch, 0, buffer, ch, 0, numSamples);
    }
    
    // Create audio blocks for DSP processing
    juce::dsp::AudioBlock<float> lowBlock(lowBand);
    juce::dsp::AudioBlock<float> midBlock(midBand);
    juce::dsp::AudioBlock<float> highBlock(highBand);
    
    juce::dsp::ProcessContextReplacing<float> lowContext(lowBlock);
    juce::dsp::ProcessContextReplacing<float> midContext(midBlock);
    juce::dsp::ProcessContextReplacing<float> highContext(highBlock);
    
    // Process LOW band: LP @ lowMidCrossover
    lowPassFilter1.process(lowContext);
    
    // Process HIGH band: HP @ midHighCrossover
    highPassFilter2.process(highContext);
    
    // Process MID band: HP @ lowMidCrossover, then LP @ midHighCrossover
    highPassFilter1.process(midContext);
    lowPassFilter2.process(midContext);
    
    /* Phase compensation for low band (in theory)
     * Midrange destroyer (in practice)
     */
    // allPassFilter.process(lowContext);
    
    // 3. OTT - MULTIBAND PROCESSING
    lowLifter.process(lowBand);
    lowComp.process(lowBand);
    lowBand.applyGain(lowLevel);
    
    midLifter.process(midBand);
    midComp.process(midBand);
    midBand.applyGain(midLevel);
    
    highLifter.process(highBand);
    highComp.process(highBand);
    highBand.applyGain(highLevel);
    
    // 4. MIX BANDS BACK TOGETHER
    buffer.clear();
    for (int ch = 0; ch < numChannels; ++ch)
    {
        buffer.addFrom(ch, 0, lowBand, ch, 0, numSamples);
        buffer.addFrom(ch, 0, midBand, ch, 0, numSamples);
        buffer.addFrom(ch, 0, highBand, ch, 0, numSamples);
    }
    
    // 5. OTT - SAFE LIMITER
    masterLimiter.process(buffer);
    
    // 6. UTILITIES - Clipper
    if (clipperState) {
        clipper.applyTanhClipper(buffer);
    }
    
    // 7. UTILITIES - OUTPUT GAIN
    buffer.applyGain(outGain);
    
    // GUI - Output level meters
    if (totalNumOutputChannels > 0)
        levelMeters.rmsOutputLeft.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples)));
    if (totalNumOutputChannels > 1)
        levelMeters.rmsOutputRight.store(juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples)));
}

//==============================================================================
bool PunkOTT_MB_Processor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PunkOTT_MB_Processor::createEditor()
{
    return new PluginEditor (*this);
    // return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void PunkOTT_MB_Processor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PunkOTT_MB_Processor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PunkOTT_MB_Processor();
}
