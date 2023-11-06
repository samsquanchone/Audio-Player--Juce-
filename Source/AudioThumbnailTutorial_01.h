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


class WaveForm : public juce::Component, private juce::ChangeListener
{
public:

    //Constructor arguments are what are required to initialise a audiothumbnail
    WaveForm(int samples, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbnailCache)

        : thumbnail(samples, formatManager, thumbnailCache)
    {

        //Set up observer 
        thumbnail.addChangeListener(this);

        addAndMakeVisible(&fileNameLabel);
        fileNameLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        fileNameLabel.setText("Filename:", juce::dontSendNotification);
        fileNameLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
        fileNameLabel.setJustificationType(juce::Justification::centred);

    }

    ~WaveForm()
    {
        //Remove observer
        thumbnail.removeChangeListener(this);
    }

    void resized() override
    {
        fileNameLabel.setBounds(10, -120, getWidth() - 20, getHeight());

    }
    void paint(juce::Graphics& g) override
    {
        juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded(g);
        else
            paintIfFileLoaded(g);
    }


    //When a file is selected from main component it will be passed here to set the thumbnail source (const used as it is a large file)
    void setFile(const juce::File& file)
    {
        thumbnail.setSource(new juce::FileInputSource(file));
        fileNameLabel.setText((juce::String)file.getFileNameWithoutExtension(), juce::dontSendNotification);
    }

    void paintIfNoFileLoaded(juce::Graphics& g)
    {

        g.fillAll(juce::Colours::darkgrey);
        g.setColour(juce::Colours::white);
        g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
    }

    void paintIfFileLoaded(juce::Graphics& g)
    {
        g.fillAll(juce::Colours::darkgrey);

        g.setColour(juce::Colours::white);

        g.setColour(juce::Colours::darkslateblue);                               // [8]


        thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);

        g.setColour(juce::Colours::green);


    }

    void paintMonoIfFileLoaded(juce::Graphics& g)
    {
        g.fillAll(juce::Colours::darkgrey);
        g.setColour(juce::Colours::red);

        thumbnail.drawChannel(g,
            getLocalBounds(),
            0.0,                                    // start time
            thumbnail.getTotalLength(),             // end time
            0,
            1.0f);                                  // vertical zoom
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {

        if (source == &thumbnail)       thumbnailChanged();
    }


private:

    void thumbnailChanged()
    {
        repaint();
    }



    juce::AudioThumbnail thumbnail;
    Label fileNameLabel;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveForm)
};

class PlayBackPosition : public juce::Component, private juce::Timer, public juce::RelativeTime
{
public:

    PlayBackPosition(AudioTransportSource& _transportSource)
        : transportSource(_transportSource)
    {
        addAndMakeVisible(&timeLabel);
        timeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        timeLabel.setText("Time:", juce::dontSendNotification);
        timeLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
        timeLabel.setJustificationType(juce::Justification::centred);

        startTimer(40);
    }

    ~PlayBackPosition()
    {
        stopTimer();
    }

    void resized() override
    {

        timeLabel.setBounds(10, 100, getWidth() - 20, getHeight());
    }


    void paint(juce::Graphics& g) override
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

    void mouseDown(const juce::MouseEvent& event)
    {
        auto duration = transportSource.getLengthInSeconds();

        if (transportSource.getTotalLength() > 0)
        {
            auto clickPosition = event.position.x;
            auto audioPosition = (clickPosition / (float)getWidth()) * duration;

            transportSource.setPosition(audioPosition);
        }
    }

    void pausePlayback()
    {
        stopTimer();
    }

    void resumePlayback()
    {
        startTimer(40);
    }

private:

    void timerCallback() override
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

    Label timeLabel;
    AudioTransportSource& transportSource;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayBackPosition)

};




//==============================================================================
class MainContentComponent : public juce::AudioAppComponent,
    private juce::ChangeListener

{
public:
    MainContentComponent()
        : state(Stopped),
        thumbnailCache(5),                            // [4]
        waveForm(512, formatManager, thumbnailCache), // [5]
        playbackPosition(transportSource)
    {
        addAndMakeVisible(&openButton);
        openButton.setButtonText("Open...");
        openButton.onClick = [this] { openButtonClicked(); };

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


        addAndMakeVisible(waveForm);
        addAndMakeVisible(playbackPosition);

        setSize(600, 400);

        formatManager.registerBasicFormats();
        transportSource.addChangeListener(this);


        setAudioChannels(2, 2);


    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
            bufferToFill.clearActiveBufferRegion();
        else
            transportSource.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void resized() override
    {
        openButton.setBounds(10, 10, getWidth() - 20, 20);
        playButton.setBounds(10, 40, getWidth() - 20, 20);
        stopButton.setBounds(10, 70, getWidth() - 20, 20);

        loopingToggle.setBounds(100, getHeight() - 20, 40, 20);

        juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

        waveForm.setBounds(thumbnailBounds);
        playbackPosition.setBounds(thumbnailBounds);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState(Playing);
            else if ((state == Stopping) || (state == Playing))
                changeState(Stopped);
            else if (Pausing == state)
                changeState(Paused);
        }
    }

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Pausing,
        Paused
    };


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

    void transportSourceChanged()
    {
        changeState(transportSource.isPlaying() ? Playing : Stopped);
    }


    void openButtonClicked()
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
                        playButton.setEnabled(true);
                        waveForm.setFile(file);                            // [7]
                        readerSource.reset(newSource.release());

                    }
                }
            });
    }


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

    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    juce::ToggleButton loopingToggle;
    juce::Label toggleLabel;


    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;                    // [3]
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    juce::AudioThumbnailCache thumbnailCache;                  // [1]
    WaveForm waveForm;                      // [2]
    PlayBackPosition playbackPosition;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
