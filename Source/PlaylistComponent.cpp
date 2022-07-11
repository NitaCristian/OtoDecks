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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // trackTitles.emplace_back("Track 1");
    // trackTitles.emplace_back("Track 2");
    // trackTitles.emplace_back("Track 3");
    // trackTitles.emplace_back("Track 4");
    // trackTitles.emplace_back("Track 5");
    // trackTitles.emplace_back("Track 6");

    tableComponent.getHeader().addColumn("No.", 1, 100);
    tableComponent.getHeader().addColumn("Title", 2, 100);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("Audio format", 4, 100);
    tableComponent.getHeader().addColumn("Playlist", 5, 100);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);

    addAndMakeVisible(addTrack);
    addAndMakeVisible(removeTrack);
    addAndMakeVisible(savePlaylist);
    addAndMakeVisible(loadPlaylist);
    addAndMakeVisible(clearPlaylist);

    addTrack.setButtonText("Add track");
    removeTrack.setButtonText("Remove track");
    savePlaylist.setButtonText("Save playlist");
    loadPlaylist.setButtonText("Load playlist");
    clearPlaylist.setButtonText("Clear playlist");
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics &g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code...
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
               juce::Justification::centred, true); // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto buttonWidth = 100;
    auto buttonHeight = getHeight() / 5;
    tableComponent.setBounds(buttonWidth, 0, getWidth(), getHeight());
    addTrack.setBounds(0, 0, buttonWidth, buttonHeight);
    removeTrack.setBounds(0, buttonHeight, buttonWidth, buttonHeight);
    savePlaylist.setBounds(0, 2 * buttonHeight, buttonWidth, buttonHeight);
    loadPlaylist.setBounds(0, 3 * buttonHeight, buttonWidth, buttonHeight);
    clearPlaylist.setBounds(0, 4 * buttonHeight, buttonWidth, buttonHeight);
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
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
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
            //            juce::String id{std::to_string(rowNumber)};
            button->setComponentID(std::to_string(rowNumber));
            existingComponentToUpdate = button;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button)
{
    DBG(button->getComponentID());
}
