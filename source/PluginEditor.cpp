#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTTProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    juce::ignoreUnused (processorRef);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- LAYOUT ---
    header.setColour (juce::TextButton::buttonColourId, UIColors::container.darker(0.5f)
                                                                           .withAlpha(0.25f)
                      );
    header.setEnabled(false);
    // header.setButtonText ("Header");
    addAndMakeVisible (header);
    
    footer.setColour (juce::TextButton::buttonColourId, UIColors::container.darker(0.5f)
                                                                           .withAlpha(0.25f)
                      );
    footer.setEnabled(false);
    // footer.setButtonText ("Footer");
    addAndMakeVisible (footer);
    
    sidebarLeft.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    sidebarLeft.setEnabled(false);
    // sidebarLeft.setButtonText ("Sidebar L");
    addAndMakeVisible (sidebarLeft);
    
    sidebarRight.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    sidebarRight.setEnabled(false);
    // sidebarRight.setButtonText ("Sidebar R");
    addAndMakeVisible (sidebarRight);
    
    lifterContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    lifterContainer.setEnabled(false);
    // lifterContainer.setButtonText("Lifter");
    addAndMakeVisible (lifterContainer);
    
    compContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    compContainer.setEnabled(false);
    // compContainer.setButtonText("Comp");
    addAndMakeVisible (compContainer);
    
    // --- UTILITIES PARAMETERS ---
    // Input knob
    inputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inputSlider.setRange(Parameters::inMin, Parameters::inMax, 0.1);
    inputSlider.setValue(Parameters::inDefault);
    inputSlider.setName("In");
    addAndMakeVisible(inputSlider);
    
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::inId, inputSlider);
    
    // Gate knob
    gateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gateSlider.setRange(Parameters::gateMin, Parameters::gateMax, 0.1);
    gateSlider.setValue(Parameters::gateDefault);
    gateSlider.setName(Parameters::gateName);
    addAndMakeVisible(gateSlider);
    
    gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::gateId, gateSlider);
    
    // Output knob
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputSlider.setRange(Parameters::outMin, Parameters::outMax, 0.1);
    outputSlider.setValue(Parameters::outDefault);
    outputSlider.setName("Out");
    addAndMakeVisible(outputSlider);
    
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::outId, outputSlider);

    // --- LIFTER PARAMETERS ---
    // Lifter-range knob
    lifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterRangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    lifterRangeSlider.setValue(Parameters::lifterThresDefault);
    lifterRangeSlider.setName("Lift");
    addAndMakeVisible(lifterRangeSlider);
    
    lifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterThresId, lifterRangeSlider);

    // Lifter-attack knob
    lifterAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterAttackSlider.setRange(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1);
    lifterAttackSlider.setValue(Parameters::lifterAttackDefault);
    lifterAttackSlider.setName("Att");
    addAndMakeVisible(lifterAttackSlider);
    
    lifterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterAttackId, lifterAttackSlider);

    // Lifter-release knob
    lifterReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterReleaseSlider.setRange(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1);
    lifterReleaseSlider.setValue(Parameters::lifterReleaseDefault);
    lifterReleaseSlider.setName("Rel");
    addAndMakeVisible(lifterReleaseSlider);
    
    lifterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterReleaseId, lifterReleaseSlider);
    
    // Mix knob
    lifterMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterMixSlider.setRange(Parameters::lifterMixMin, Parameters::lifterMixMax, 0.01);
    lifterMixSlider.setValue(Parameters::lifterMixDefault);
    lifterMixSlider.setName("Mix");
    addAndMakeVisible(lifterMixSlider);
    
    lifterMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterMixId, lifterMixSlider);

    // --- COMPRESSOR PARAMETERS ---
    // Comp-thres knob
    compThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compThresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    compThresSlider.setValue(Parameters::compThresDefault);
    compThresSlider.setName("Comp");
    addAndMakeVisible(compThresSlider);
    
    compThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compThresId, compThresSlider);
    
    // Comp-attack knob
    compAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compAttackSlider.setRange(Parameters::compAttackMin, Parameters::compAttackMax, 0.1);
    compAttackSlider.setValue(Parameters::compAttackDefault);
    compAttackSlider.setName("Att");
    addAndMakeVisible(compAttackSlider);
    
    compAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compAttackId, compAttackSlider);
    
    // Comp-release knob
    compReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compReleaseSlider.setRange(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1);
    compReleaseSlider.setValue(Parameters::compReleaseDefault);
    compReleaseSlider.setName("Rel");
    addAndMakeVisible(compReleaseSlider);
    
    compReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compReleaseId, compReleaseSlider);
    
    // Mix knob
    compMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compMixSlider.setRange(Parameters::compMixMin, Parameters::compMixMax, 0.01);
    compMixSlider.setValue(Parameters::compMixDefault);
    compMixSlider.setName("Mix");
    addAndMakeVisible(compMixSlider);
    
    compMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compMixId, compMixSlider);
    
    // --- CLIPPER ---
    // Clipper button
    clipperButton.setClickingTogglesState(true);
    addAndMakeVisible(clipperButton);
    
    clipperAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.apvts, Parameters::clipperId, clipperButton);
    
    // --- LEVEL METERS ---
    // Level Meters - Create meters based on channel count
    int numInputChannels = processorRef.getTotalNumInputChannels();
    int numOutputChannels = processorRef.getTotalNumOutputChannels();
    
    // Input meters
    inputLeftMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsInputLeft, "In L");
    addAndMakeVisible(*inputLeftMeter);
    
    if (numInputChannels > 1)
    {
        inputRightMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsInputRight, "In R");
        addAndMakeVisible(*inputRightMeter);
    }
    
    // Output meters
    outputLeftMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsOutputLeft, "Out L");
    addAndMakeVisible(*outputLeftMeter);
    
    if (numOutputChannels > 1)
    {
        outputRightMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsOutputRight, "Out R");
        addAndMakeVisible(*outputRightMeter);
    }
    
    // --- VERSION ---
    // Version tag
    versionTag.setText(juce::String ("") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " by @punkarra4",
                       juce::dontSendNotification);
    versionTag.setJustificationType(juce::Justification::centred);
    versionTag.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(versionTag);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(1.55);
    setResizeLimits(650, 420, 780, 600);
    setSize (650, 420);
}

