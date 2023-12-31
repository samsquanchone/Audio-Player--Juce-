/*
  ==============================================================================

    WaveformControls.h
    Created: 10 Nov 2023 4:43:28pm
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
    CustomButtons()
    {
        
    }
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        const int x = 0;
        const int y = 0;
        const int width = button.getWidth();
        const int height = button.getHeight();
            
        const float indent = 2.0f;
        const int cornerSize = jmin(roundToInt(width * 0.4), roundToInt(height * 0.4));

        

        Path p;
        //p.addRoundedRectangle(indent, indent, width - indent * 2.0f, height - indent * 2.0f, (float)cornerSize);

        //Draw play button (triangle)
        p.addTriangle(x, y, x, height, width, height / 2);
       
        Colour bc(backgroundColour.withMultipliedSaturation(2));

        //Briefly set colour of button to purple when pressed
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }

      
        g.setColour(bc);
        g.fillPath(p);
    }


};

class StopButton : public juce::LookAndFeel_V4
{
public:
    StopButton()
    {

    }

    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        const int x = 0;
        const int y = 0;
        const int width = button.getWidth();
        const int height = button.getHeight();

        Path p;

        p.addRectangle(juce::Rectangle<int>(x, y, width / 2, height));

        
        Colour bc(backgroundColour.withMultipliedSaturation(2));
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }
        g.setColour(bc);
        g.fillPath(p);
    }
};