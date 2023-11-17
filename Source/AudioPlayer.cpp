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

    transportControl(transportSource, readerSource), resampledSource(&transportSource, true, 2)
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

    startTimer(100);
 
    transportControl.nextSongButton.onClick = [this] { playNextSong(); }; //this is coupled atm deffo wanna refactor it

    transportControl.previousSongButton.onClick = [this] { playPreviousSong();  }; //this is coupled atm deffo wanna refactor it
   // transportSource.addChangeListener(&playNextSong());
    

}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
    playlistContainer.clear();
    transportSource.releaseResources();
    resampledSource.releaseResources();
    mixerSource.releaseResources();

    stopTimer();
   
    
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    ///* Below are just an example on how to set up pitch control (COULD BE USED FOR NEXT PROJECT! DJ APP)*/
    resampledSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampledSource.setResamplingRatio(1);
    
  
}

void MainContentComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
    }
    else
    {
        transportSource.getNextAudioBlock(bufferToFill);
       
    }
    if (readerSource.get() != nullptr && transportSource.hasStreamFinished())
    {
        hasSongFinished = true; //Obviously is not thread safe to call our next song function here, therefore we use an atomic flag to ensure we can change song
    }


}

void MainContentComponent::releaseResources()
{
    transportSource.releaseResources();
    resampledSource.releaseResources();
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

                    mixerSource.addInputSource(&resampledSource, false); //Example of how to do pitch control, requires adding mixer source
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

                playlistContainer.push_back(std::make_unique<Song>(file, playlistContainer.size())); //Make entry unique and pass file into constructor, as well as vector size (for ID purposes)

                Logger::outputDebugString(playlistContainer.front().get()->fileName);
                Logger::outputDebugString(juce::String(playlistContainer.size()));
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
    
}

void MainContentComponent::playNextSong()
{

    if (!readerSource->isLooping() && playlistPos + 1 < playlistContainer.size()) //actually! we may not want to do this here! think about it.... even if we are looping and we skip song, we probs wanna change the song lol!
    {
        if (!isFirstTimePlayingFromPlaylist) { playlistPos = playlistContainer[playlistPos].get()->id + 1; }
        auto file = playlistContainer[playlistPos].get()->file; //get file from current queue song object

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
            hasSongFinished = false;

            isFirstTimePlayingFromPlaylist = false;
            
        }
    }

}

void MainContentComponent::playPreviousSong()
{
    if (!readerSource->isLooping()) //actually! we may not want to do this here! think about it.... even if we are looping and we skip song, we probs wanna change the song lol!
    {

        if (playlistPos != 0) {
            playlistPos = playlistContainer[playlistPos].get()->id - 1;  //Check the playlist pos is not == 0 (otherwise we will be trying to access an index that is out of range)

            auto file = playlistContainer[playlistPos].get()->file; //get file from current queue song object

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
                hasSongFinished = false;


            }
        }
    }
}

void MainContentComponent::timerCallback()
{
    if (hasSongFinished)
    {
        playNextSong();
    }
}

