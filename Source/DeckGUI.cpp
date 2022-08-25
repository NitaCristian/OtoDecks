#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer *player, juce::AudioFormatManager &formatManager, juce::AudioThumbnailCache &cache, PlaylistComponent *playlistComponent,
                 juce::Colour colour)
        : djAudioPlayer{player},
          waveformDisplay(formatManager, cache, player, colour),
          playlist(playlistComponent),
          customLookAndFeel{colour} {
    // General LookAndFeel
    setLookAndFeel(&customLookAndFeel);

    // Initialize Buttons
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setButtonText("Play");

    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    pauseButton.setButtonText("Pause");

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setButtonText("Stop");

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setButtonText("Load");

    // Initialize Sliders
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setName("Gain");
    gainSlider.setRange(0.0, 1.0, 0.01);
    gainSlider.setValue(1.0);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, gainSlider.getTextBoxWidth(), gainSlider.getTextBoxHeight());

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setName("Speed");
    speedSlider.setRange(0.25, 2.0, 0.25);
    speedSlider.setValue(1.0);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, speedSlider.getTextBoxWidth(), speedSlider.getTextBoxHeight());

    addAndMakeVisible(bassSlider);
    bassSlider.addListener(this);
    bassSlider.setName("Bass");
    bassSlider.setRange(50.0, 10000.0);
    bassSlider.setValue(10000.0);
    bassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    bassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, bassSlider.getTextBoxWidth(), bassSlider.getTextBoxHeight());

    addAndMakeVisible(trebleSlider);
    trebleSlider.addListener(this);
    trebleSlider.setName("Treble");
    trebleSlider.setRange(50.0, 5000.0);
    trebleSlider.setValue(50.0);
    trebleSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trebleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, trebleSlider.getTextBoxWidth(), trebleSlider.getTextBoxHeight());

    // Initialize Track Name
    addAndMakeVisible(trackName);
    trackName.setText("No track", juce::dontSendNotification);
    trackName.setFont(juce::Font(32.0f));

    // Initialize Waveform
    addAndMakeVisible(waveformDisplay);

    startTimer(500);
}

DeckGUI::~DeckGUI() {
    setLookAndFeel(nullptr);
    stopTimer();
}

//==============================================================================

void DeckGUI::paint(juce::Graphics &g) {
    // Clear the background
//    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);

    // Draw an outline around the component
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

void DeckGUI::resized() {
    auto area = getLocalBounds();
    auto rowHeight = getHeight() / 8;

    trackName.setBounds(area.removeFromTop(rowHeight));
    waveformDisplay.setBounds(area.removeFromTop(2 * rowHeight));

    auto width = getWidth() / 4;

    auto sliderArea = area.removeFromTop(4 * rowHeight);
    bassSlider.setBounds(sliderArea.removeFromLeft(width).reduced(10));
    trebleSlider.setBounds(sliderArea.removeFromLeft(width).reduced(10));
    gainSlider.setBounds(sliderArea.removeFromLeft(width).reduced(15, 5));
    speedSlider.setBounds(sliderArea.removeFromLeft(width).reduced(15, 5));

    auto buttonArea = area.removeFromTop(rowHeight);
    playButton.setBounds(buttonArea.removeFromLeft(width));
    pauseButton.setBounds(buttonArea.removeFromLeft(width));
    stopButton.setBounds(buttonArea.removeFromLeft(width));
    loadButton.setBounds(buttonArea.removeFromLeft(width));
}

//==============================================================================

void DeckGUI::buttonClicked(juce::Button *button) {
    if (button == &playButton) {
        djAudioPlayer->start();
    }
    if (button == &pauseButton) {
        djAudioPlayer->pause();
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
    }
    if (button == &loadButton) {
        auto track = playlist->getFirstSelectedTrack();
        // If a track couldn't be loaded, just exit
        if (track.getDuration() == -1) {
            return;
        }
        auto audioURL = track.getAudioURL();
        this->djAudioPlayer->loadURL(audioURL);
        this->waveformDisplay.loadURL(audioURL);
        this->trackName.setText(track.getTitle(), juce::dontSendNotification);
    }
}

//==============================================================================

void DeckGUI::sliderValueChanged(juce::Slider *slider) {
    if (slider == &gainSlider) {
        djAudioPlayer->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        djAudioPlayer->setSpeed(slider->getValue());
    }
    if (slider == &bassSlider) {
        djAudioPlayer->setBass(slider->getValue());
    }
    if (slider == &trebleSlider) {
        djAudioPlayer->setTreble(slider->getValue());
    }
}

//==============================================================================

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files) {
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y) {

    for (auto i = 0; i < files.size(); i++) {
        auto file = juce::File{files[i]};
        auto audioURL = juce::URL{file};
        if (i == 0) {
            djAudioPlayer->loadURL(audioURL);
            waveformDisplay.loadURL(audioURL);
        }
        playlist->insertUniqueTrack(Track(file));
    }
}

//==============================================================================

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
