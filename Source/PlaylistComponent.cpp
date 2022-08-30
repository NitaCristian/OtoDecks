#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent() {
    setLookAndFeel(&myLookAndFeel);

    addAndMakeVisible(searchTrack);
    searchTrack.addListener(this);

    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("No.", 1, 100);
    tableComponent.getHeader().addColumn("Title", 2, 100);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("Audio format", 4, 100);
    tableComponent.setModel(this);
    tableComponent.setMultipleSelectionEnabled(true);

    addAndMakeVisible(addTracks);
    addTracks.setButtonText("Add tracks");
    addTracks.addListener(this);

    addAndMakeVisible(removeTracks);
    removeTracks.setButtonText("Remove tracks");
    removeTracks.addListener(this);

    addAndMakeVisible(savePlaylistBtn);
    savePlaylistBtn.setButtonText("Save playlist");
    savePlaylistBtn.addListener(this);

    addAndMakeVisible(loadPlaylistBtn);
    loadPlaylistBtn.setButtonText("Load playlist");
    loadPlaylistBtn.addListener(this);

    addAndMakeVisible(clearPlaylist);
    clearPlaylist.setButtonText("Clear playlist");
    clearPlaylist.addListener(this);

    juce::File playlist = juce::File::getCurrentWorkingDirectory().getChildFile("OtoDecks.state");
    if (playlist.existsAsFile()) {
        loadPlaylist(playlist);
    }
}

PlaylistComponent::~PlaylistComponent() {
    setLookAndFeel(nullptr);
    juce::File playlist = juce::File::getCurrentWorkingDirectory().getChildFile("OtoDecks.state");
    playlist.create();
    savePlaylist(playlist, createPlaylist());
}

//==============================================================================

void PlaylistComponent::paint(juce::Graphics &g) {
    // Clear the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PlaylistComponent::resized() {
    // Get the total area of the component
    auto area = getLocalBounds();

    // Set the width of one button
    auto buttonWidth = getWidth() / 8;
    // Get the area of the buttons
    auto buttonsArea = area.removeFromLeft(buttonWidth).reduced(5);
    // Set the height of one button
    auto buttonHeight = getHeight() / 5;

    addTracks.setBounds(buttonsArea.removeFromTop(buttonHeight));
    removeTracks.setBounds(buttonsArea.removeFromTop(buttonHeight));
    savePlaylistBtn.setBounds(buttonsArea.removeFromTop(buttonHeight));
    loadPlaylistBtn.setBounds(buttonsArea.removeFromTop(buttonHeight));
    clearPlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));

    searchTrack.setBounds(area.removeFromTop(30));
    tableComponent.setBounds(area);
}

//==============================================================================

int PlaylistComponent::getNumRows() {
    return filteredTracks.size();
}

void
PlaylistComponent::paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) {
    g.fillAll(juce::Colours::darkgrey);
    if (rowIsSelected) {
        g.fillAll(juce::Colours::greenyellow);
    }
}

void PlaylistComponent::paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height,
                                  bool rowIsSelected) {
    auto track = filteredTracks[rowNumber];
    auto duration = floor(track.getDuration() / 60 * 100) / 100;

    if (columnId == 1) // No.
    {
        g.drawText(std::to_string(rowNumber + 1), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 2) // Title
    {
        g.drawText(track.getTitle(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 3) // Duration
    {
        g.drawText(std::to_string(duration).substr(0, 4), 2, 0, width - 4, height, juce::Justification::centredLeft,
                   true);
    }
    if (columnId == 4) // Audio format
    {
        g.drawText(track.getExtension(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

juce::Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                                            Component *existingComponentToUpdate) {
    return existingComponentToUpdate;
}

void PlaylistComponent::selectedRowsChanged(int row) {
    selectedRows = tableComponent.getSelectedRows();
}

//==============================================================================

void PlaylistComponent::buttonClicked(juce::Button *button) {
    if (button == &addTracks) {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser) {
                                 juce::Array<juce::File> chosenFiles = chooser.getResults();
                                 for (const auto &file: chosenFiles) {
                                     Track newTrack(file);
                                     insertUniqueTrack(newTrack);
                                 }
                             });
    }
    if (button == &removeTracks) {
        for (auto i = 0; i < selectedRows.size(); i++) {
            tracks.erase(tracks.begin() + selectedRows[i]);
        }
        updateFilteredTracks(searchTrack.getText());
        updateTable();
    }
    if (button == &savePlaylistBtn) {
        auto fileChooserFlags = juce::FileBrowserComponent::saveMode;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser) {
                                 auto file = chooser.getResult();
                                 file = file.withFileExtension("otolist");
                                 savePlaylist(file, createPlaylist());
                             });
    }
    if (button == &loadPlaylistBtn) {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser) {
                                 auto file = chooser.getResult();
                                 if (file.hasFileExtension("otolist"))
                                     loadPlaylist(file);
                             });
        updateTable();
    }
    if (button == &clearPlaylist) {
        tracks.clear();
        updateFilteredTracks(searchTrack.getText());
        updateTable();
    }
}

//==============================================================================

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray &files, int x, int y) {
    for (const auto &filename: files) {
        auto file = juce::File{filename};
        insertUniqueTrack(Track(file));
    }
}

//==============================================================================

void PlaylistComponent::textEditorTextChanged(juce::TextEditor &textEditor) {
    auto text = textEditor.getText();
    updateFilteredTracks(text);
    updateTable();
}

//==============================================================================

void PlaylistComponent::insertUniqueTrack(const Track &newTrack) {
    for (const auto &track: tracks) {
        if (newTrack.getAudioURL() == track.getAudioURL()) {
            return;
        }
    }
    tracks.push_back(newTrack);
    updateFilteredTracks(searchTrack.getText());
    updateTable();
}

int PlaylistComponent::getFirstSelectedRow() {
    if (selectedRows.size() > 0) {
        return selectedRows[0];
    }
    return -1;
}

void PlaylistComponent::updateTable() {
    tableComponent.updateContent();
    tableComponent.repaint();
}

void PlaylistComponent::updateFilteredTracks(const juce::String &text) {
    filteredTracks.clear();
    for (const auto &track: tracks) {
        if (track.getTitle().containsIgnoreCase(text)) {
            filteredTracks.push_back(track);
        }
    }
}

Track PlaylistComponent::getFirstSelectedTrack() {
    auto index = getFirstSelectedRow();
    if (index == -1 || index >= filteredTracks.size()) {
        return {};
    }
    return filteredTracks[index];
}

juce::String PlaylistComponent::createPlaylist() {
    juce::String playlistContents;
    // For every track
    for (const auto &track: tracks) {
        // Stringify the data and add it to the playlist contents
        playlistContents += track.toString() + "\n";
    }
    return playlistContents;
}

void PlaylistComponent::savePlaylist(const juce::File &playlistFile, const juce::String &contents) {
    playlistFile.replaceWithText(contents);
}

void PlaylistComponent::loadPlaylist(const juce::File &playlistFile) {
    // Create an input stream
    std::unique_ptr<juce::FileInputStream> inputStream(playlistFile.createInputStream());
    // While the stream has data to read
    while (!inputStream->isExhausted()) {
        // Read the next line
        juce::String line = inputStream->readNextLine();
        // Tokenize the line into elements delimited by a comma
        juce::StringArray tokens;
        tokens.addTokens(line, ",", "\"");
        // Insert a unique track using the tokens
        insertUniqueTrack(Track{tokens[0], tokens[1], tokens[2].getDoubleValue(), tokens[3]});
    }
}
