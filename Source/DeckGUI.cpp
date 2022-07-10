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
DeckGUI::DeckGUI(DJAudioPlayer *_player, juce::AudioFormatManager &formatManagerToUse,
                 juce::AudioThumbnailCache &cacheToUse) : djAudioPlayer{_player}, waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(gainSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    gainSlider.addListener(this);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(1.0);

    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 100.0);
    speedSlider.setValue(1.0);

    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);

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
    auto rowHeight = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);

    gainSlider.setBounds(0, 2 * rowHeight, getWidth(), rowHeight);
    speedSlider.setBounds(0, 3 * rowHeight, getWidth(), rowHeight);
    posSlider.setBounds(0, 4 * rowHeight, getWidth(), rowHeight);

    waveformDisplay.setBounds(0, 5 * rowHeight, getWidth(), rowHeight * 2);
    loadButton.setBounds(0, 7 * rowHeight, getWidth(), rowHeight);
}

void DeckGUI::buttonClicked(juce::Button *button)
{
    if (button == &playButton)
    {
        djAudioPlayer->start();
    }
    if (button == &stopButton)
    {
        djAudioPlayer->stop();
    }
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select file");
        if (chooser.browseForFileToOpen())
        {
            auto audioURL = juce::URL{chooser.getResult()};
            djAudioPlayer->loadURL(audioURL);
            waveformDisplay.loadURL(audioURL);
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
    if (slider == &posSlider)
    {
        djAudioPlayer->setPositionRelative(slider->getValue());
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
        juce::URL fileURL = juce::URL{juce::File{filename}};
        djAudioPlayer->loadURL(fileURL);
        return;
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
