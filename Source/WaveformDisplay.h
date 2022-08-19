#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"

//==============================================================================
/*
 */

// TODO - Refactor
// Add comments

class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse, DJAudioPlayer *djAudioPlayer);

    ~WaveformDisplay() override;

    //==============================================================================

    void paint(juce::Graphics &) override;

    void resized() override;

    //==============================================================================

    void loadURL(const juce::URL &audioURL);

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    void setPositionRelative(double pos);

    void mouseDown(const juce::MouseEvent &event);

    void mouseDrag(const juce::MouseEvent &event);

private:
    juce::AudioThumbnail audioThumbnail;
    bool fileLoaded;
    double position;
    DJAudioPlayer *djAudioPlayer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
