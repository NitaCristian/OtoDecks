#pragma once

#include <JuceHeader.h>

/**
 * Track is a class that stores all the details about a track such as title, extension, duration and the URL to the file
 */
class Track {
public:
    Track();

    /**
     * Constructor method used to initialize the data members using a file
     *
     * @param audioFile audio file of the track
     */
    explicit Track(const juce::File &audioFile);

    /**
     * Constructor method used to initialize the data members using values for each member
     *
     * @param _title title of the track
     * @param _extension extension of the file name, eg. .mp3 or .wav
     * @param _duration duration of the track in seconds
     * @param _audioURL URL of the file
     */
    Track(juce::String _title, juce::String _extension, double _duration, juce::URL _audioURL);

    //==============================================================================

    /**
     * Method to get the title of the track
     *
     * @returns title of the track
     */
    juce::String getTitle() const;

    /**
     * Method to get the extension of the track
     *
     * @returns extension of the track
     */
    juce::String getExtension() const;

    /**
     * Method to get the URL of the track
     *
     * @return URL of the track
     */
    juce::URL getAudioURL() const;

    /**
     * Method to get the duration of the track
     *
     * @returns duration of the track
     */
    double getDuration() const;

    /**
     * Method used to calculate and return the duration of the track in seconds
     *
     * @param audioFile audio file of the track
     * @returns duration in seconds of the track
     */
    static double calculateDuration(const juce::File &audioFile);

    /**
     * Method to combine all the data about a track into a String
     *
     * @returns a string containing all the data about a track, eg. "title,extension,duration,audioURL"
     */
    juce::String toString() const;

private:
    // Title of the track
    juce::String title;
    // File extension of the file
    juce::String extension;
    // Duration in seconds of the track
    double duration;
    // URL to the audio file
    juce::URL audioURL;
};