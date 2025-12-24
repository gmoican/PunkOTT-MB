#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "punk_dsp/punk_dsp.h"

#if (MSVC)
#include "ipps.h"
#endif

namespace Parameters
{
    // ======= UTILITY PARAMETERS ========
    constexpr auto inId = "in_gain";
    constexpr auto inName = "Input Gain (dB)";
    constexpr auto inDefault = 0.0f;
    constexpr auto inMin = -24.0f;
    constexpr auto inMax = 24.0f;

    constexpr auto gateId = "gate_thresh";
    constexpr auto gateName = "Gate";
    constexpr auto gateDefault = -80.f;
    constexpr auto gateMin = -90.0f;
    constexpr auto gateMax = 0.0f;

    // Clipper: Applies a soft-clipping function after the dynamics processing
    constexpr auto clipperId = "clipper";
    constexpr auto clipperName = "Clipper";
    constexpr auto clipperDefault = true;

    constexpr auto outId = "out_gain";
    constexpr auto outName = "Output Gain (dB)";
    constexpr auto outDefault = 0.f;
    constexpr auto outMin = -24.0f;
    constexpr auto outMax = 24.0f;

    // ====== COMPRESSOR PARAMETERS ===========
    // Comp: Determines the ceiling (in dB) for high-level signals to be compressed down
    constexpr auto lowCompThresId = "low_comp";
    constexpr auto lowCompThresName = "Low Compressor Threshold";
    constexpr auto midCompThresId = "mid_comp";
    constexpr auto midCompThresName = "Mid Compressor Threshold";
    constexpr auto highCompThresId = "high_comp";
    constexpr auto highCompThresName = "High Compressor Threshold";
    constexpr auto compThresDefault = -12.0f;
    constexpr auto compThresMin = -24.0f;
    constexpr auto compThresMax = 0.0f;

    // Comp attack
    constexpr auto lowCompAttackId = "low_compressorAttack";
    constexpr auto lowCompAttackName = "Low Compressor Attack (ms)";
    constexpr auto midCompAttackId = "mid_compressorAttack";
    constexpr auto midCompAttackName = "Mid Compressor Attack (ms)";
    constexpr auto highCompAttackId = "high_compressorAttack";
    constexpr auto highCompAttackName = "High Compressor Attack (ms)";
    constexpr auto compAttackDefault = 15.0f;
    constexpr auto compAttackMin = 0.1f;
    constexpr auto compAttackMax = 250.0f;

    // Comp release
    constexpr auto lowCompReleaseId = "low_compressorRelease";
    constexpr auto lowCompReleaseName = "Low Compressor Release (ms)";
    constexpr auto midCompReleaseId = "mid_compressorRelease";
    constexpr auto midCompReleaseName = "Mid Compressor Release (ms)";
    constexpr auto highCompReleaseId = "high_compressorRelease";
    constexpr auto highCompReleaseName = "High Compressor Release (ms)";
    constexpr auto compReleaseDefault = 60.0f;
    constexpr auto compReleaseMin = 10.0f;
    constexpr auto compReleaseMax = 2500.0f;

    // Comp mix control
    constexpr auto lowCompMixId = "low_compMix";
    constexpr auto lowCompMixName = "Low Comp Mix";
    constexpr auto midCompMixId = "mid_compMix";
    constexpr auto midCompMixName = "Mid Comp Mix";
    constexpr auto highCompMixId = "high_compMix";
    constexpr auto highCompMixName = "High Comp Mix";
    constexpr auto compMixDefault = 100.f;
    constexpr auto compMixMin = 0.0f;
    constexpr auto compMixMax = 100.0f;

    // ======= LIFTER PARAMETERS ===========
    // Lifter: Determines the ceiling (in dB) for low-level signals to be lifted up
    constexpr auto lowLifterThresId = "low_lifter";
    constexpr auto lowLifterThresName = "Low Lifter Range";
    constexpr auto midLifterThresId = "mid_lifter";
    constexpr auto midLifterThresName = "Mid Lifter Range";
    constexpr auto highLifterThresId = "high_lifter";
    constexpr auto highLifterThresName = "High Lifter Range";
    constexpr auto lifterThresDefault = -40.0f;
    constexpr auto lifterThresMin = -80.0f;
    constexpr auto lifterThresMax = 0.0f;

    // Lifter attack
    constexpr auto lowLifterAttackId = "low_lifterAttack";
    constexpr auto lowLifterAttackName = "Low Lifter Attack (ms)";
    constexpr auto midLifterAttackId = "mid_lifterAttack";
    constexpr auto midLifterAttackName = "Mid Lifter Attack (ms)";
    constexpr auto highLifterAttackId = "high_lifterAttack";
    constexpr auto highLifterAttackName = "High Lifter Attack (ms)";
    constexpr auto lifterAttackDefault = 50.0f;
    constexpr auto lifterAttackMin = 1.0f;
    constexpr auto lifterAttackMax = 500.0f;

