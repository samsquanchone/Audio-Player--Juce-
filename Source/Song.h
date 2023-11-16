/*
  ==============================================================================

    Song.h
    Created: 15 Nov 2023 12:06:11pm
    Author:  09sam

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Song
{

    Song(const juce::File& _file, const unsigned int& _id);


    juce::File file;
    juce::String fileName;
    unsigned int id;

};
