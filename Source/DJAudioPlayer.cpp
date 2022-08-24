#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager &_formatManager) : formatManager(_formatManager) {
}

DJAudioPlayer::~DJAudioPlayer() = default;

//==============================================================================

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    bassFilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    bassFilterSource.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 10000.0));

    trebleFilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    trebleFilterSource.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, 50.0));
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    if (readerSource == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    bassFilterSource.releaseResources();
    trebleFilterSource.releaseResources();
    resampleSource.releaseResources();
}

//==============================================================================

void DJAudioPlayer::loadURL(const juce::URL &audioURL) {
    auto *reader = formatManager.createReaderFor(
            audioURL.createInputStream(juce::URL::InputStreamOptions{juce::URL::ParameterHandling::inAddress}));

    if (reader == nullptr) {
        return;
    }

    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
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
    if (ratio <= 0.0 || ratio > 5.0) {
        std::cout << "Error: The ratio needs to be between 0.0 and 5.0.\n";
        return;
    }

    resampleSource.setResamplingRatio(ratio);
}

void DJAudioPlayer::setPosition(double posInSec) {
    transportSource.setPosition(posInSec);
}

double DJAudioPlayer::getPositionRelative() const {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
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

void DJAudioPlayer::pause() {
    transportSource.stop();
}

void DJAudioPlayer::stop() {
    pause();
    setPosition(0);
}

void DJAudioPlayer::setBass(double frequency) {
    bassFilterSource.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, frequency));
}

void DJAudioPlayer::setTreble(double frequency) {
    trebleFilterSource.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, frequency));
}
