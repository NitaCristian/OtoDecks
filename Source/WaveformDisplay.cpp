#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse, DJAudioPlayer *_djAudioPlayer,
                                 juce::Colour colour)
        : audioThumbnail(1000, formatManagerToUse, cacheToUse),
          fileLoaded(false),
          position(0.0),
          djAudioPlayer(_djAudioPlayer), colour(colour) {
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() = default;

//==============================================================================

void WaveformDisplay::paint(juce::Graphics &g) {
    // Clear the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw an outline around the component
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    if (!fileLoaded) {
        g.setFont(20.0f);
        g.setColour(colour);
        // Draw some placeholder text
        g.drawText("File not loaded", getLocalBounds(), juce::Justification::centred, true);
        return;
    }

    g.setColour(colour);
    // Get the local bounds of the component
    auto area = getLocalBounds();
    // Set the height of the waveform of one channel
    auto channelWaveformHeight = getHeight() / 2;

    // Draw the waveform for the two channels
    audioThumbnail.drawChannel(g, area.removeFromTop(channelWaveformHeight), 0, audioThumbnail.getTotalLength(), 0, 1);
    audioThumbnail.drawChannel(g, area.removeFromTop(channelWaveformHeight), 0, audioThumbnail.getTotalLength(), 1, 1);

    // Draw an indicator of the current position
    auto width = getWidth() / 50;
    const auto indicatorPosition = position * getWidth();

    g.setColour(juce::Colours::greenyellow);
    g.drawRect(indicatorPosition - width / 2, 0, width, getHeight());
}

void WaveformDisplay::resized() {
}

void WaveformDisplay::mouseDown(const juce::MouseEvent &event) {
    // Get the mouse x position
    auto mouseX = event.x;
    // Calculate the relative position
    auto relativePos = mouseX * 1.0 / getWidth();
    // Set the relative position of the player
    djAudioPlayer->setPositionRelative(relativePos);
    // Set the relative position of the indicator
    setPositionRelative(relativePos);
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent &event) {
    // Get the mouse x position
    auto mouseX = event.x;
    // Calculate the relative position
    auto relativePos = mouseX * 1.0 / getWidth();
    // Set the relative position of the player
    djAudioPlayer->setPositionRelative(relativePos);
    // Set the relative position of the indicator
    setPositionRelative(relativePos);
}

//==============================================================================

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
    repaint();
}

//==============================================================================

void WaveformDisplay::loadURL(const juce::URL &audioURL) {
    // Clear the current audio thumbnail
    audioThumbnail.clear();

    // Load a new thumbnail from the audio source
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::setPositionRelative(double pos) {
    // If the new position is different from the last position
    // And is a number
    if (pos != position && !std::isnan(pos)) {
        // Set the position to the new position
        position = pos;
        // Repaint the waveform
        repaint();
    }
}