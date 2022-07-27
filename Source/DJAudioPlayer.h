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
};