/*
  ==============================================================================

    PlaybackPosition.h
    Created: 6 Nov 2023 2:04:33pm
    Author:  Squanch Audio

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PlayBackPosition : public juce::Component, private juce::Timer, public juce::RelativeTime
{
public:

    PlayBackPosition(AudioTransportSource& _transportSource);
        
   
    ~PlayBackPosition();
   
    void resized();
    

    void paint(juce::Graphics& g) override;
   

    void mouseDown(const juce::MouseEvent& event);
   
    void pausePlayback();
   
    void resumePlayback();
    
   

private:

    void timerCallback() override;
    
   
    Label timeLabel;
    AudioTransportSource& transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayBackPosition)

};
