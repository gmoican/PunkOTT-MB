#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colors for components
    setColour(juce::Slider::thumbColourId, UIColors::primary);
    setColour(juce::Slider::trackColourId, UIColors::secondary);
    setColour(juce::Slider::backgroundColourId, UIColors::background);

    setColour(juce::TextButton::buttonColourId, UIColors::secondary);
    setColour(juce::TextButton::buttonOnColourId, UIColors::primary);
    setColour(juce::TextButton::textColourOffId, UIColors::text);
    setColour(juce::TextButton::textColourOnId, UIColors::background);

    setColour(juce::ComboBox::backgroundColourId, UIColors::secondary);
    setColour(juce::ComboBox::textColourId, UIColors::text);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider)
{
    // juce::ignoreUnused(slider);
    
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // // Define the central area for text
    // auto textBounds = juce::Rectangle<int>(x, y, width, height).reduced(width/4, height/4).toFloat();

    // Draw background circle
    g.setColour(UIColors::secondary.withAlpha(0.3f));
    g.fillEllipse(rx, ry, rw, rw);

    // Draw track (background arc)
    g.setColour(UIColors::secondary);
    juce::Path trackArc;
    trackArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                           rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(trackArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Draw filled arc (value)
    g.setColour(UIColors::primary);
    juce::Path valueArc;
    valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                           rotaryStartAngle, angle, true);
    g.strokePath(valueArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // // Draw thumb (indicator)
    // juce::Path p;
    // auto pointerLength = radius * 0.5f;
    // auto pointerThickness = 2.0f;
    // p.addRectangle (-pointerThickness * 0.5f, -radius * 0.7f, pointerThickness, pointerLength);
    // p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    // g.setColour (UIColors::primary);
    // g.fillPath (p);
    
    // 5. Draw Dynamic Text Label/Value
    // Define the central area for text
    auto textBounds = juce::Rectangle<int>(x, y, width, height).reduced(width/4, height/4).toFloat();
    
    juce::String textToDisplay;
    float fontSize = juce::jmin(radius * 0.7f, 24.0f);
    
    // Check if the user is dragging or hovering
    if (slider.isMouseButtonDown() || slider.isMouseOver())
    {
        // Display the current value
        textToDisplay = slider.getTextFromValue(slider.getValue());
    }
    else
    {
        textToDisplay = slider.getName();
    }
    
    g.setColour(UIColors::text);
    g.setFont(juce::FontOptions(fontSize));
    
    // Draw the text in the center of the knob
    g.drawFittedText(textToDisplay,
                     textBounds.toNearestInt(),
                     juce::Justification::centred,
                     1);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Use the default JUCE drawing for linear sliders
    juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                             const juce::Colour& backgroundColour,
                                             bool shouldDrawButtonAsHighlighted,
                                             bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsDown);
        
    auto buttonArea = button.getLocalBounds().toFloat();
    g.setColour (shouldDrawButtonAsHighlighted ? UIColors::primary : backgroundColour);
    g.fillRoundedRectangle(buttonArea, 7.0f);
}

void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                       bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted);
    
    auto font = getTextButtonFont (button, button.getHeight());
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
                                                            : juce::TextButton::textColourOffId)
                       .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f)
                 );
    auto yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
    auto cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 2;
    auto fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;
    auto edge = 4;
    auto offset = shouldDrawButtonAsDown ? edge / 2 : 0;
    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
                          leftIndent + offset,
                          yIndent + offset,
                          textWidth,
                          button.getHeight() - yIndent * 2 - edge,
                          juce::Justification::centred,
                          2);
}
