#pragma once

#include "PluginProcessor.h"
// #include "BinaryData.h"
#include "gui/CustomLookAndFeel.h"
#include "gui/LevelMeter.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PunkOTT_MB_Processor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PunkOTT_MB_Processor& processorRef;
    
    // Custom Look and Feel
    CustomLookAndFeel myCustomLnF;
    
    // Layout utilities
    juce::TextButton header, sidebarLeft, sidebarRight, lowLifterContainer, midLifterContainer, highLifterContainer, lowCompContainer, midCompContainer, highCompContainer, crossoverContainer, footer;
    
    // Sliders - Rotary knobs
    juce::Slider inputSlider, gateSlider, outputSlider;
    
    juce::Slider lowLifterRangeSlider, lowLifterAttackSlider, lowLifterReleaseSlider, lowLifterMixSlider;
    juce::Slider midLifterRangeSlider, midLifterAttackSlider, midLifterReleaseSlider, midLifterMixSlider;
    juce::Slider highLifterRangeSlider, highLifterAttackSlider, highLifterReleaseSlider, highLifterMixSlider;
    
    juce::Slider lowCompThresSlider, lowCompAttackSlider, lowCompReleaseSlider, lowCompMixSlider;
    juce::Slider midCompThresSlider, midCompAttackSlider, midCompReleaseSlider, midCompMixSlider;
    juce::Slider highCompThresSlider, highCompAttackSlider, highCompReleaseSlider, highCompMixSlider;
    
    juce::Slider lowLevelSlider, midLevelSlider, highLevelSlider, lowmidCrossSlider, midhighCrossSlider;
    
    juce::TextButton clipperButton { "Clip" };
    
    // Level meters - Conditional creation based on channel count
    std::unique_ptr<LevelMeter> inputLeftMeter;
    std::unique_ptr<LevelMeter> inputRightMeter;
    std::unique_ptr<LevelMeter> outputLeftMeter;
    std::unique_ptr<LevelMeter> outputRightMeter;
    
    // Version
    juce::Label versionTag;
    
    // Attachments for linking sliders-parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment, gateAttachment, outputAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowLifterRangeAttachment, lowLifterAttackAttachment, lowLifterReleaseAttachment, lowLifterMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midLifterRangeAttachment, midLifterAttackAttachment, midLifterReleaseAttachment, midLifterMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highLifterRangeAttachment, highLifterAttackAttachment, highLifterReleaseAttachment, highLifterMixAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCompThresAttachment, lowCompAttackAttachment, lowCompReleaseAttachment, lowCompMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midCompThresAttachment, midCompAttackAttachment, midCompReleaseAttachment, midCompMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCompThresAttachment, highCompAttackAttachment, highCompReleaseAttachment, highCompMixAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowLevelAttachment, midLevelAttachment, highLevelAttachment, lowmidCrossAttachment, midhighCrossAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> clipperAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
