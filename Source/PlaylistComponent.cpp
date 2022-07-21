/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 29 Jun 2022 3:21:22pm
    Author:  cristi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    trackTitles.emplace_back("Track 1");
    trackTitles.emplace_back("Track 2");
    trackTitles.emplace_back("Track 3");
    trackTitles.emplace_back("Track 4");
    trackTitles.emplace_back("Track 5");
    trackTitles.emplace_back("Track 6");

    addAndMakeVisible(searchTrack);

    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("No.", 1, 100);
    tableComponent.getHeader().addColumn("Title", 2, 100);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("Audio format", 4, 100);
    // tableComponent.getHeader().addColumn("Playlist", 5, 100);
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
}

PlaylistComponent::~PlaylistComponent()
{
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
    return trackTitles.size();
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
    if (columnId == 1) // No.
    {
        g.drawText(JUCE_TO_STRING(rowNumber), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 2) // Title
    {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 3) // Duration
    {
        g.drawText("1:56", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 4) // Audio format
    {
        g.drawText(".idk", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

juce::Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    return existingComponentToUpdate;

    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto *button = new juce::TextButton{"PLAY"};
            button->addListener(this);
            button->setComponentID(std::to_string(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button)
{
    if (button == &addTracks)
    {
        auto chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...", juce::File{}, "*.mp3");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(chooserFlags, [this](const juce::FileChooser &fc)
                             {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                DBG(file.getFileName());
            } });

        tableComponent.updateContent();
        tableComponent.repaint();
    }
    if (button == &removeTracks)
    {
        auto selectedRows = tableComponent.getSelectedRows();
        for (size_t i = 0; i < tableComponent.getNumSelectedRows(); i++)
        {
            trackTitles.erase(trackTitles.begin() + selectedRows[i]);
        }
        tableComponent.updateContent();
        tableComponent.repaint();
    }
    if (button == &savePlaylist)
    {
        // Write to file
        // Save the file
    }
    if (button == &loadPlaylist)
    {
        // Load file
        // Process info
        // Add new Songs to trackTitles
        tableComponent.updateContent();
        tableComponent.repaint();
    }
    if (button == &clearPlaylist)
    {
        trackTitles.clear();
        tableComponent.updateContent();
        tableComponent.repaint();
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
        trackTitles.push_back(filename.toStdString());
        auto audioURL = juce::URL{juce::File{filename}};
        // TODO need loadURL
        // Update loadURL with new method from DJPlayer
        tableComponent.updateContent();
        tableComponent.repaint();
        return;
    }
}