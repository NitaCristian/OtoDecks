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
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse) :
        audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {
}

void WaveformDisplay::paint(juce::Graphics &g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::orange);
    if (!fileLoaded) {
        g.setFont(20.0f);
        g.drawText("File not loaded", getLocalBounds(),
                   juce::Justification::centred, true);   // draw some placeholder text
        return;
    }
    audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1);
}

void WaveformDisplay::resized() {
}

void WaveformDisplay::loadURL(const juce::URL &audioURL) {
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
    repaint();
}
