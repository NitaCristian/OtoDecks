/*
  ==============================================================================

    DeckGUI.cpp
    Created: 28 Jun 2022 1:55:53pm
    Author:  cristi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer *_player, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse, PlaylistComponent *playlistComponent)
    : djAudioPlayer{_player}, waveformDisplay(formatManagerToUse, cacheToUse, _player), playlist(playlistComponent)
{
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setButtonText("Play");

    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    pauseButton.setButtonText("Pause");

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setButtonText("Stop");

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setButtonText("Load");

    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(1.0);

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, true);

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(1.0, 5.0);
    speedSlider.setValue(1.0);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(bassSlider);
    bassSlider.addListener(this);
    bassSlider.setRange(50.0, 20000.0);
    bassSlider.setValue(20000.0);
    bassSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    addAndMakeVisible(bassLabel);
    bassLabel.setText("Bass", juce::dontSendNotification);

    addAndMakeVisible(trebleSlider);
    trebleSlider.addListener(this);
    trebleSlider.setRange(50.0, 20000.0);
    trebleSlider.setValue(50.0);
    trebleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    addAndMakeVisible(trebleLabel);
    trebleLabel.setText("Treble", juce::dontSendNotification);

    addAndMakeVisible(trackName);
    trackName.setFont(juce::Font(32.0f));

    addAndMakeVisible(waveformDisplay);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
}

void DeckGUI::resized()
{
    auto area = getLocalBounds();

    auto rowHeight = getHeight() / 8;

    auto trackNameArea = area.removeFromTop(rowHeight);
    trackName.setBounds(trackNameArea);

    auto waveformArea = area.removeFromTop(rowHeight);
    waveformDisplay.setBounds(waveformArea);

    auto buttonArea = area.removeFromTop(rowHeight);
    auto buttonWidth = getWidth() / 4;
    playButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    pauseButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    stopButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    loadButton.setBounds(buttonArea.removeFromLeft(buttonWidth));

    auto sliderArea = area.removeFromTop(rowHeight * 2);
    auto padding = sliderArea.removeFromLeft(50);
    gainSlider.setBounds(sliderArea.removeFromTop(rowHeight));
    speedSlider.setBounds(sliderArea.removeFromTop(rowHeight));

    auto eqArea = area.removeFromTop(rowHeight * 2);
    bassSlider.setBounds(eqArea.removeFromLeft(getWidth() / 2));
    trebleSlider.setBounds(eqArea.removeFromLeft(getWidth() / 2));
}

void DeckGUI::buttonClicked(juce::Button *button)
{
    if (button == &playButton)
    {
        djAudioPlayer->start();
    }
    if (button == &pauseButton)
    {
        djAudioPlayer->stop();
    }
    if (button == &stopButton)
    {
        djAudioPlayer->stop();
        djAudioPlayer->setPosition(0);
    }
    if (button == &loadButton)
    {
        int firstSelectedSong = playlist->getFirstSelectedRow();
        if (firstSelectedSong != -1)
        {
            auto track = playlist->tracks[firstSelectedSong];
            auto audioURL = track.audioURL;
            this->djAudioPlayer->loadURL(audioURL);
            this->waveformDisplay.loadURL(audioURL);
            this->trackName.setText(track.title, juce::dontSendNotification);
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &gainSlider)
    {
        djAudioPlayer->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        djAudioPlayer->setSpeed(slider->getValue());
    }
    if (slider == &bassSlider)
    {
        djAudioPlayer->setBass(slider->getValue());
    }
    if (slider == &trebleSlider)
    {
        djAudioPlayer->setTreble(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (const auto &filename : files)
    {
        auto file = juce::File{filename};
        auto audioURL = juce::URL{file};
        djAudioPlayer->loadURL(audioURL);
        waveformDisplay.loadURL(audioURL);
        playlist->insertUniqueTrack(Track(file));
        return;
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
