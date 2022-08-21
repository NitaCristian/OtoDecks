#pragma once

#include <JuceHeader.h>
#include "Track.h"
#include "MyLookAndFeel.h"

/**
 * PlaylistComponent is a class that creates and manages a playlist of tracks. It can load and delete tracks, and also save them as a playlist file
 */
class PlaylistComponent
        : public juce::Component,
          public juce::TableListBoxModel,
          public juce::Button::Listener,
          public juce::FileDragAndDropTarget,
          public juce::TextEditor::Listener {
public:
    /**
     *  Constructor method used to initialize data members
     */
    PlaylistComponent();

    /**
     * Destructor method used to deallocate memory and cleanup before the object is destroyed
     */
    ~PlaylistComponent() override;

    //==============================================================================
    // Inherited from Component

    /**
     *  Method used by the component to draw its content
     *
     * @param g the graphics context that must be used to do the drawing operations
     */
    void paint(juce::Graphics &) override;

    /**
     * Method used to laying out the child components when the width or height of the component changes
     */
    void resized() override;

    //==============================================================================
    // Inherited from TableListBoxModel

    /**
     *  Method that returns the number of rows currently in the table
     */
    int getNumRows() override;

    /**
     * Method that draws the background behind one of the rows in the table

     * @param g the graphics context that must be used to do the drawing operations
     * @param rowNumber the number of the current row
     * @param width width of the row
     * @param height height of the row
     * @param rowIsSelected flag that indicates if the row is selected
     */
    void paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) override;

    /**
     * Method that draws one of the cells
     *
     * @param g the graphics context that must be used to do the drawing operations
     * @param rowNumber the number of the current row
     * @param columnId id of the column
     * @param width width of the row
     * @param height height of the row
     * @param rowIsSelected flag that indicates if the row is selected
     */
    void paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /**
     * Method that is used to create or update a custom component to go in a cell
     *
     * @param rowNumber the number of the current row
     * @param columnId id of the column
     * @param isRowSelected flag that indicates if the row is selected
     * @param existingComponentToUpdate
     * @return
     */
    juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

    /**
     * Method called when the rows have been selected
     * @param row
     */
    void selectedRowsChanged(int row) override;

    //==============================================================================
    // Inherited from Button::Listener

    /**
     * Method called when a button is clicked
     *
     * @param button button pressed
     */
    void buttonClicked(juce::Button *button) override;

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
     * @param files the set of paths of the files that the user is dragging
     * @param x the mouse x position
     * @param y the mouse y position
     */
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    //==============================================================================
    // Inherited from TextEditor::Listener

    /**
     *
     * @param textEditor
     */
    void textEditorTextChanged(juce::TextEditor &textEditor) override;

    //==============================================================================

    /**
     * Method to insert a new track into the list of all tracks
     *
     * @param newTrack track to be inserted
     */
    void insertUniqueTrack(const Track &newTrack);

    /**
     * Method to get the index of the first selected row
     *
     * @returns index of the first selected row
     */
    int getFirstSelectedRow();

    Track getFirstSelectedTrack();

    /**
     * Method to update the table component after changes to the track list have been made
     */
    void updateTable();

    juce::String createPlaylist();

    // TODO - comments
    // TODO - playlist file extension and checking
    // todo - check uploaded file extension; findFormatForFileExtension() - audioformatmanager

    void savePlaylist(const juce::File &playlistFile, const juce::String &contents);

    void loadPlaylist(const juce::File &playlistFile);

    void updateFilteredTracks(const juce::String &text);

private:
    // Custom LookAndFeel
    MyLookAndFeel myLookAndFeel;
    // The table component that displays the tracks
    juce::TableListBox tableComponent;
    // Button to add tracks to the playlist
    juce::TextButton addTracks;
    // Button to remove tracks from the playlist
    juce::TextButton removeTracks;
    // Button to save the playlist
    juce::TextButton savePlaylistBtn;
    // Button to load a playlist
    juce::TextButton loadPlaylistBtn;
    // Button to remove all tracks from the playlist
    juce::TextButton clearPlaylist;
    // Text editor to search for specific tracks in the playlist
    juce::TextEditor searchTrack;
    // Vector that stores all the tracks
    std::vector<Track> tracks;
    // Vector that stores tracks after they have been filtered
    std::vector<Track> filteredTracks;
    // Set that contains the selected rows
    juce::SparseSet<int> selectedRows;
    // File chooser component used to create and load files
    juce::FileChooser fChooser{"Name of dialog", juce::File::getSpecialLocation(juce::File::userMusicDirectory)};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
