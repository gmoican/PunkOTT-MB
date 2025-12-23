#pragma once

#include "PluginProcessor.h"
// #include "BinaryData.h"
#include "gui/CustomLookAndFeel.h"
#include "gui/LevelMeter.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PunkOTTProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PunkOTTProcessor& processorRef;
    
    // Custom Look and Feel
    CustomLookAndFeel myCustomLnF;
    
    // Layout utilities
    juce::TextButton header, sidebarLeft, sidebarRight, lifterContainer, compContainer, displayContainer, footer;
    
    // Sliders - Rotary knobs
    juce::Slider inputSlider, gateSlider, outputSlider;
    juce::Slider lifterRangeSlider, lifterAttackSlider, lifterReleaseSlider, lifterMixSlider;
    juce::Slider compThresSlider, compAttackSlider, compReleaseSlider, compMixSlider;
    
    juce::TextButton clipperButton { "Clip" };
    
    // Level meters - Conditional creation based on channel count
    std::unique_ptr<LevelMeter> inputLeftMeter;
    std::unique_ptr<LevelMeter> inputRightMeter;
    std::unique_ptr<LevelMeter> outputLeftMeter;
    std::unique_ptr<LevelMeter> outputRightMeter;
    
    // Version
    juce::Label versionTag;
    
    // Attachments for linking sliders-parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment, gateAttachment, mixAttachment, outputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lifterRangeAttachment, lifterAttackAttachment, lifterReleaseAttachment, lifterMixAttachment, compThresAttachment, compAttackAttachment, compReleaseAttachment, compMixAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> clipperAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
