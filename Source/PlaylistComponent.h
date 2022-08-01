/*
  ==============================================================================

    PlaylistComponent.h
    Created: 29 Jun 2022 3:21:22pm
    Author:  cristi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

  void paint(juce::Graphics &) override;

  void resized() override;

  int getNumRows() override;

  void paintRowBackground(juce::Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;

  void paintCell(juce::Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

  juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

  // From Button::Listener
  void buttonClicked(juce::Button *) override;

  // From FileDragAndDropTarget
  bool isInterestedInFileDrag(const juce::StringArray &files) override;

  void filesDropped(const juce::StringArray &files, int x, int y) override;

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

  juce::FileChooser fChooser{"Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.mp3"};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
