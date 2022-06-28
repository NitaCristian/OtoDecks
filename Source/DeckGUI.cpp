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
DeckGUI::DeckGUI(DJAudioPlayer *_player) : player{_player} {
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    posSlider.setRange(0.0, 1.0);
}

DeckGUI::~DeckGUI() {
}

void DeckGUI::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DeckGUI::resized() {
    auto rowHeight = getHeight() / 6;
    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    volSlider.setBounds(0, 2 * rowHeight, getWidth(), getHeight() / 5);
    speedSlider.setBounds(0, 3 * rowHeight, getWidth(), getHeight() / 5);
    posSlider.setBounds(0, 4 * rowHeight, getWidth(), getHeight() / 5);
    loadButton.setBounds(0, 5 * rowHeight, getWidth(), rowHeight);
}

void DeckGUI::buttonClicked(juce::Button *button) {
    if (button == &playButton) {
        player->start();
    }
    if (button == &stopButton) {
        player->stop();
    }
    if (button == &loadButton) {
        juce::FileChooser chooser("Select file");
        if (chooser.browseForFileToOpen()) {
            player->loadURL(juce::URL{chooser.getResult()});
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider *slider) {
    if (slider == &volSlider) {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
    }
}