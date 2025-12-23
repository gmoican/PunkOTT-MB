#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace UIColors
{
    // Define your 6-color palette
    const juce::Colour background    = juce::Colour(0xff1B3C53);  // White background
    const juce::Colour container     = juce::Colour(0xff234C6A);  // Light Orange
    const juce::Colour primary       = juce::Colour(0xffDFD0B8);  // Strong Orange
    const juce::Colour secondary     = juce::Colour(0xff456882);  // Mid Orange
    const juce::Colour text          = juce::Colour(0xffE0D9D9);  // Strong Red
    const juce::Colour highlight     = juce::Colour(0xffD3DAD9);  // Mid Red
}

namespace UIDimensions
{
    // You can also add sizing constants here
    const int knobSize = 80;
    const int margin = 20;
}
