#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIConstants.h"

class LevelMeter : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>* levelPtr, const juce::String label) : levelPointer(levelPtr), meterLabel(label)
    {
        startTimerHz(30);
    }
    
    ~LevelMeter() override
    {
        stopTimer();
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        
        // Adjust level
        float min_dB = -40.0f;
        float max_dB = 0.0f;
        
        float clampedLevel = juce::jlimit(min_dB, max_dB, currentLevel);
        float normalizedLevel = juce::jmap(clampedLevel, min_dB, max_dB, 0.0f, 1.0f);
        juce::String levelLabel = (clampedLevel > min_dB) ? juce::String(currentLevel, 1) : "";
        
        // Draw background
        // g.setColour(UIColors::secondary.withAlpha(0.2f));
        // g.fillRect(bounds);
        
        // Draw label - dB Value
        g.setColour(UIColors::text);
        g.setFont(10.0f);
        auto labelArea = bounds.removeFromTop(25);
        // g.drawText(meterLabel, labelArea, juce::Justification::centred);
        g.drawText(levelLabel, labelArea, juce::Justification::centred);
        
        // Draw background bar
        auto meterBackArea = bounds.reduced(7);
        g.setColour(UIColors::secondary.withAlpha(0.5f));
        g.fillRoundedRectangle(meterBackArea.toFloat(), meterBackArea.getWidth());
        
        // Reserve top area for red indicator and check if signal is clipping
        auto redIndicatorHeight = 8;
        auto redIndicatorArea = meterBackArea.removeFromTop(redIndicatorHeight);
        bool isClipping = currentLevel > 0.0f;
        
        // Draw red indicator if clipping
        if (isClipping)
        {
            g.setColour(juce::Colours::red);
            g.fillRoundedRectangle(redIndicatorArea.toFloat(), redIndicatorArea.getWidth());
        }
        
        // Draw level bar
        int meterHeight = (int)(meterBackArea.getHeight() * normalizedLevel);
        auto meterRect = meterBackArea.removeFromBottom(meterHeight);
        g.setColour(UIColors::primary);
        // g.fillRect(meterRect);
        g.fillRoundedRectangle(meterRect.toFloat(), meterRect.getWidth());
    }
    
private:
    void timerCallback() override
    {
        if (levelPointer == nullptr)
            return;
        
        // Get new level from atomic pointer
        float newLevel = levelPointer->load();
        
        // Apply smoothing with ballistics
        if (newLevel > currentLevel)
        {
            // Fast attack for rising levels
            currentLevel = newLevel;
        }
        else
        {
            // Slow decay for falling levels
            float decayRate = 0.9f;
            currentLevel = currentLevel * decayRate + newLevel * (1.0f - decayRate);
        }
        
        repaint();
    }
    
    std::atomic<float>* levelPointer;
    juce::String meterLabel;
    float currentLevel = -100.f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
