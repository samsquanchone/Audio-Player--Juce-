/*
  ==============================================================================

    Waveform.cpp
    Created: 6 Nov 2023 1:51:02pm
    Author:  Squanch Audio

  ==============================================================================
*/

#include "Waveform.h"

//Constructor arguments are what are required to initialise a audiothumbnail
WaveForm::WaveForm(int samples, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbnailCache)

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

WaveForm::~WaveForm()
{
    //Remove observer
    thumbnail.removeChangeListener(this);
}

void WaveForm::resized() 
{
    fileNameLabel.setBounds(10, -120, getWidth() - 20, getHeight());

}
void WaveForm::paint(juce::Graphics& g) 
{
    juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g);
    else
        paintIfFileLoaded(g);
}


//When a file is selected from main component it will be passed here to set the thumbnail source (const used as it is a large file)
void WaveForm::setFile(const juce::File& file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
    fileNameLabel.setText((juce::String)file.getFileNameWithoutExtension(), juce::dontSendNotification);
}

void WaveForm::paintIfNoFileLoaded(juce::Graphics& g)
{

    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
}

void WaveForm::paintIfFileLoaded(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    g.setColour(juce::Colours::white);

    g.setColour(juce::Colours::darkslateblue);                               // [8]


    thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);

    g.setColour(juce::Colours::green);


}

void WaveForm::paintMonoIfFileLoaded(juce::Graphics& g)
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

void WaveForm::changeListenerCallback(juce::ChangeBroadcaster* source) 
{

    if (source == &thumbnail)       thumbnailChanged();
}




void WaveForm::thumbnailChanged()
{
      repaint();
}
