/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jun 2022 12:26:30pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {

public:
    DJAudioPlayer();

    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;

    void releaseResources() override;

    void loadURL(const juce::URL& audioURL);

    void setGain(double gain);

    void setSpeed(double ratio);

    void setPosition(double posInSec);

    void setPositionRelative(double pos);

    void start();

    void stop();

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};