    // Lifter release
    constexpr auto lowLifterReleaseId = "low_lifterRelease";
    constexpr auto lowLifterReleaseName = "Low Lifter Release (ms)";
    constexpr auto midLifterReleaseId = "mid_lifterRelease";
    constexpr auto midLifterReleaseName = "Mid Lifter Release (ms)";
    constexpr auto highLifterReleaseId = "high_lifterRelease";
    constexpr auto highLifterReleaseName = "High Lifter Release (ms)";
    constexpr auto lifterReleaseDefault = 50.0f;
    constexpr auto lifterReleaseMin = 10.0f;
    constexpr auto lifterReleaseMax = 2500.0f;

    // Lifter mix control
    constexpr auto lowLifterMixId = "low_lifterMix";
    constexpr auto lowLifterMixName = "Low Lifter Mix";
    constexpr auto midLifterMixId = "mid_lifterMix";
    constexpr auto midLifterMixName = "Mid Lifter Mix";
    constexpr auto highLifterMixId = "high_lifterMix";
    constexpr auto highLifterMixName = "High Lifter Mix";
    constexpr auto lifterMixDefault = 100.f;
    constexpr auto lifterMixMin = 0.0f;
    constexpr auto lifterMixMax = 100.0f;

    // ======= MULTIBAND PARAMETERS ===========
    // Level for each band
    constexpr auto lowLevelId = "low_level";
    constexpr auto lowLevelName = "Low Level (dB)";
    constexpr auto midLevelId = "mid_level";
    constexpr auto midLevelName = "Mid Level (dB)";
    constexpr auto highLevelId = "high_level";
    constexpr auto highLevelName = "High Level (dB)";
    constexpr auto levelDefault = 0.0f;
    constexpr auto levelMin = -24.0f;
    constexpr auto levelMax = 12.0f;

    // Crossover frequencies
    constexpr auto lowmid_CrossId = "low_cross";
    constexpr auto lowmid_CrossName = "Low Crossover (Hz)";
    constexpr auto lowmid_CrossDefault = 350.0f;
    constexpr auto lowmid_CrossMin = 80.0f;
    constexpr auto lowmid_CrossMax = 600.0f;
    
    constexpr auto midhigh_CrossId = "high_cross";
    constexpr auto midhigh_CrossName = "High Crossover (Hz)";
    constexpr auto midhigh_CrossDefault = 1000.0f;
    constexpr auto midhigh_CrossMin = 750.0f;
    constexpr auto midhigh_CrossMax = 10000.0f;
}

// GUI Level Meters
struct LevelMeters
{
    std::atomic<float> rmsInputLeft { -100.0f };
    std::atomic<float> rmsInputRight { -100.0f };
    std::atomic<float> rmsOutputLeft { -100.0f };
    std::atomic<float> rmsOutputRight { -100.0f };
};

class PunkOTTProcessor : public juce::AudioProcessor
{
public:
    PunkOTTProcessor();
    ~PunkOTTProcessor() override;

    // DEFAULT STUFF ===============================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // ===== MY STUFF ===============================================================
    juce::AudioProcessorValueTreeState apvts;
    
    // GUI Level Meters
    LevelMeters levelMeters;
    
    void updateParameters();

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    // --- INTERNAL PARAMETER HANDLING ---
    // Utilities
    float inGain = 1.0f;
    float outGain = 1.0f;
    punk_dsp::Gate gate;
    punk_dsp::Waveshaper clipper;
    bool clipperState = true;
    
    // --- OTT PROCESSORS ---
    punk_dsp::Lifter lowLifter, midLifter, highLifter;
    punk_dsp::Compressor lowComp, midComp, highComp;
    punk_dsp::Compressor masterLimiter;
    
    // --- CROSSOVER ---
    float lowLevel = 1.0f;
    float midLevel = 1.0f;
    float highLevel = 1.0f;
    
    using Filter = juce::dsp::LinkwitzRileyFilter<float>;
        
    Filter lowPassFilter1, lowPassFilter2;      // For low band
    Filter highPassFilter1, highPassFilter2;    // For high band
    Filter allPassFilter;                       // For phase compensation
    
    juce::AudioBuffer<float> lowBand, midBand, highBand;
    
    float lowMidCrossover = Parameters::lowmid_CrossDefault;    // Hz
    float midHighCrossover = Parameters::midhigh_CrossDefault;  // Hz
    
    // =============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkOTTProcessor)
};
