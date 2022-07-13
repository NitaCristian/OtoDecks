/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 28 Jun 2022 4:53:51pm
    Author:  cristi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse) : audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0.0)
{
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component

    g.setColour(juce::Colours::orange);
    if (!fileLoaded)
    {
        g.setFont(20.0f);
        g.drawText("File not loaded", getLocalBounds(),
                   juce::Justification::centred, true); // draw some placeholder text
        return;
    }

    audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1);

    g.setColour(juce::Colours::lightgreen);
    g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(const juce::URL &audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position && !std::isnan(pos))
    {
        position = pos;
        repaint();
    }
}
