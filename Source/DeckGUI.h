/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Jun 2022 1:55:53pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener,
                public juce::FileDragAndDropTarget {
public:
    DeckGUI(DJAudioPlayer *player);

    ~DeckGUI() override;

    void paint(juce::Graphics &) override;

    void resized() override;

    // From Button::Listener
    void buttonClicked(juce::Button *) override;

    // From Slider::Listener
    void sliderValueChanged(juce::Slider *slider) override;

    // From FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray &files);

    // From FileDragAndDropTarget
    void filesDropped(const juce::StringArray &files, int x, int y);


private:
    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    juce::TextButton loadButton{"LOAD"};

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    DJAudioPlayer *player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
