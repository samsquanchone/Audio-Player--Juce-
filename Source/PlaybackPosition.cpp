/*
  ==============================================================================

    PlaybackPosition.cpp
    Created: 6 Nov 2023 2:04:33pm
    Author:  Squanch Audio

  ==============================================================================
*/

#include "PlaybackPosition.h"


PlayBackPosition::PlayBackPosition(AudioTransportSource& _transportSource)
    : transportSource(_transportSource)
{
    addAndMakeVisible(&timeLabel);
    timeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    timeLabel.setText("Time:", juce::dontSendNotification);
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    timeLabel.setJustificationType(juce::Justification::centred);

    timeLabel.setInterceptsMouseClicks(false, true);

    startTimer(40);
}

PlayBackPosition::~PlayBackPosition()
{
    stopTimer();
}

void PlayBackPosition::resized()
{
    timeLabel.setBounds(10, 50, getWidth() - 20, getHeight());
}


void PlayBackPosition::paint(juce::Graphics& g)
{
    auto audioLength = (float)transportSource.getLengthInSeconds();


    if (audioLength > 0)
    {
        auto audioPosition = (float)transportSource.getCurrentPosition();
        auto drawPosition = (audioPosition / audioLength) * (float)getWidth();                            // [13]

        g.setColour(juce::Colours::green);
        g.drawLine(drawPosition, 0.0f, drawPosition,
            (float)getHeight());

        g.setColour(juce::Colour(0.0f, 0.0f, 0.0f, 0.3f));
        g.fillRect(0.0f, 0.0f, drawPosition, (float)getHeight());
    }

}

void PlayBackPosition::mouseDown(const juce::MouseEvent& event)
{
    auto duration = transportSource.getLengthInSeconds();

    if (transportSource.getTotalLength() > 0)
    {
        auto clickPosition = event.position.x;
        auto audioPosition = (clickPosition / (float)getWidth()) * duration;

        transportSource.setPosition(audioPosition);
    }
}

void PlayBackPosition::pausePlayback()
{
    stopTimer();
}

void PlayBackPosition::resumePlayback()
{
    startTimer(40);
}


void PlayBackPosition::timerCallback()
{
    auto rawTime = juce::RelativeTime::seconds(transportSource.getCurrentPosition());

    auto tSeconds = (int)rawTime.inSeconds() % 60;
    auto tMinutes = (int)rawTime.inMinutes() % 60;
    auto tMs = (int)rawTime.inMilliseconds() % 1000;
    auto tHours = (int)rawTime.inHours();
    auto time = "Time: " + juce::String(tHours) + ":" + juce::String(tMinutes) + ":" + juce::String(tSeconds) + ":" + juce::String(tMs);

    timeLabel.setText(time, juce::dontSendNotification);
    repaint();
}