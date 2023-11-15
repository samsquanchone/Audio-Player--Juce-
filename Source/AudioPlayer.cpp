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

    transportControl(transportSource, readerSource)
{

    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&openFileForQueueButton);
    openFileForQueueButton.setButtonText("Add File to song queue...");
    openFileForQueueButton.onClick = [this] { openAddFileToQueueButtonClicked(); };
   

    addAndMakeVisible(waveForm);
    addAndMakeVisible(transportControl);
 

    setSize(600, 400);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);


    setAudioChannels(2, 2);

    

}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
    while (playlistContainer.empty() != true)
    {
        playlistContainer.pop();
    }

    
   
    
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
    openFileForQueueButton.setBounds(10, 30, getWidth() - 20, 20);
  
    juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 200);


    waveForm.setBounds(thumbnailBounds);
    transportControl.setBounds(10, 50, getWidth() - 20, getHeight());
    
}

void MainContentComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        //Note if we get this refactoring to work, maybe use a getter, so we can keep state private!
        if (transportSource.isPlaying())
            transportControl.changeState(Playing);
        else if ((transportControl.GetTransportState() == Stopping) || (transportControl.GetTransportState() == Playing))
            transportControl.changeState(Stopped);
        else if (Pausing == transportControl.GetTransportState())
            transportControl.changeState(Paused);
    }
}


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

void MainContentComponent::openAddFileToQueueButtonClicked()
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

                playlistContainer.push(std::make_unique<Song>(file)); //Make entry unique and pass file into constructor


                Logger::outputDebugString(playlistContainer.front().get()->fileName);
                Logger::outputDebugString(juce::String(playlistContainer.size()));

                //auto* reader = formatManager.createReaderFor(file);

               /* if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                    playlistContainer.push(std::make_unique<Song>(file)); //Make entry unique and pass file into constructor
  
                   
                    Logger::outputDebugString(playlistContainer.front().get()->fileName);
                    Logger::outputDebugString(juce::String(playlistContainer.size()));
                    
                }
                */

              //  checkIfSongHasFinished();
            }
            
        });

    
}

juce::File MainContentComponent::getFile()
{
    return juce::File();
}

void MainContentComponent::checkIfSongHasFinished()
{
    if (transportSource.hasStreamFinished()) { playNextSong(); }

    playNextSong();
}

void MainContentComponent::playNextSong()
{
    auto file = playlistContainer.front().get()->file; //get file from current queue song object
    
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
