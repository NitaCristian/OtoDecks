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

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    phase = 0;
    dphase = 0.0001;

    formatManager.registerBasicFormats();

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::loadURL(juce::URL audioURL) {
//    juce::URL audioURL{"file:///home/cristi/aon_inspired.mp3"};
    // Convert the autoURL to an input stream

//    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    auto *reader = formatManager.createReaderFor(
            audioURL.createInputStream(juce::URL::InputStreamOptions{juce::URL::ParameterHandling::inAddress}));
    // if the file is good
    if (reader != nullptr) {
        // create a source
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        // pass to transport source
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        // if something goes wrong
        readerSource.reset(newSource.release());
        // start the transport source
    }
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    transportSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    transportSource.releaseResources();
    // For more details, see the help for AudioProcessor::releaseResources()
}


void MainComponent::buttonClicked(juce::Button *button) {
    std::cout << "Button was clicked: " << button->getName() << "\n";
    if (button == &playButton) {
        transportSource.start();

    }
    if (button == &stopButton) {
        transportSource.stop();
    }
    if (button == &loadButton) {
        juce::FileChooser chooser("Select file");
        if (chooser.browseForFileToOpen()) {
            loadURL(juce::URL{chooser.getResult()});
        }
    }
}


void MainComponent::sliderValueChanged(juce::Slider *slider) {
    std::cout << "Slider was changed: " << slider->getValue() << "\n";
//    dphase = slider->getValue() * 0.001;
    if (slider == &volSlider) {
        transportSource.setGain(slider->getValue());
    }
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized() {
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto rowHeight = getHeight() / 5;
    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    volSlider.setBounds(0, 2 * rowHeight, getWidth(), getHeight() / 5);
    loadButton.setBounds(0, 3 * rowHeight, getWidth(), rowHeight);
}
