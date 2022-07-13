/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Jun 2022 1:55:53pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
 */
class DeckGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::Timer, public juce::FileDragAndDropTarget
{
public:
  DeckGUI(DJAudioPlayer *player, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse);

  ~DeckGUI() override;

  void paint(juce::Graphics &) override;

  void resized() override;

  // From Button::Listener
  void buttonClicked(juce::Button *) override;

  // From Slider::Listener
  void sliderValueChanged(juce::Slider *slider) override;

  // From FileDragAndDropTarget
  bool isInterestedInFileDrag(const juce::StringArray &files) override;

  void filesDropped(const juce::StringArray &files, int x, int y) override;

  // From Timer
  void timerCallback() override;

private:
  juce::TextButton playButton;
  juce::TextButton pauseButton;
  juce::TextButton stopButton;
  juce::TextButton loadButton;

  juce::Slider gainSlider;
  juce::Label gainLabel;

  juce::Slider speedSlider;
  juce::Label speedLabel;

  juce::Slider posSlider;
  juce::Label posLabel;

  juce::Slider bassSlider;
  juce::Label bassLabel;

  juce::Slider trebleSlider;
  juce::Label trebleLabel;

  WaveformDisplay waveformDisplay;

  DJAudioPlayer *djAudioPlayer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
