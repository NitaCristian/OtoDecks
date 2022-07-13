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
    // frequencySlider.setTextValueSuffix(" Hz");

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(1.0, 5.0);
    speedSlider.setValue(1.0);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);

    addAndMakeVisible(posLabel);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);

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

    auto rowHeight = getHeight() / 5;

    auto waveformArea = area.removeFromTop(rowHeight);
    waveformDisplay.setBounds(waveformArea);

    auto buttonArea = area.removeFromTop(rowHeight);
    auto buttonWidth = getWidth() / 4;
    playButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    pauseButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    stopButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    loadButton.setBounds(buttonArea.removeFromLeft(buttonWidth));

    auto padding = area.removeFromLeft(50);
    gainSlider.setBounds(area.removeFromTop(rowHeight));
    speedSlider.setBounds(area.removeFromTop(rowHeight));
    posSlider.setBounds(area.removeFromTop(rowHeight));
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
        auto audioURL = juce::URL{juce::File{filename}};
        djAudioPlayer->loadURL(audioURL);
        waveformDisplay.loadURL(audioURL);
        // Load only 1 file dropped then return
        // and do not process the rest of the files
        return;
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
