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
    addAndMakeVisible(volSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    volSlider.addListener(this);
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
    juce::URL audioURL{"file:///home/cristi/aon_inspired.mp3"};
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
        transportSource.start();
    }
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    transportSource.getNextAudioBlock(bufferToFill);
    // Your audio-processing code goes here!

//    auto *leftChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
//    auto *rightChan = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
//    for (auto i = 0; i < bufferToFill.numSamples; ++i) {
////        auto sample = rand.nextFloat() * 0.2f;
////        auto sample = fmod(phase, 0.2);
//        auto sample = sin(phase);
//        leftChan[i] = sample;
//        rightChan[i] = sample;
//        phase += dphase;
//    }
    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    // bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    transportSource.releaseResources();
    // For more details, see the help for AudioProcessor::releaseResources()
}


void MainComponent::buttonClicked(juce::Button *button) {
    std::cout << "Button was clicked: " << button->getName() << "\n";
}


void MainComponent::sliderValueChanged(juce::Slider *slider) {
    std::cout << "Slider was changed: " << slider->getValue() << "\n";
    dphase = slider->getValue() * 0.001;
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
}
