#include <JuceHeader.h>
#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel() = default;

MyLookAndFeel::MyLookAndFeel(juce::Colour colour) : colour(colour) {}

// Taken from https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
void MyLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                                     const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider) {
    // Determine the radius
    auto radius = (float) juce::jmin(width / 2, height / 2) - 4.0f;
    // Find the coordinates of the center point
    auto centreX = (float) x + (float) width * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    // Find top left position of the rotary slider
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    // Calculate the diameter
    auto rw = radius * 2.0f;
    // Calculate the angle of the pointer
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(colour);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::white);
    g.drawEllipse(rx, ry, rw, rw, 1.5f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(juce::Colours::white);
    g.fillPath(p);

    // Draw the name of the slider
    g.setColour(juce::Colours::white);
    g.drawText(slider.getName(), x, y, width, height, juce::Justification::centredTop, true);
}

// Taken from https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
void MyLookAndFeel::drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool, bool isButtonDown) {
    auto buttonArea = button.getLocalBounds();
    auto edge = 4;

    buttonArea.removeFromLeft(edge);
    buttonArea.removeFromTop(edge);

    // Draw the shadow first
    g.setColour(juce::Colours::darkgrey.withAlpha(0.5f));
    g.fillRect(buttonArea);

    // Translate the position
    auto offset = isButtonDown ? -edge / 2 : -edge;
    buttonArea.translate(offset, offset);

    g.setColour(backgroundColour);
    g.setColour(juce::Colour::fromRGB(49, 8, 123));
    g.fillRect(buttonArea);
}

// Taken from https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
void MyLookAndFeel::drawButtonText(juce::Graphics &g, juce::TextButton &button, bool, bool isButtonDown) {
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);
    g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
                                                          : juce::TextButton::textColourOffId)
                        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

    auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;

    auto edge = 4;
    auto offset = isButtonDown ? edge / 2 : 0;

    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
                         leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                         juce::Justification::centred, 2);
}

void MyLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                                     float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style,
                                     juce::Slider &slider) {

    if (style == juce::Slider::SliderStyle::LinearBarVertical) {
        g.fillAll(colour);
        g.fillRect(x, y, width, int(sliderPos));

        // Draw the name of the slider
        g.setColour(juce::Colours::white);
        g.drawText(slider.getName(), x, y, width, height, juce::Justification::centredTop, true);
    }
}
