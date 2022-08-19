#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "MyLookAndFeel.h"

//==============================================================================
/*
  DeckGUI is a class that contains all the components that change the playback of the audio.
  It includes buttons to play, pause, stop and load an audio file,
  sliders to change the gain, speed, bass and treble of a track,
  as well as a label to display the currently playing track and a waveform.
 */
class DeckGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::Timer, public juce::FileDragAndDropTarget
{
    // TODO - REFACTOR
    // Add comments
public:
    /**
     * Constructor method that initializes private members and components used
     *
     * @param player Pointer to a DJAudioPlayer
     * @param
     */
    DeckGUI(DJAudioPlayer *player, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse, PlaylistComponent *playlistComponent);

    ~DeckGUI() override;

    //==============================================================================

    void paint(juce::Graphics &) override;

    // method called when the component is resized; also used to set components bounds
    void resized() override;

    //==============================================================================

    // Inherited methods

    // From Button::Listener
    void buttonClicked(juce::Button *) override;

    // From Slider::Listener
    void sliderValueChanged(juce::Slider *slider) override;

    // From FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray &files) override;

    void filesDropped(const juce::StringArray &files, int x, int y) override;

    // From Timer
    void timerCallback() override;

private:
    // Pointer to the playlist component
    PlaylistComponent *playlist;

    // Button to play an audio file
    juce::TextButton playButton;
    // Button to pause an audio file
    juce::TextButton pauseButton;
    // Button to stop and return to the beginning of an audio file
    juce::TextButton stopButton;
    // Button to load a track from the playlist to the deck
    juce::TextButton loadButton;

    // Slider to control the gain
    juce::Slider gainSlider;
    juce::Label gainLabel;

    // Slider to control the speed of the playback
    juce::Slider speedSlider;
    juce::Label speedLabel;

    // Slider to contorl the bass filter
    juce::Slider bassSlider;
    juce::Label bassLabel;

    // Slider to control the treble filter
    juce::Slider trebleSlider;
    juce::Label trebleLabel;

    // Pointer to the DJAudioPlayer for this deck
    DJAudioPlayer *djAudioPlayer;

    // Label for the track name
    juce::Label trackName;

    // Waveform
    WaveformDisplay waveformDisplay;

    // File chooser object to select files
    juce::FileChooser fChooser{"Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.mp3"};

    // Custom LookAndFeel class for buttons and rotary sliders
    MyLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
