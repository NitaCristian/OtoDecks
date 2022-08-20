#include "Track.h"

Track::Track() : title{""}, extension{""}, duration{-1}, audioURL{""}
{}

Track::Track(const juce::File &audioFile)
        : title{audioFile.getFileNameWithoutExtension()},
          extension{audioFile.getFileExtension()},
          audioURL{juce::URL{audioFile}},
          duration{calculateDuration(audioFile)} {
}

Track::Track(juce::String _title, juce::String _extension, double _duration, juce::URL _audioURL)
        : title{_title},
          extension{_extension},
          duration{_duration},
          audioURL{_audioURL} {
}

juce::String Track::getTitle() const {
    return title;
}

juce::String Track::getExtension() const {
    return extension;
}

double Track::getDuration() const {
    return duration;
}

juce::URL Track::getAudioURL() const {
    return audioURL;
}

double Track::calculateDuration(const juce::File &audioFile) {
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    // Create a reader for the file and if it is valid, execute the code
    if (auto reader = formatManager.createReaderFor(audioFile)) {
        // Calculate the length in seconds
        auto lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
        // Delete the reader object
        delete reader;
        // Return the length
        return lengthInSeconds;
    }

    // Return -1 if a reader couldn't be created
    return -1;
}

juce::String Track::toString() const {
    return title + "," + extension + "," + std::to_string(duration) + "," + audioURL.toString(false);
}
