/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 6 Nov 2023 2:31:32pm
    Author:  Squanch Audio

  ==============================================================================
*/

#include "AudioPlayer.h"

//==============================================================================

MainContentComponent::MainContentComponent()
    : //state(Stopped),
    thumbnailCache(5),                            // [4]
    waveForm(512, formatManager, thumbnailCache), // [5]
   // playbackPosition(transportSource),
    transportControl(transportSource, readerSource)
{

    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };
    /*
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    addAndMakeVisible(&loopingToggle);
    loopingToggle.onClick = [this] {updateToggleState(&loopingToggle, "Looping Toggle"); }; //Create an onclick event through a lamba function so we can get the state of button when clicked and dictate behaviour

    addAndMakeVisible(&toggleLabel);
    toggleLabel.setText("Looping:", juce::dontSendNotification);
    toggleLabel.attachToComponent(&loopingToggle, true);
    toggleLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    toggleLabel.setJustificationType(juce::Justification::right);
    */

    addAndMakeVisible(waveForm);
    addAndMakeVisible(transportControl);
    //addAndMakeVisible(playbackPosition);

    setSize(600, 400);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);


    setAudioChannels(2, 2);


}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainContentComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
        bufferToFill.clearActiveBufferRegion();
    else
        transportSource.getNextAudioBlock(bufferToFill);
}

void MainContentComponent::releaseResources()
{
    transportSource.releaseResources();
}

void MainContentComponent::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    /*  playButton.setBounds(10, 40, getWidth() - 20, 20);
      stopButton.setBounds(10, 70, getWidth() - 20, 20);

      loopingToggle.setBounds(100, getHeight() - 20, 40, 20);

      */

    juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);


    waveForm.setBounds(thumbnailBounds);
    transportControl.setBounds(thumbnailBounds);
    // playbackPosition.setBounds(thumbnailBounds);
}

void MainContentComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        //Note if we get this refactoring to work, maybe use a getter, so we can keep state private!
        if (transportSource.isPlaying())
            transportControl.changeState(Playing);
        else if ((transportControl.state == Stopping) || (transportControl.state == Playing))
            transportControl.changeState(Stopped);
        else if (Pausing == transportControl.state)
            transportControl.changeState(Paused);
    }
}



/*
void MainContentComponent::changeState(TransportState newState)
{


    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:                           // [3]
            stopButton.setEnabled(false);
            transportSource.setPosition(0.0);
            playButton.setButtonText("Play");
            stopButton.setButtonText("Stop");  // [5]
            break;

        case Starting:                          // [4]
            transportSource.start();
            playbackPosition.resumePlayback();
            break;

        case Playing:
            playButton.setButtonText("Pause");
            stopButton.setButtonText("Stop");  // [5]
            stopButton.setEnabled(true);

            break;

        case Stopping:                          // [6]
            transportSource.stop();
            playbackPosition.pausePlayback();
            break;

        case Pausing:
            transportSource.stop();
            break;

        case Paused:
            playButton.setButtonText("Resume");
            stopButton.setButtonText("Restart");

            break;



        default:
            jassertfalse;

            break;

        }
    }
}
*/
void MainContentComponent::transportSourceChanged()
{
    transportControl.changeState(transportSource.isPlaying() ? Playing : Stopped);
}


void MainContentComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select an audio file to play... (wav, mp3, aif and aiff supported)",
        juce::File{},
        "*.wav; *.mp3; *aif; *aiff*");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                auto* reader = formatManager.createReaderFor(file);

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                    transportControl.FileLoaded();
                    //playButton.setEnabled(true);
                    waveForm.setFile(file);                            // [7]
                    readerSource.reset(newSource.release());
                    transportSourceChanged();

                }
            }
        });
}

/*
void MainContentComponent::updateToggleState(juce::Button* button, juce::String name)
{
    auto state = button->getToggleState();


    juce::String stateString = state ? "ON" : "OFF";

    juce::Logger::outputDebugString(name + " Button changed to " + stateString);

    setLoopingState(state);

}

void MainContentComponent::setLoopingState(bool shouldLoop)
{
    if (readerSource.get() != nullptr)
        readerSource->setLooping(shouldLoop);
}


void MainContentComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
}

void MainContentComponent::stopButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}
*/