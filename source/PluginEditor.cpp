#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTT_MB_Processor& p)
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
    addAndMakeVisible (header);
    
    footer.setColour (juce::TextButton::buttonColourId, UIColors::container.darker(0.5f)
                                                                           .withAlpha(0.25f)
                      );
    footer.setEnabled(false);
    addAndMakeVisible (footer);
    
    sidebarLeft.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    sidebarLeft.setEnabled(false);
    addAndMakeVisible (sidebarLeft);
    
    sidebarRight.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    sidebarRight.setEnabled(false);
    addAndMakeVisible (sidebarRight);
    
    lowLifterContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    lowLifterContainer.setEnabled(false);
    addAndMakeVisible (lowLifterContainer);
    
    midLifterContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    midLifterContainer.setEnabled(false);
    addAndMakeVisible (midLifterContainer);
    
    highLifterContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    highLifterContainer.setEnabled(false);
    addAndMakeVisible (highLifterContainer);
    
    lowCompContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    lowCompContainer.setEnabled(false);
    addAndMakeVisible (lowCompContainer);
    
    midCompContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    midCompContainer.setEnabled(false);
    addAndMakeVisible (midCompContainer);
    
    highCompContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    highCompContainer.setEnabled(false);
    addAndMakeVisible (highCompContainer);
    
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
    lowLifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowLifterRangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowLifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    lowLifterRangeSlider.setValue(Parameters::lifterThresDefault);
    lowLifterRangeSlider.setName("Lift");
    addAndMakeVisible(lowLifterRangeSlider);
    
    lowLifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowLifterThresId, lowLifterRangeSlider);
    
    midLifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midLifterRangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midLifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    midLifterRangeSlider.setValue(Parameters::lifterThresDefault);
    midLifterRangeSlider.setName("Lift");
    addAndMakeVisible(midLifterRangeSlider);
    
    midLifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midLifterThresId, midLifterRangeSlider);
    
    highLifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highLifterRangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highLifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    highLifterRangeSlider.setValue(Parameters::lifterThresDefault);
    highLifterRangeSlider.setName("Lift");
    addAndMakeVisible(highLifterRangeSlider);
    
    highLifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highLifterThresId, highLifterRangeSlider);

    // Lifter-attack knob
    lowLifterAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowLifterAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowLifterAttackSlider.setRange(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1);
    lowLifterAttackSlider.setValue(Parameters::lifterAttackDefault);
    lowLifterAttackSlider.setName("Att");
    addAndMakeVisible(lowLifterAttackSlider);
    
    lowLifterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowLifterAttackId, lowLifterAttackSlider);
    
    midLifterAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midLifterAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midLifterAttackSlider.setRange(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1);
    midLifterAttackSlider.setValue(Parameters::lifterAttackDefault);
    midLifterAttackSlider.setName("Att");
    addAndMakeVisible(midLifterAttackSlider);
    
    midLifterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midLifterAttackId, midLifterAttackSlider);
    
    highLifterAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highLifterAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highLifterAttackSlider.setRange(Parameters::lifterAttackMin, Parameters::lifterAttackMax, 0.1);
    highLifterAttackSlider.setValue(Parameters::lifterAttackDefault);
    highLifterAttackSlider.setName("Att");
    addAndMakeVisible(highLifterAttackSlider);
    
    highLifterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highLifterAttackId, highLifterAttackSlider);

    // Lifter-release knob
    lowLifterReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowLifterReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowLifterReleaseSlider.setRange(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1);
    lowLifterReleaseSlider.setValue(Parameters::lifterReleaseDefault);
    lowLifterReleaseSlider.setName("Rel");
    addAndMakeVisible(lowLifterReleaseSlider);
    
    lowLifterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowLifterReleaseId, lowLifterReleaseSlider);
    
    midLifterReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midLifterReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midLifterReleaseSlider.setRange(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1);
    midLifterReleaseSlider.setValue(Parameters::lifterReleaseDefault);
    midLifterReleaseSlider.setName("Rel");
    addAndMakeVisible(midLifterReleaseSlider);
    
    midLifterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midLifterReleaseId, midLifterReleaseSlider);
    
    highLifterReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highLifterReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highLifterReleaseSlider.setRange(Parameters::lifterReleaseMin, Parameters::lifterReleaseMax, 0.1);
    highLifterReleaseSlider.setValue(Parameters::lifterReleaseDefault);
    highLifterReleaseSlider.setName("Rel");
    addAndMakeVisible(highLifterReleaseSlider);
    
    highLifterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highLifterReleaseId, highLifterReleaseSlider);
    
    // Mix knob
    lowLifterMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowLifterMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowLifterMixSlider.setRange(Parameters::lifterMixMin, Parameters::lifterMixMax, 0.01);
    lowLifterMixSlider.setValue(Parameters::lifterMixDefault);
    lowLifterMixSlider.setName("Mix");
    addAndMakeVisible(lowLifterMixSlider);
    
    lowLifterMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowLifterMixId, lowLifterMixSlider);
    
    midLifterMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midLifterMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midLifterMixSlider.setRange(Parameters::lifterMixMin, Parameters::lifterMixMax, 0.01);
    midLifterMixSlider.setValue(Parameters::lifterMixDefault);
    midLifterMixSlider.setName("Mix");
    addAndMakeVisible(midLifterMixSlider);
    
    midLifterMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midLifterMixId, midLifterMixSlider);
    
    highLifterMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highLifterMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highLifterMixSlider.setRange(Parameters::lifterMixMin, Parameters::lifterMixMax, 0.01);
    highLifterMixSlider.setValue(Parameters::lifterMixDefault);
    highLifterMixSlider.setName("Mix");
    addAndMakeVisible(highLifterMixSlider);
    
    highLifterMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highLifterMixId, highLifterMixSlider);

    // --- COMPRESSOR PARAMETERS ---
    // Comp-thres knob
    lowCompThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowCompThresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowCompThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    lowCompThresSlider.setValue(Parameters::compThresDefault);
    lowCompThresSlider.setName("Comp");
    addAndMakeVisible(lowCompThresSlider);
    
    lowCompThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowCompThresId, lowCompThresSlider);
    
    midCompThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midCompThresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midCompThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    midCompThresSlider.setValue(Parameters::compThresDefault);
    midCompThresSlider.setName("Comp");
    addAndMakeVisible(midCompThresSlider);
    
    midCompThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midCompThresId, midCompThresSlider);
    
    highCompThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highCompThresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highCompThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    highCompThresSlider.setValue(Parameters::compThresDefault);
    highCompThresSlider.setName("Comp");
    addAndMakeVisible(highCompThresSlider);
    
    highCompThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highCompThresId, highCompThresSlider);
    
    // Comp-attack knob
    lowCompAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowCompAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowCompAttackSlider.setRange(Parameters::compAttackMin, Parameters::compAttackMax, 0.1);
    lowCompAttackSlider.setValue(Parameters::compAttackDefault);
    lowCompAttackSlider.setName("Att");
    addAndMakeVisible(lowCompAttackSlider);
    
    lowCompAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowCompAttackId, lowCompAttackSlider);
    
    midCompAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midCompAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midCompAttackSlider.setRange(Parameters::compAttackMin, Parameters::compAttackMax, 0.1);
    midCompAttackSlider.setValue(Parameters::compAttackDefault);
    midCompAttackSlider.setName("Att");
    addAndMakeVisible(midCompAttackSlider);
    
    midCompAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midCompAttackId, midCompAttackSlider);
    
    highCompAttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highCompAttackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highCompAttackSlider.setRange(Parameters::compAttackMin, Parameters::compAttackMax, 0.1);
    highCompAttackSlider.setValue(Parameters::compAttackDefault);
    highCompAttackSlider.setName("Att");
    addAndMakeVisible(highCompAttackSlider);
    
    highCompAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highCompAttackId, highCompAttackSlider);
    
    // Comp-release knob
    lowCompReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowCompReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowCompReleaseSlider.setRange(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1);
    lowCompReleaseSlider.setValue(Parameters::compReleaseDefault);
    lowCompReleaseSlider.setName("Rel");
    addAndMakeVisible(lowCompReleaseSlider);
    
    lowCompReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowCompReleaseId, lowCompReleaseSlider);
    
    midCompReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midCompReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midCompReleaseSlider.setRange(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1);
    midCompReleaseSlider.setValue(Parameters::compReleaseDefault);
    midCompReleaseSlider.setName("Rel");
    addAndMakeVisible(midCompReleaseSlider);
    
    midCompReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midCompReleaseId, midCompReleaseSlider);
    
    highCompReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highCompReleaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highCompReleaseSlider.setRange(Parameters::compReleaseMin, Parameters::compReleaseMax, 0.1);
    highCompReleaseSlider.setValue(Parameters::compReleaseDefault);
    highCompReleaseSlider.setName("Rel");
    addAndMakeVisible(highCompReleaseSlider);
    
    highCompReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highCompReleaseId, highCompReleaseSlider);
    
    // Mix knob
    lowCompMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowCompMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowCompMixSlider.setRange(Parameters::compMixMin, Parameters::compMixMax, 0.01);
    lowCompMixSlider.setValue(Parameters::compMixDefault);
    lowCompMixSlider.setName("Mix");
    addAndMakeVisible(lowCompMixSlider);
    
    lowCompMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowCompMixId, lowCompMixSlider);
    
    midCompMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midCompMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midCompMixSlider.setRange(Parameters::compMixMin, Parameters::compMixMax, 0.01);
    midCompMixSlider.setValue(Parameters::compMixDefault);
    midCompMixSlider.setName("Mix");
    addAndMakeVisible(midCompMixSlider);
    
    midCompMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midCompMixId, midCompMixSlider);
    
    highCompMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highCompMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highCompMixSlider.setRange(Parameters::compMixMin, Parameters::compMixMax, 0.01);
    highCompMixSlider.setValue(Parameters::compMixDefault);
    highCompMixSlider.setName("Mix");
    addAndMakeVisible(highCompMixSlider);
    
    highCompMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highCompMixId, highCompMixSlider);
    
    // --- CROSSOVER PARAMETERS ---
    // Level
    lowLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowLevelSlider.setRange(Parameters::levelMin, Parameters::levelMax, 0.1);
    lowLevelSlider.setValue(Parameters::levelDefault);
    lowLevelSlider.setName("Lows");
    addAndMakeVisible(lowLevelSlider);
    
    lowLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowLevelId, lowLevelSlider);
    
    midLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midLevelSlider.setRange(Parameters::levelMin, Parameters::levelMax, 0.1);
    midLevelSlider.setValue(Parameters::levelDefault);
    midLevelSlider.setName("Mids");
    addAndMakeVisible(midLevelSlider);
    
    midLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midLevelId, midLevelSlider);
    
    highLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highLevelSlider.setRange(Parameters::levelMin, Parameters::levelMax, 0.1);
    highLevelSlider.setValue(Parameters::levelDefault);
    highLevelSlider.setName("Highs");
    addAndMakeVisible(highLevelSlider);
    
    highLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::highLevelId, highLevelSlider);
    
    // Crossover frequencies
    lowmidCrossSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowmidCrossSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowmidCrossSlider.setRange(Parameters::lowmid_CrossMin, Parameters::lowmid_CrossMax, 1.0);
    lowmidCrossSlider.setValue(Parameters::lowmid_CrossDefault);
    lowmidCrossSlider.setName("Low X");
    addAndMakeVisible(lowmidCrossSlider);
    
    lowmidCrossAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lowmid_CrossId, lowmidCrossSlider);
    
    midhighCrossSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midhighCrossSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    midhighCrossSlider.setRange(Parameters::midhigh_CrossMin, Parameters::midhigh_CrossMax, 1.0);
    midhighCrossSlider.setValue(Parameters::midhigh_CrossDefault);
    midhighCrossSlider.setName("High X");
    addAndMakeVisible(midhighCrossSlider);
    
    midhighCrossAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::midhigh_CrossId, midhighCrossSlider);
    
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
    
    // --- CROSSOVER CONTROLS ---
    auto crossoverArea = area.removeFromBottom( 60 );
    crossoverContainer.setBounds(crossoverArea);
    
    auto crossSliderArea = crossoverArea.getWidth() / 5;
    lowLevelSlider.setBounds( crossoverArea.removeFromLeft(crossSliderArea).reduced( 5 ) );
    lowmidCrossSlider.setBounds( crossoverArea.removeFromLeft(crossSliderArea).reduced( 5 ) );
    midLevelSlider.setBounds( crossoverArea.removeFromLeft(crossSliderArea).reduced( 5 ) );
    midhighCrossSlider.setBounds( crossoverArea.removeFromLeft(crossSliderArea).reduced( 5 ) );
    highLevelSlider.setBounds( crossoverArea.reduced( 5 ) );
    
    // --- LIFTER AND COMP CONTROLS ---
    int gridWidth = area.getWidth();
    int gridHeight = area.getHeight();
    int colWidth = gridWidth / 3;
    int rowHeight = gridHeight / 2;
    
    lowLifterContainer.setBounds(area.withX( 0 )    // col * colWidth
                                     .withY( 0 )    // row * rowHeigh
                                     .withWidth( colWidth )
                                     .withHeight( rowHeight )
                                     .reduced( 5 )
                                 );
    midLifterContainer.setBounds(area.withX( colWidth )
                                     .withY( 0 )
                                     .withWidth( colWidth )
                                     .withHeight( rowHeight )
                                     .reduced( 5 )
                                 );
    highLifterContainer.setBounds(area.withX( 2 * colWidth )
                                      .withY( 0 )
                                      .withWidth( colWidth )
                                      .withHeight( rowHeight )
                                      .reduced( 5 )
                                  );
    
    lowCompContainer.setBounds(area.withX( 0 )
                                   .withY( rowHeight )
                                   .withWidth( colWidth )
                                   .withHeight( rowHeight )
                                   .reduced( 5 )
                               );
    midCompContainer.setBounds(area.withX( colWidth )
                                   .withY( rowHeight )
                                   .withWidth( colWidth )
                                   .withHeight( rowHeight )
                                   .reduced( 5 )
                               );
    highCompContainer.setBounds(area.withX( 2 * colWidth )
                                    .withY( rowHeight )
                                    .withWidth( colWidth )
                                    .withHeight( rowHeight )
                                    .reduced( 5 )
                                );
    
    
    // Position sliders inside lifter container
    auto lowLifterSliderArea = lowLifterContainer.getLocalBounds().reduced( 5 );
    auto midLifterSliderArea = midLifterContainer.getLocalBounds().reduced( 5 );
    auto highLifterSliderArea = highLifterContainer.getLocalBounds().reduced( 5 );
    auto lowCompSliderArea = lowCompContainer.getLocalBounds().reduced( 5 );
    auto midCompSliderArea = midCompContainer.getLocalBounds().reduced( 5 );
    auto highCompSliderArea = highCompContainer.getLocalBounds().reduced( 5 );
    
    // Split into top and bottom rows
    auto topRow = lowLifterSliderArea.removeFromTop(lowLifterSliderArea.getHeight() / 2);
    auto bottomRow = lowLifterSliderArea;

    // Split each row into left and right columns
    auto sliderWidth = topRow.getWidth() / 2;
    
    lowLifterRangeSlider.setBounds( topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lowLifterMixSlider.setBounds( topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lowLifterAttackSlider.setBounds( bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lowLifterReleaseSlider.setBounds( bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    
    // Iterate process to allocate other controllers
    topRow = midLifterSliderArea.removeFromTop(midLifterSliderArea.getHeight() / 2);
    bottomRow = midLifterSliderArea;
    
    midLifterRangeSlider.setBounds( topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    midLifterMixSlider.setBounds( topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    midLifterAttackSlider.setBounds( bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    midLifterReleaseSlider.setBounds( bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    
    // Iterate process to allocate other controllers
    topRow = highLifterSliderArea.removeFromTop(highLifterSliderArea.getHeight() / 2);
    bottomRow = highLifterSliderArea;
    
    highLifterRangeSlider.setBounds( topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    highLifterMixSlider.setBounds( topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    highLifterAttackSlider.setBounds( bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    highLifterReleaseSlider.setBounds( bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    
    // Iterate process to allocate other controllers
    topRow = lowCompSliderArea.removeFromTop(lowCompSliderArea.getHeight() / 2);
    bottomRow = lowCompSliderArea;
    
    lowCompThresSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lowCompMixSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lowCompAttackSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    lowCompReleaseSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    
    // Iterate process to allocate other controllers
    topRow = midCompSliderArea.removeFromTop(midCompSliderArea.getHeight() / 2);
    bottomRow = midCompSliderArea;
    
    midCompThresSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    midCompMixSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    midCompAttackSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    midCompReleaseSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    
    // Iterate process to allocate other controllers
    topRow = highCompSliderArea.removeFromTop(highCompSliderArea.getHeight() / 2);
    bottomRow = highCompSliderArea;
    
    highCompThresSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    highCompMixSlider.setBounds(topRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    highCompAttackSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
    highCompReleaseSlider.setBounds(bottomRow.removeFromLeft(sliderWidth).reduced( (int) sliderWidth * 0.05) );
}
