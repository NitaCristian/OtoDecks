#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
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

    std::ifstream fin("OtoDecks.state");
    if (!fin.is_open())
        return;
    std::string filePath;
    while (fin >> filePath)
    {
        insertUniqueTrack(Track(juce::File(filePath)));
    }
    fin.close();
}

PlaylistComponent::~PlaylistComponent()
{
    std::ofstream fout("OtoDecks.state");
    for (const auto &track : tracks)
    {
        fout << track.audioURL.getLocalFile().getFullPathName() << '\n';
    }
    fout.close();
}

void PlaylistComponent::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
}

void PlaylistComponent::resized()
{
    auto area = getLocalBounds();

    auto buttonWidth = getWidth() / 8;
    auto buttonsArea = area.removeFromLeft(buttonWidth);

    auto buttonHeight = getHeight() / 5;
    addTracks.setBounds(buttonsArea.removeFromTop(buttonHeight));
    removeTracks.setBounds(buttonsArea.removeFromTop(buttonHeight));
    savePlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));
    loadPlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));
    clearPlaylist.setBounds(buttonsArea.removeFromTop(buttonHeight));

    searchTrack.setBounds(area.removeFromTop(50));
    tableComponent.setBounds(area);
}

int PlaylistComponent::getNumRows()
{
    return filteredTracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    g.fillAll(juce::Colours::darkgrey);

    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
}

void PlaylistComponent::paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    auto track = filteredTracks[rowNumber];
    auto duration = floor(track.duration / 60 * 100) / 100;

    if (columnId == 1) // No.
    {
        g.drawText(std::to_string(rowNumber + 1), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 2) // Title
    {
        g.drawText(track.title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 3) // Duration
    {
        g.drawText(std::to_string(duration).substr(0, 4), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 4) // Audio format
    {
        g.drawText(track.format, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

juce::Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button)
{
    if (button == &addTracks)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser)
                             {
                                 juce::Array<juce::File> chosenFiles = chooser.getResults();
                                 for (const auto &file : chosenFiles)
                                 {
                                     Track newTrack(file);
                                     insertUniqueTrack(newTrack);
                                 }
                             });
    }
    if (button == &removeTracks)
    {
        auto selectedRows = tableComponent.getSelectedRows();
        for (auto i = 0; i < selectedRows.size(); i++)
        {
            tracks.erase(tracks.begin() + selectedRows[i]);
        }
        updateTable();
    }
    if (button == &savePlaylist)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::saveMode;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser)
                             {
                                 auto newFile = chooser.getResult();
                                 for (const auto &track : tracks)
                                 {
                                     newFile.appendText(track.audioURL.getLocalFile().getFullPathName() + '\n');
                                 }
                                 newFile.create();
                             });
    }
    if (button == &loadPlaylist)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags,
                             [this](const juce::FileChooser &chooser)
                             {
                                 auto file = chooser.getResult();
                                 juce::StringArray lines;
                                 file.readLines(lines);
                                 for (const auto &line : lines)
                                 {
                                     insertUniqueTrack(Track(juce::File(line)));
                                 }
                             });
        updateTable();
    }
    if (button == &clearPlaylist)
    {
        tracks.clear();
        updateTable();
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files)
{
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (const auto &filename : files)
    {
        auto file = juce::File{filename};
        insertUniqueTrack(Track(file));
    }
}

void insertUniqueTrack(const Track &newTrack)
{
    for (const auto &track : tracks)
    {
        if (newTrack.audioURL == track.audioURL)
        {
            return;
        }
    }
    tracks.push_back(newTrack);
    textEditorTextChanged(searchTrack);
    updateTable();
}

void selectedRowsChanged(int row)
{
    selectedRows = tableComponent.getSelectedRows();
}

int getFirstSelectedRow()
{
    if (selectedRows.size() > 0)
    {
        return selectedRows[0];
    }
    return -1;
}

void updateTable()
{
    tableComponent.updateContent();
    tableComponent.repaint();
}

void textEditorTextChanged(juce::TextEditor &textEditor)
{
    filteredTracks.clear();
    auto text = textEditor.getText();
    for (const auto &track : tracks)
    {
        if (track.title.containsIgnoreCase(text))
        {
            filteredTracks.push_back(track);
        }
    }
    updateTable();
}