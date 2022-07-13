/*
  ==============================================================================

    PlaylistComponent.h
    Created: 29 Jun 2022 3:21:22pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener, public juce::FileDragAndDropTarget
{
public:
  PlaylistComponent();

  ~PlaylistComponent() override;

  void paint(juce::Graphics &) override;

  void resized() override;

  int getNumRows() override;

  void paintRowBackground(juce::Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;

  void paintCell(juce::Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

  juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                           Component *existingComponentToUpdate) override;

  // From Button::Listener
  void buttonClicked(juce::Button *) override;

  // From FileDragAndDropTarget
  bool isInterestedInFileDrag(const juce::StringArray &files) override;

  void filesDropped(const juce::StringArray &files, int x, int y) override;

private:
  juce::TableListBox tableComponent;
  std::vector<std::string> trackTitles;

  juce::TextButton addTrack;
  juce::TextButton removeTrack;
  juce::TextButton savePlaylist;
  juce::TextButton loadPlaylist;
  juce::TextButton clearPlaylist;

  // Search barb -> TextEditor needs listener
  juce::TextEditor searchTrack;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
