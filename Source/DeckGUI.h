#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "MyLookAndFeel.h"

/**
  * DeckGUI is a class that contains all the components that change the playback of the audio.
  * It includes buttons to play, pause, stop and load an audio file, sliders to change the gain, speed,
  * bass and treble of a track, as well as a label to display the currently playing track and a waveform.
 */
class DeckGUI
        : public juce::Component,
          public juce::Button::Listener,
          public juce::Slider::Listener,
          public juce::Timer,
          public juce::FileDragAndDropTarget {
public:
    /**
     * Constructor method that initializes private members and components used
     *
     * @param player DJAudioPlayer that corresponds to this deck
     * @param formatManager a list of available audio formats
     * @param cache manages multiple AudioThumbnail objects
     * @param playlistComponent a pointer to the playlist
     */
    DeckGUI(DJAudioPlayer *player, juce::AudioFormatManager &formatManager, juce::AudioThumbnailCache &cache, PlaylistComponent *playlistComponent,
            juce::Colour primary);

    /**
    * Destructor method used to deallocate memory and cleanup before the object is destroyed
    */
    ~DeckGUI() override;

    //==============================================================================
    // Inherited from Component

    /**
     *  Method used by the component to draw its content
     *
     * @param g the graphics context that must be used to do the drawing operations
     */
    void paint(juce::Graphics &g) override;

    /**
     * Method used to laying out the child components when the width or height of the component changes
     */
    void resized() override;

    //==============================================================================
    // Inherited from Button::Listener

    /**
     * Method called when a button is clicked
     *
     * @param button button pressed
     */
    void buttonClicked(juce::Button *button) override;

    //==============================================================================
    // Inherited from Slider::Listener

    /**
     * Method called when the value of a slider has been changed
     *
     * @param slider slider changed
     */
    void sliderValueChanged(juce::Slider *slider) override;

    //==============================================================================
    // Inherited from FileDragAndDropTarget

    /**
     * Callback to check whether this target is interested in the set of files being offered
     *
     * @param files the set of paths of the files that the user is dragging
     * @return if it is interested in file drag
     */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;

    /**
     * Callback to indicate that the user has dropped the files onto this component
     *
     * @param files the set of paths of the files that the user has dropped
     * @param x the mouse x position
     * @param y the mouse y position
     */
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    //==============================================================================
    // Inherited from Timer

    /**
     * A callback method that gets called periodically by the timer
     */
    void timerCallback() override;

private:
    // Primary colour of the LookAndFeel
    juce::Colour primary;
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
    // Slider to control the speed of the playback
    juce::Slider speedSlider;
    // Slider to control the bass filter
    juce::Slider bassSlider;
    // Slider to control the treble filter
    juce::Slider trebleSlider;
    // Pointer to the DJAudioPlayer for this deck
    DJAudioPlayer *djAudioPlayer;
    // Label for the track name
    juce::Label trackName;
    // Waveform
    WaveformDisplay waveformDisplay;
    // File chooser object to select files
    juce::FileChooser fChooser{"Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory),
                               "*.mp3"};
    // Custom LookAndFeel class for buttons and rotary sliders
    MyLookAndFeel customLookAndFeel{primary};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
