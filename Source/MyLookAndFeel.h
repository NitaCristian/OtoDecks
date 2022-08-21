#pragma once

#include <JuceHeader.h>

/**
 * MyLookAndFeel is a custom LookAndFeel class that overrides the appearance of some components
 */
class MyLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MyLookAndFeel();

    //==============================================================================

    // Method that overrides the design of a slider with a style of Rotary
    void
    drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &)
    override;

    // Method that overrides the background of a button
    void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool, bool isButtonDown) override;

    // Method that overrides the drawing of text of a button
    void drawButtonText(juce::Graphics &g, juce::TextButton &button, bool, bool isButtonDown) override;

    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider &s) override;


};
