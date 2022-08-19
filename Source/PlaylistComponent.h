#pragma once

#include <JuceHeader.h>

// TODO - Split into own class
// Add new constructor that initializes all members: Track(title, format, duration, audioURL)
struct Track
{
    juce::String title;
    juce::String format;
    double duration;
    juce::URL audioURL;

    Track(const juce::File &audioFile)
    {
        title = audioFile.getFileNameWithoutExtension();
        format = audioFile.getFileExtension();
        audioURL = juce::URL{audioFile};
        duration = getDurationInSeconds(audioFile);
    }

    double getDurationInSeconds(const juce::File &audioFile)
    {
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        if (auto reader = formatManager.createReaderFor(audioFile))
        {
            auto lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
            delete reader;
            return lengthInSeconds;
        }
        return -1;
    }
};

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener, public juce::FileDragAndDropTarget, public juce::TextEditor::Listener
{
public:
    PlaylistComponent();

    ~PlaylistComponent() override;

    //==============================================================================

    void paint(juce::Graphics &) override;

    void resized() override;

    //==============================================================================

    int getNumRows() override;

    void paintRowBackground(juce::Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

    // From Button::Listener
    void buttonClicked(juce::Button *) override;

    // From FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray &files) override;

    void filesDropped(const juce::StringArray &files, int x, int y) override;

    void insertUniqueTrack(const Track &newTrack);

    void selectedRowsChanged(int row);

    int getFirstSelectedRow();

    void updateTable();

    void textEditorTextChanged(juce::TextEditor &textEditor);

    std::vector<Track> tracks;
    std::vector<Track> filteredTracks;
    juce::SparseSet<int> selectedRows;

private:
    juce::TableListBox tableComponent;

    juce::TextButton addTracks;
    juce::TextButton removeTracks;
    juce::TextButton savePlaylist;
    juce::TextButton loadPlaylist;
    juce::TextButton clearPlaylist;

    juce::TextEditor searchTrack;

    juce::FileChooser fChooser{"Name of dialog", juce::File::getSpecialLocation(juce::File::userMusicDirectory)};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
