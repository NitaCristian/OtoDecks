/*
  ==============================================================================

    WaveformDisplay.h
    Created: 28 Jun 2022 4:53:51pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void loadURL(const juce::URL& audioURL);

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumbnail;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
