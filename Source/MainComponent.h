#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent : public juce::AudioAppComponent {
public:
    //==============================================================================

    MainComponent();

    ~MainComponent() override;

    //==============================================================================

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;

    void releaseResources() override;

    //==============================================================================

    void paint(juce::Graphics &g) override;

    void resized() override;

private:
    // A list of available audio formats
    juce::AudioFormatManager formatManager;
    // A list used to manage multiple AudioThumbnail objects
    juce::AudioThumbnailCache thumbnailCache{100};

    // Playlist that stores all the tracks
    PlaylistComponent playlistComponent;

    // DJAudioPlayer for the first deck
    DJAudioPlayer player1{formatManager};
    // First deck to control the dj player
    DeckGUI deckGUI1{&player1, formatManager, thumbnailCache, &playlistComponent};

    // DjAudioPlayer for the second deck
    DJAudioPlayer player2{formatManager};
    // Second deck to control the dj player
    DeckGUI deckGUI2{&player2, formatManager, thumbnailCache, &playlistComponent};

    // Mixer to combine both audio sources
    juce::MixerAudioSource mixerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
