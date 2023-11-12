/*
  ==============================================================================

    TransportControl.cpp
    Created: 10 Nov 2023 2:32:44pm
    Author:  09sam

  ==============================================================================
*/

#include "TransportControl.h"
//Consrtuctor used to pass ref to transport source
TransportControl::TransportControl(juce::AudioTransportSource& _transportSource, std::unique_ptr<juce::AudioFormatReaderSource>& const _readerSource)
    : transportSource(_transportSource), state(Stopped), playbackPosition(transportSource), readerSource(_readerSource)
{
    

    addAndMakeVisible(&playButton);
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    stopButton.setEnabled(false);

    addAndMakeVisible(&fastForwardButton);
    fastForwardButton.onClick = [this] { fastForwardButtonClicked(); };
    fastForwardButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    fastForwardButton.setEnabled(false);

    addAndMakeVisible(&rewindButton);
    rewindButton.onClick = [this] { rewindButtonClicked(); };
    rewindButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    rewindButton.setEnabled(false);

    addAndMakeVisible(&nextSongButton);
   // rewindButton.onClick = [this] { rewindButtonClicked(); };
    nextSongButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    nextSongButton.setEnabled(false);

    addAndMakeVisible(&previousSongButton);
    // rewindButton.onClick = [this] { rewindButtonClicked(); };
    previousSongButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    previousSongButton.setEnabled(false);

    stopButton.setLookAndFeel(&stopButtonLookandFeel);
    playButton.setLookAndFeel(&otherLookandFeel);
    fastForwardButton.setLookAndFeel(&fastForwardButtonLookAndFeel);
    rewindButton.setLookAndFeel(&rewindButtonLookAndFeel);
    nextSongButton.setLookAndFeel(&nextSongButtonLookAndFeel);
    previousSongButton.setLookAndFeel(&previousSongLookAndFeel);

    addAndMakeVisible(playbackPosition);

    addAndMakeVisible(&loopingToggle);
    loopingToggle.onClick = [this] {updateToggleState(&loopingToggle, "Looping Toggle"); }; //Create an onclick event through a lamba function so we can get the state of button when clicked and dictate behaviour

    addAndMakeVisible(&toggleLabel);
    toggleLabel.setText("Looping:", juce::dontSendNotification);
    toggleLabel.attachToComponent(&loopingToggle, true);
    toggleLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    toggleLabel.setJustificationType(juce::Justification::right);

}

TransportControl::~TransportControl()
{
    setLookAndFeel(nullptr);
    stopTimer();
}

void TransportControl::changeState(TransportState newState)
{


    if (state != newState)
    {
        state = newState;
       

        switch (state)
        {
        case Stopped:                           // [3]
            stopButton.setEnabled(false);
            rewindButton.setEnabled(false);
            fastForwardButton.setEnabled(false);
            transportSource.setPosition(0.0);
            playButton.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
            stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
      
            stopTimer();
            break;

        case Starting:                          // [4]
            transportSource.start();
            playbackPosition.resumePlayback();
            rewindButton.setEnabled(true);
            fastForwardButton.setEnabled(true);
            stopTimer();
            break;

        case Playing:
            playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
            stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            fastForwardButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            rewindButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            stopButton.setEnabled(true);
            stopTimer();

            break;

        case Stopping:                          // [6]
            transportSource.stop();
            playbackPosition.pausePlayback();
            fastForwardButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            rewindButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            stopTimer();
            break;

        case Pausing:
            transportSource.stop();
            stopTimer();
            break;

        case Paused:
              playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
              stopTimer();
            break;

        case FastFoward:
            startTimer(250);
            fastForwardButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
            rewindButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
            break;

        case Rewind:
            startTimer(250);
            rewindButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
            fastForwardButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);

            break;

        default:
            jassertfalse;
            break;

        }

    }
}

void TransportControl::resized()
{
    rewindButton.setBounds(0, 0, (getWidth() * 10) / 100, 20);
    playButton.setBounds(70, 0, (getWidth() * 10) / 100, 20);
    stopButton.setBounds(110, 0, (getWidth() * 10) / 100, 20);
    fastForwardButton.setBounds(150, 0, (getWidth() * 10) / 100, 20);
    nextSongButton.setBounds(210, 0, (getWidth() * 10) / 100, 20);
    previousSongButton.setBounds(300, 0, (getWidth() * 10) / 100, 20);
    //rewindButton.setBounds(170, 0, (getWidth() * 10) / 100, 20);

    loopingToggle.setBounds(100, getHeight() - 100, 40, 20);
    juce::Rectangle<int> thumbnailBounds(0, 50, getParentWidth() - 20, getHeight() - 200);
    playbackPosition.setBounds(thumbnailBounds);
}

TransportState TransportControl::GetTransportState()
{
    return state;
}

void TransportControl::FileLoaded()
{
    playButton.setEnabled(true);
    fastForwardButton.setEnabled(true);
    rewindButton.setEnabled(true);

    changeState(Starting);
}




void TransportControl::updateToggleState(juce::Button* button, juce::String name)
{
    auto state = button->getToggleState();


    juce::String stateString = state ? "ON" : "OFF";

    juce::Logger::outputDebugString(name + " Button changed to " + stateString);

    setLoopingState(state);

}

void TransportControl::setLoopingState(bool shouldLoop)
{
    if (readerSource.get() != nullptr)
        readerSource->setLooping(shouldLoop);
}


void TransportControl::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    if (state == FastFoward || state == Rewind)
        changeState(Playing);
    else if (state == Playing) 
        changeState(Pausing);
}

void TransportControl::stopButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void TransportControl::rewindButtonClicked()
{
    changeState(Rewind);
}

void TransportControl::fastForwardButtonClicked()
{
    changeState(FastFoward);
}

void TransportControl::timerCallback()
{
    switch (state)
    {
    case FastFoward:
        transportSource.setPosition(transportSource.getCurrentPosition() + 1);
        break;

    case Rewind:
        transportSource.setPosition(transportSource.getCurrentPosition() - 1); 
        break;
    }
}