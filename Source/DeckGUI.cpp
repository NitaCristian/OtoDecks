#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer *_player, juce::AudioFormatManager &formatManagerToUse,
                 juce::AudioThumbnailCache &cacheToUse, PlaylistComponent *playlistComponent)
        : djAudioPlayer{_player},
          waveformDisplay(formatManagerToUse, cacheToUse, _player),
          playlist(playlistComponent) {
    // TODO - REFACTOR
    // 1. Remove unused elements
    // 2. Introduce methods that initialize components, eg. initButton(button, text), initSlider(slider, min, max, val, style)
    // 3. Clean up the initializer list
    setLookAndFeel(&customLookAndFeel);

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

    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(1.0);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    // gainLabel.attachToComponent(&gainSlider, true);

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(1.0, 5.0);
    speedSlider.setValue(1.0);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    // speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(bassSlider);
    bassSlider.addListener(this);
    bassSlider.setRange(50.0, 20000.0);
    bassSlider.setValue(20000.0);
    bassSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    bassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(bassLabel);
    bassLabel.setText("Bass", juce::dontSendNotification);
    bassLabel.attachToComponent(&bassSlider, false);

    addAndMakeVisible(trebleSlider);
    trebleSlider.addListener(this);
    trebleSlider.setRange(50.0, 20000.0);
    trebleSlider.setValue(50.0);
    trebleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    trebleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(trebleLabel);
    trebleLabel.setText("Treble", juce::dontSendNotification);

    addAndMakeVisible(trackName);
    trackName.setText("No track", juce::dontSendNotification);
    trackName.setFont(juce::Font(32.0f));

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
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour::fromRGB(16, 7, 32));

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

    auto sliders = area.removeFromTop(4 * rowHeight);
    bassSlider.setBounds(sliders.removeFromLeft(width).reduced(10));
    trebleSlider.setBounds(sliders.removeFromLeft(width));
    gainSlider.setBounds(sliders.removeFromLeft(width).reduced(10, 0));
    speedSlider.setBounds(sliders.removeFromLeft(width));

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
    auto file = juce::File{files[0]};
    auto audioURL = juce::URL{file};
    djAudioPlayer->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    playlist->insertUniqueTrack(Track(file));
}

//==============================================================================

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
