#pragma once

#include <JuceHeader.h>

/**
 * MyLookAndFeel is a custom LookAndFeel class that overrides the appearance of some components
 */
class MyLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MyLookAndFeel();

    explicit MyLookAndFeel(juce::Colour primary);

    //==============================================================================
    // Taken from https://docs.juce.com/master/tutorial_look_and_feel_customisation.html

    // Method that overrides the design of a slider with a style of Rotary
    void
    drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &)
    override;

    // Method that overrides the background of a button
    void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool, bool isButtonDown) override;

    // Method that overrides the drawing of text of a button
    void drawButtonText(juce::Graphics &g, juce::TextButton &button, bool, bool isButtonDown) override;

    // Method that overrides the drawing of a linear slider
    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider &s) override;

private:
    // Primary colour of the LookAndFeel
    // It will make buttons and sliders blue, red, etc.
    juce::Colour primary;
};
