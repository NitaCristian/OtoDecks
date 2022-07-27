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
    juce::AudioFormatManager formatManager; // TODO make global
    formatManager.registerBasicFormats();

    if (auto reader = formatManager.createReaderFor(audioFile))
    {
      auto lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
      delete reader;
      return lengthInSeconds;
    }

    // if (auto *reader = audioFormatReaderSource->getAudioFormatReader())
    // double lengthInSeconds = static_cast<double>(audioFormatReaderSource->getTotalLength()) / reader->sampleRate;

    return -1;
  }
};

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
  }

  void selectedRowsChanged(int row)
  {
    if (row == -1)
      return;
    juce::SparseSet<int> selectedRows = tableComponent.getSelectedRows();
    selectrows = selectedRows;
  }

  std::vector<Track> tracks;
  juce::SparseSet<int> selectrows;

private:
  juce::TableListBox tableComponent;

  juce::TextButton addTracks;
  juce::TextButton removeTracks;
  juce::TextButton savePlaylist;
  juce::TextButton loadPlaylist;
  juce::TextButton clearPlaylist;

  // TODO - Search bar -> TextEditor needs listener
  juce::TextEditor searchTrack;

  juce::FileChooser fChooser{"Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.mp3"};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
