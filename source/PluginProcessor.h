#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "dsp/Gate.h"
#include "dsp/Lifter.h"
#include "dsp/Compressor.h"
#include "dsp/Clipper.h"

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
    constexpr auto clipperDefault = false;

    constexpr auto outId = "out_gain";
    constexpr auto outName = "Output Gain (dB)";
    constexpr auto outDefault = 0.f;
    constexpr auto outMin = -24.0f;
    constexpr auto outMax = 24.0f;

    // ====== COMPRESSOR PARAMETERS ===========
    // Comp: Determines the ceiling (in dB) for high-level signals to be compressed down
    constexpr auto compThresId = "comp";
    constexpr auto compThresName = "Compressor Threshold";
    constexpr auto compThresDefault = -12.0f;
    constexpr auto compThresMin = -24.0f;
    constexpr auto compThresMax = 0.0f;

    // Comp attack
    constexpr auto compAttackId = "compressorAttack";
    constexpr auto compAttackName = "Compressor Attack (ms)";
    constexpr auto compAttackDefault = 15.0f;
    constexpr auto compAttackMin = 0.1f;
    constexpr auto compAttackMax = 250.0f;

    // Comp release
    constexpr auto compReleaseId = "compressorRelease";
    constexpr auto compReleaseName = "Compressor Release (ms)";
    constexpr auto compReleaseDefault = 60.0f;
    constexpr auto compReleaseMin = 10.0f;
    constexpr auto compReleaseMax = 2500.0f;

    // Comp mix control
    constexpr auto compMixId = "compMix";
    constexpr auto compMixName = "Comp Mix";
    constexpr auto compMixDefault = 100.f;
    constexpr auto compMixMin = 0.0f;
    constexpr auto compMixMax = 100.0f;

    // ======= LIFTER PARAMETERS ===========
    // Lifter: Determines the ceiling (in dB) for low-level signals to be lifted up
    constexpr auto lifterThresId = "lifter";
    constexpr auto lifterThresName = "Lifter Range";
    constexpr auto lifterThresDefault = -40.0f;
    constexpr auto lifterThresMin = -80.0f;
    constexpr auto lifterThresMax = 0.0f;

    // Lifter attack
    constexpr auto lifterAttackId = "lifterAttack";
    constexpr auto lifterAttackName = "Lifter Attack (ms)";
    constexpr auto lifterAttackDefault = 50.0f;
    constexpr auto lifterAttackMin = 1.0f;
    constexpr auto lifterAttackMax = 500.0f;

    // Lifter release
    constexpr auto lifterReleaseId = "lifterRelease";
    constexpr auto lifterReleaseName = "Lifter Release (ms)";
    constexpr auto lifterReleaseDefault = 50.0f;
    constexpr auto lifterReleaseMin = 10.0f;
    constexpr auto lifterReleaseMax = 2500.0f;

    // Lifter mix control
    constexpr auto lifterMixId = "lifterMix";
    constexpr auto lifterMixName = "Lifter Mix";
    constexpr auto lifterMixDefault = 100.f;
    constexpr auto lifterMixMin = 0.0f;
    constexpr auto lifterMixMax = 100.0f;
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
    Gate gate;
    Clipper clipper;
    bool clipperState = false;
    
    // --- OTT PROCESSORS ---
    Lifter lifter;
    Compressor compressor, masterLimiter;
    
    // =============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkOTTProcessor)
};
