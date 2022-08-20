#pragma once

#include <JuceHeader.h>

/**
 * DJAudioPlayer is a class that manages the loading and playing of an audio file
 */
class DJAudioPlayer : public juce::AudioSource {
public:
    /**
     * Constructor method used to initialize data members
     *
     * @param _formatManager list of available audio formats
     */
    explicit DJAudioPlayer(juce::AudioFormatManager &_formatManager);

    /**
     * Destructor method used to deallocate memory and cleanup before the object is destroyed
     */
    ~DJAudioPlayer() override;

    //==============================================================================
    // Inherited from AudioSource

    /**
     * Tells the source to prepare for playing
     *
     * @param samplesPerBlockExpected the number of samples that the source will be expected to supply each time
     * @param sampleRate the sample rate that the output will be used at
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Called repeatedly to fetch subsequent blocks of audio data
     *
     * @param bufferToFill buffer that contains audio data
     */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;

    /**
     * Allows the source to release anything it no longer needs after playback has stopped
     */
    void releaseResources() override;

    //==============================================================================

    /**
     *
     * @param audioURL
     */
    void loadURL(const juce::URL &audioURL);

    /**
     * Method to start playing the audio
     */
    void start();

    /**
     * Method to pause the playing of the audio
     */
    void pause();

    /**
     * Method to stop the playing of the audio
     */
    void stop();

    /**
     * Method to set the gain
     *
     * @param gain
     */
    void setGain(double gain);

    /**
     * Method to set the playback speed
     *
     * @param ratio
     */
    void setSpeed(double ratio);

    /**
     * Set current playback position
     *
     * @param posInSec position of the new playback in seconds
     */
    void setPosition(double posInSec);

    /**
     * Set the current playback position relative to the range [0, 1]
     *
     * @param pos position of the playback relative to the range [0, 1]
     */
    void setPositionRelative(double pos);

    /**
     * Get the current playback position relative to the range [0, 1]
     *
     * @return the current playback position in the range [0, 1]
     */
    double getPositionRelative() const;

    /**
     * Method to set the frequency of the bass filter
     *
     * @param frequency
     */
    void setBass(double frequency);

    /**
     * Method to set the frequency of the treble filter
     *
     * @param frequency
     */
    void setTreble(double frequency);

private:
    // A list of available audio formats
    juce::AudioFormatManager &formatManager;
    // A type of AudioSource that will read from an AudioFormatReader
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // An AudioSource that allows it to be played, stopped, started
    juce::AudioTransportSource transportSource;
    // An AudioSource that performs an IIR filter on another source to create a bass filter
    juce::IIRFilterAudioSource bassFilterSource{&transportSource, false};
    // An AudioSource that performs an IIR filter on another source to create a treble filter
    juce::IIRFilterAudioSource trebleFilterSource{&bassFilterSource, false};
    // A type of AudioSource that takes an input source and changes its sample rate
    juce::ResamplingAudioSource resampleSource{&trebleFilterSource, false, 2};
};