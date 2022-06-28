#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio)) {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                                          [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    } else {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    player1.releaseResources();
}

void MainComponent::buttonClicked(juce::Button *button) {
    if (button == &playButton) {
        player1.start();
    }
    if (button == &stopButton) {
        player1.stop();
    }
    if (button == &loadButton) {
        juce::FileChooser chooser("Select file");
        if (chooser.browseForFileToOpen()) {
            player1.loadURL(juce::URL{chooser.getResult()});
        }
    }
}

void MainComponent::sliderValueChanged(juce::Slider *slider) {
    if (slider == &volSlider) {
        player1.setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player1.setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        player1.setPositionRelative(slider->getValue());
    }
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {
    auto rowHeight = getHeight() / 6;
    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    volSlider.setBounds(0, 2 * rowHeight, getWidth(), getHeight() / 5);
    speedSlider.setBounds(0, 3 * rowHeight, getWidth(), getHeight() / 5);
    posSlider.setBounds(0, 4 * rowHeight, getWidth(), getHeight() / 5);
    loadButton.setBounds(0, 5 * rowHeight, getWidth(), rowHeight);
}