PluginEditor::~PluginEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (UIColors::background);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    
    // --- LAYOUT SETUP ---
    auto headerArea = area.removeFromTop( 50 );
    auto sideLArea = area.removeFromLeft( 50 );
    auto sideRArea = area.removeFromRight( 50 );
    auto footerArea = area.removeFromBottom( 30 );
    
    header.setBounds(headerArea);
    footer.setBounds(footerArea);
    sidebarLeft.setBounds(sideLArea);
    sidebarRight.setBounds(sideRArea);
    
    // // --- LEVEL METERS ---
    // --- INPUT METERS ---
    int numInputChannels = processorRef.getTotalNumInputChannels();
    
    if (numInputChannels == 1)  // Mono = Only left channel meter
    {
        // Single input channel - meter takes full width
        if (inputLeftMeter)
        {
            inputLeftMeter->setVisible(true);
            inputLeftMeter->setBounds(sideLArea);
        }
        if (inputRightMeter)
            inputRightMeter->setVisible(false);
    }
    else if (numInputChannels > 1) // Stereo = Both channels meters
    {
        // Stereo input - split width in half
        auto leftHalf = sideLArea.removeFromLeft(sideLArea.getWidth() / 2);
        if (inputLeftMeter)
        {
            inputLeftMeter->setVisible(true);
            inputLeftMeter->setBounds(leftHalf);
        }
        if (inputRightMeter)
        {
            inputRightMeter->setVisible(true);
            inputRightMeter->setBounds(sideLArea);
        }
    }
    
    // --- OUTPUT METERS ---
    int numOutputChannels = processorRef.getTotalNumOutputChannels();
    
    if (numOutputChannels == 1)
    {
        // Single output channel - meter takes full height
        if (outputLeftMeter)
        {
            outputLeftMeter->setVisible(true);
            outputLeftMeter->setBounds(sideRArea);
        }
        if (outputRightMeter)
            outputRightMeter->setVisible(false);
    }
    else if (numOutputChannels > 1)
    {
        // Stereo output - split height in half
        auto leftHalf = sideRArea.removeFromLeft(sideRArea.getWidth() / 2);
        if (outputLeftMeter)
        {
            outputLeftMeter->setVisible(true);
            outputLeftMeter->setBounds(leftHalf);
        }
        if (outputRightMeter)
        {
            outputRightMeter->setVisible(true);
            outputRightMeter->setBounds(sideRArea);
        }
    }
    
    // --- FOOTER ---
    versionTag.setBounds(footerArea);
    
    // --- HEADER CONTROLS ---
    inputSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    
    gateSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    
    outputSlider.setBounds(headerArea.removeFromRight(headerArea.getHeight()));
    
    clipperButton.setBounds(headerArea.removeFromRight(headerArea.getHeight() + 20)
                                      .reduced(10, 15)
                            );
    
    // --- LIFTER AND COMP CONTROLS ---
    auto contentItemHeight = area.getHeight();
    // Reserve the top area for lifter and comp containers
    auto topArea = area.removeFromTop(contentItemHeight);
    
    // Split the top area in half horizontally
    auto lifterArea = topArea.removeFromLeft(topArea.getWidth() / 2).reduced(10);
    auto compArea = topArea.reduced(10);
    
    lifterContainer.setBounds (lifterArea);
    compContainer.setBounds (compArea);
    
    // Position sliders inside lifter container
    auto lifterSliderArea = lifterArea.reduced(10);
    auto compSliderArea = compArea.reduced(10);
    
    // Split into top and bottom rows
    auto topRow = lifterSliderArea.removeFromTop(lifterSliderArea.getHeight() / 2);
    auto bottomRow = lifterSliderArea;

    // Split each row into left and right columns
    auto sliderWidth = topRow.getWidth() / 2;
    
    lifterRangeSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lifterMixSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lifterAttackSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lifterReleaseSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    
    // Position sliders inside comp container
    // Split into top and bottom rows
    topRow = compSliderArea.removeFromTop(compSliderArea.getHeight() / 2);
    bottomRow = compSliderArea;
    
    compThresSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    compMixSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    compAttackSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    compReleaseSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
}
