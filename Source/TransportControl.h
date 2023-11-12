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
#include "WaveformControls.h"

enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
    Pausing,
    Paused,
    FastFoward,
    Rewind
};


class TransportControl : public juce::Component, private juce::Timer
{
public:
    
    //Consrtuctor used to pass ref to transport source
    TransportControl(juce::AudioTransportSource& _transportSource, std::unique_ptr<juce::AudioFormatReaderSource>& const _readerSource);

    ~TransportControl();
     
    void changeState(TransportState newState);
    

    void resized() override;
   
    TransportState GetTransportState();
  

    void FileLoaded();
 


private:

    void updateToggleState(juce::Button* button, juce::String name);
    

    void setLoopingState(bool shouldLoop);
  
    void playButtonClicked();
   
    void stopButtonClicked();

    void rewindButtonClicked();
    
    void fastForwardButtonClicked();

    void timerCallback() override;
    

    std::unique_ptr<juce::AudioFormatReaderSource>& readerSource;
    juce::AudioTransportSource& transportSource;
    //==========================================================================
   // juce::TextButton openButton;

    CustomButtons otherLookandFeel;
    StopButton stopButtonLookandFeel;
    FastForwardButton fastForwardButtonLookAndFeel;
    RewindButton rewindButtonLookAndFeel;
    MoveToNextSongButton nextSongButtonLookAndFeel;
    MoveToPreviousSongButton previousSongLookAndFeel;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton fastForwardButton;
    juce::TextButton rewindButton;
    juce::TextButton nextSongButton;
    juce::TextButton previousSongButton;
    TransportState state;
    juce::ToggleButton loopingToggle;
    juce::Label toggleLabel;


    PlayBackPosition playbackPosition;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportControl)
};