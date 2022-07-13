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

    addAndMakeVisible(addTrack);
    addTrack.setButtonText("Add track");

    addAndMakeVisible(removeTrack);
    removeTrack.setButtonText("Remove track");

    addAndMakeVisible(savePlaylist);
    savePlaylist.setButtonText("Save playlist");

    addAndMakeVisible(loadPlaylist);
    loadPlaylist.setButtonText("Load playlist");

    addAndMakeVisible(clearPlaylist);
    clearPlaylist.setButtonText("Clear playlist");
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
    addTrack.setBounds(buttonsArea.removeFromTop(buttonHeight));
    removeTrack.setBounds(buttonsArea.removeFromTop(buttonHeight));
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
    if (button == &addTrack)
    {
    }
    DBG(button->getComponentID());
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