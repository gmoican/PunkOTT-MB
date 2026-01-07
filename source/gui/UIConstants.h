#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace UIColors
{
    // Define your 5-color palette
    const juce::Colour background    = juce::Colour(0xff0A0908);
    const juce::Colour primary       = juce::Colour(0xffEAE0D5);
    const juce::Colour secondary     = juce::Colour(0xff22333B);
    const juce::Colour text          = juce::Colour(0xffF7F7F7);
    const juce::Colour highlight     = juce::Colour(0xffFCA311);
}

namespace UIDimensions
{
    // You can also add sizing constants here
    const int knobSize = 80;
    const int margin = 20;
}
