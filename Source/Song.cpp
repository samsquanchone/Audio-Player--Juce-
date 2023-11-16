/*
  ==============================================================================

    Song.cpp
    Created: 15 Nov 2023 12:06:11pm
    Author:  09sam

  ==============================================================================
*/

#include "Song.h"

Song::Song(const juce::File& _file, const unsigned& _id)
{
    file = _file;
    fileName = file.getFileNameWithoutExtension();
    id = _id;
}
