/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             AudioThumbnailTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Plays audio files and displays a thumbnail.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

#include <JuceHeader.h>
#include "Waveform.h"
#include "TransportControl.h"
//#include "PlaybackPosition.h"



//==============================================================================
class MainContentComponent : public juce::AudioAppComponent,
    private juce::ChangeListener

{
public:
    MainContentComponent();


    ~MainContentComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    void resized() override;


    void changeListenerCallback(juce::ChangeBroadcaster* source) override;


private:
    /*enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Pausing,
        Paused
    };
    */

    //void changeState(TransportState newState);


    void transportSourceChanged();



    void openButtonClicked();

    /*

    void updateToggleState(juce::Button* button, juce::String name);


    void setLoopingState(bool shouldLoop);


    void playButtonClicked();

    void stopButtonClicked();
    */



    //==========================================================================
    juce::TextButton openButton;
    /* juce::TextButton playButton;
     juce::TextButton stopButton;

     juce::ToggleButton loopingToggle;
     juce::Label toggleLabel;
     */

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;                    // [3]
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportControl transportControl;
    //TransportState state;
    juce::AudioThumbnailCache thumbnailCache;                  // [1]
    WaveForm waveForm;                      // [2]

    //PlayBackPosition playbackPosition;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
