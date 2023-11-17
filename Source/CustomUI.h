/*
  ==============================================================================

    CustomUI.h
    Created: 17 Nov 2023 11:40:10am
    Author:  09sam

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


/// <summary>
/// This class is just to test the basis of custom look and feel, the idea is to create more complex buttons e.g. a rotate triangle
/// </summary>
class CustomButtons : public juce::LookAndFeel_V4
{
public:
    CustomButtons();
   
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
        

};

class StopButton : public juce::LookAndFeel_V4
{
public:
    StopButton();
  

    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

};

class FastForwardButton : public juce::LookAndFeel_V4
{
public:

    FastForwardButton();


    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;


};

class RewindButton : public juce::LookAndFeel_V4
{
public:

    RewindButton();

    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    


};

/// <summary>
/// Idea for this class is to do similar to the rewind and fastfoward button, but instead of the second triangle, we will draw a narrow high rectangle 
/// </summary>
class  MoveToPreviousSongButton : public juce::LookAndFeel_V4
{
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    


};

class MoveToNextSongButton : public juce::LookAndFeel_V4
{
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
};

