/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 28 Jun 2022 12:26:30pm
    Author:  cristi

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager &_formatManager) : formatManager(_formatManager) {

}

DJAudioPlayer::~DJAudioPlayer() {

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
//    if (readerSource == nullptr) {
//        bufferToFill.clearActiveBufferRegion();
//        return;
//    }
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(const juce::URL &audioURL) {
    auto *reader = formatManager.createReaderFor(
            audioURL.createInputStream(juce::URL::InputStreamOptions{juce::URL::ParameterHandling::inAddress}));
    if (reader == nullptr) {
        std::cout << "Error: The formatManager could not create a reader for the file.\n";
    }

    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0.0 || gain > 1.0) {
        std::cout << "Error: The gain needs to be between 0.0 and 1.0.\n";
        return;
    }
    transportSource.setGain(gain);
}

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio <= 0.0 || ratio > 100.0) {
        std::cout << "Error: The ratio needs to be between 0.0 and 100.0.\n";
        return;
    }

    resampleSource.setResamplingRatio(ratio);
}

void DJAudioPlayer::setPosition(double posInSec) {
    transportSource.setPosition(posInSec);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0.0 || pos > 1.0) {
        std::cout << "Error: The pos needs to be between 0.0 and 1.0.\n";
        return;
    }
    double posInSeconds = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSeconds);
}

void DJAudioPlayer::start() {
    transportSource.start();
}

void DJAudioPlayer::stop() {
    transportSource.stop();
}
