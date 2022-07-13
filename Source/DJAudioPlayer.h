/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jun 2022 12:26:30pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{

public:
  DJAudioPlayer(juce::AudioFormatManager &_formatManager);

  ~DJAudioPlayer();

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

  void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;

  void releaseResources() override;

  void loadURL(const juce::URL &audioURL);

  void setGain(double gain);

  void setSpeed(double ratio);

  void setPosition(double posInSec);

  void setPositionRelative(double pos);

  void start();

  void stop();

  double getPositionRelative() const;

  void setBass(double frequency)
  {
    bassFilterSource.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, frequency));
  }

  void setTreble(double frequency)
  {
    trebleFilterSource.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, frequency));
  }

private:
  juce::AudioFormatManager &formatManager;
  std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
  juce::AudioTransportSource transportSource;
  juce::IIRFilterAudioSource bassFilterSource{&transportSource, false};
  juce::IIRFilterAudioSource trebleFilterSource{&bassFilterSource, false};
  juce::ResamplingAudioSource resampleSource{&trebleFilterSource, false, 2};
  // juce::IIRFilterAudioSource midFilterSource{&bassFilterSource, false};
};

// void openButtonClicked()
// {
//   chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
//                                                 juce::File{},
//                                                 "*.wav");
//   auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

//   chooser->launchAsync(chooserFlags, [this](const FileChooser &fc)
//                        {
//             auto file = fc.getResult();

//             if (file != File{})
//             {
//                 auto* reader = formatManager.createReaderFor (file);

//                 if (reader != nullptr)
//                 {
//                     auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
//                     transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
//                     playButton.setEnabled (true);
//                     thumbnail.setSource (new juce::FileInputSource (file));                            // [7]
//                     readerSource.reset (newSource.release());
//                 }
//             } });
// }
