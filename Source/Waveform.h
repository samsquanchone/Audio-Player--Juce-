/*
  ==============================================================================

    Waveform.h
    Created: 6 Nov 2023 1:50:50pm
    Author:  Squanch Audio

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveForm : public juce::Component, private juce::ChangeListener
{
public:

    //Constructor arguments are what are required to initialise a audiothumbnail
    WaveForm(int samples, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbnailCache);

   

    ~WaveForm();
    
    void resized() override;
   
    void paint(juce::Graphics& g) override;
   

    //When a file is selected from main component it will be passed here to set the thumbnail source (const used as it is a large file)
    void setFile(const juce::File& file);
   
    void paintIfNoFileLoaded(juce::Graphics& g);
   

    void paintIfFileLoaded(juce::Graphics& g);
 
    
    void paintMonoIfFileLoaded(juce::Graphics& g);
  

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    

private:

    void thumbnailChanged();
   
    

    juce::AudioThumbnail thumbnail;
    Label fileNameLabel;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveForm)
};