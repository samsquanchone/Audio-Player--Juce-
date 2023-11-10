/*
  ==============================================================================

    TransportControl.h
    Created: 10 Nov 2023 2:32:44pm
    Author:  09sam

  ==============================================================================
*/

#pragma once
#pragma once
#include <JuceHeader.h>
#include "PlaybackPosition.h"

enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
    Pausing,
    Paused
};
class TransportControl : public juce::Component
{
public:
    TransportState state;
    //Consrtuctor used to pass ref to transport source
    TransportControl(juce::AudioTransportSource& _transportSource, std::unique_ptr<juce::AudioFormatReaderSource>& const _readerSource)
        : transportSource(_transportSource), state(Stopped), playbackPosition(transportSource), readerSource(_readerSource)
    {
        /*  addAndMakeVisible(&openButton);
          openButton.setButtonText("Open...");
          openButton.onClick = [this] { openButtonClicked(); };
          */

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

        addAndMakeVisible(playbackPosition);


    }

    void changeState(TransportState newState)
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

    void resized() override
    {
        playButton.setBounds(10, 40, getWidth() - 20, 20);
        stopButton.setBounds(10, 70, getWidth() - 20, 20);

        loopingToggle.setBounds(100, getHeight() - 20, 40, 20);
        juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);
        playbackPosition.setBounds(thumbnailBounds);
    }

    void FileLoaded()
    {
        playButton.setEnabled(true);
    }


private:

    void updateToggleState(juce::Button* button, juce::String name)
    {
        auto state = button->getToggleState();


        juce::String stateString = state ? "ON" : "OFF";

        juce::Logger::outputDebugString(name + " Button changed to " + stateString);

        setLoopingState(state);

    }

    void setLoopingState(bool shouldLoop)
    {
        if (readerSource.get() != nullptr)
            readerSource->setLooping(shouldLoop);
    }


    void playButtonClicked()
    {
        if ((state == Stopped) || (state == Paused))
            changeState(Starting);
        else if (state == Playing)
            changeState(Pausing);
    }

    void stopButtonClicked()
    {
        if (state == Paused)
            changeState(Stopped);
        else
            changeState(Stopping);
    }

    std::unique_ptr<juce::AudioFormatReaderSource>& readerSource;
    juce::AudioTransportSource& transportSource;
    //==========================================================================
   // juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    juce::ToggleButton loopingToggle;
    juce::Label toggleLabel;


    PlayBackPosition playbackPosition;


};