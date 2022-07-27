/*
  ==============================================================================

    WaveformDisplay.h
    Created: 28 Jun 2022 4:53:51pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"

//==============================================================================
/*
 */
class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
  WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse, DJAudioPlayer *djAudioPlayer);
  ~WaveformDisplay() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  void loadURL(const juce::URL &audioURL);

  void changeListenerCallback(juce::ChangeBroadcaster *source) override;

  void setPositionRelative(double pos);

  void mouseDown(const juce::MouseEvent &event)
  {
    auto mouseX = event.x;
    auto ratio = mouseX * 1.0 / getWidth();
    djAudioPlayer->setPositionRelative(ratio);
    setPositionRelative(djAudioPlayer->getPositionRelative());
  }

  void mouseDrag(const juce::MouseEvent &event)
  {
    auto mouseX = event.x;
    auto ratio = mouseX * 1.0 / getWidth();
    djAudioPlayer->setPositionRelative(ratio);
    setPositionRelative(djAudioPlayer->getPositionRelative());
  }

private:
  juce::AudioThumbnail audioThumbnail;
  bool fileLoaded;
  double position;
  DJAudioPlayer *djAudioPlayer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
