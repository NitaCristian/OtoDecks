

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "MyLookAndFeel.h"

//==============================================================================
/*
 */
class DeckGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::Timer, public juce::FileDragAndDropTarget
{
public:
  DeckGUI(DJAudioPlayer *player, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse, PlaylistComponent *playlistComponent);

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
  PlaylistComponent *playlist;

  juce::TextButton playButton;
  juce::TextButton pauseButton;
  juce::TextButton stopButton;
  juce::TextButton loadButton;

  juce::Slider gainSlider;
  juce::Label gainLabel;

  juce::Slider speedSlider;
  juce::Label speedLabel;

  juce::Slider bassSlider;
  juce::Label bassLabel;

  juce::Slider trebleSlider;
  juce::Label trebleLabel;

  DJAudioPlayer *djAudioPlayer;

  juce::Label trackName;
  WaveformDisplay waveformDisplay; // Take the djautioplayer

  juce::FileChooser fChooser{"Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.mp3"};
  MyLookAndFeel customLookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
