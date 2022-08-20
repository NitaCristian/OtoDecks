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

    addAndMakeVisible(savePlaylist);
    savePlaylist.setButtonText("Save playlist");
    savePlaylist.addListener(this);

    addAndMakeVisible(loadPlaylist);
    loadPlaylist.setButtonText("Load playlist");
    loadPlaylist.addListener(this);

    addAndMakeVisible(clearPlaylist);
    clearPlaylist.setButtonText("Clear playlist");
    clearPlaylist.addListener(this);

    getPlaylistState();
}

PlaylistComponent::~PlaylistComponent() {
    setLookAndFeel(nullptr);
    savePlaylistState();
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
    auto buttonsArea = area.removeFromLeft(buttonWidth);
    // Set the height of one button
    auto buttonHeight = getHeight() / 5;

    addTracks.setBounds(buttonsArea.removeFromTop(buttonHeight));
    removeTracks.setBounds(buttonsArea.removeFromTop(buttonHeight));
    savePlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));
    loadPlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));
    clearPlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));

    searchTrack.setBounds(area.removeFromTop(50));
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
        g.fillAll(juce::Colours::orange);
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

//==============================================================================

// TODO - Refactor this section
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
        auto selectedRows = tableComponent.getSelectedRows();
        for (auto i = 0; i < selectedRows.size(); i++) {
            tracks.erase(tracks.begin() + selectedRows[i]);
        }
        updateTable();
    }
    if (button == &savePlaylist) {
        auto fileChooserFlags = juce::FileBrowserComponent::saveMode;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser) {
                                 auto newFile = chooser.getResult();
                                 for (const auto &track: tracks) {
                                     newFile.appendText(track.toString());
                                 }
                                 newFile.create();
                             });
    }
    if (button == &loadPlaylist) {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser) {
                                 auto file = chooser.getResult();
                                 juce::StringArray lines;
                                 file.readLines(lines);
                                 for (const auto &line: lines) {
                                     insertUniqueTrack(Track(juce::File(line)));
                                 }
                             });
        updateTable();
    }
    if (button == &clearPlaylist) {
        tracks.clear();
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
    filteredTracks.clear();
    auto text = textEditor.getText();
    for (const auto &track: tracks) {
        if (track.getTitle().containsIgnoreCase(text)) {
            filteredTracks.push_back(track);
        }
    }
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
    textEditorTextChanged(searchTrack);
    updateTable();
}

void PlaylistComponent::selectedRowsChanged(int row) {
    selectedRows = tableComponent.getSelectedRows();
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

void PlaylistComponent::savePlaylistState() {
    juce::File stateFile = juce::File::getCurrentWorkingDirectory().getChildFile("OtoDecks.state");

    if (!stateFile.existsAsFile()) {
        stateFile.create();
    }

    for (const auto &track: tracks) {
        stateFile.appendText(track.toString() + '\n');
    }
}

void PlaylistComponent::getPlaylistState() {
    juce::File stateFile = juce::File::getCurrentWorkingDirectory().getChildFile("OtoDecks.state");

    if (!stateFile.existsAsFile()) {
        return;
    }

    juce::StringArray lines;
    stateFile.readLines(lines);

    for (const auto &line: lines) {
        // Tokenize line
        // insertUniqueTrack(Track{title,});
    }
}