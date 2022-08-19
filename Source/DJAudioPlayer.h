#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
    // TODO - REFACTOR
    // 1. Add comments
public:
    DJAudioPlayer(juce::AudioFormatManager &_formatManager);

    ~DJAudioPlayer();

    //==============================================================================

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;

    void releaseResources() override;

    //==============================================================================

    void loadURL(const juce::URL &audioURL);

    void start();

    void pause();

    void stop();

    void setGain(double gain);

    void setSpeed(double ratio);

    void setPosition(double posInSec);

    void setPositionRelative(double pos);

    double getPositionRelative() const;

    void setBass(double frequency);

    void setTreble(double frequency);

private:
    juce::AudioFormatManager &formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::IIRFilterAudioSource bassFilterSource{&transportSource, false};
    juce::IIRFilterAudioSource trebleFilterSource{&bassFilterSource, false};
    juce::ResamplingAudioSource resampleSource{&trebleFilterSource, false, 2};
};