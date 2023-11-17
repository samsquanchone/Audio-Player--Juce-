/*
  ==============================================================================

    CustomUI.cpp
    Created: 17 Nov 2023 11:40:10am
    Author:  09sam

  ==============================================================================
*/

#include "CustomUI.h"

    CustomButtons::CustomButtons()
    {

    }
    void CustomButtons::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) 
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
        p.addTriangle(x, y, x, height, (width * 60) / 100, height / 2);

        Colour bc(backgroundColour.withMultipliedSaturation(2));

        //Briefly set colour of button to purple when pressed
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }


        g.setColour(bc);
        g.fillPath(p);
    }





    StopButton::StopButton()
    {

    }

    void StopButton::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
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



    FastForwardButton::FastForwardButton()
    {}

    void FastForwardButton::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
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
        p.addTriangle(x, y, x, height, width / 2, height / 2);

        p.addTriangle(width / 2, y, width / 2, height, width, height / 2);

        Colour bc(backgroundColour.withMultipliedSaturation(2));

        //Briefly set colour of button to purple when pressed
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }


        g.setColour(bc);
        g.fillPath(p);
    }





    RewindButton::RewindButton() {};

    void RewindButton::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) 
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
        p.addTriangle(x, height / 2, width / 2, y, width / 2, height);

        p.addTriangle(width / 2, height / 2, width, y, width, height);

        Colour bc(backgroundColour.withMultipliedSaturation(2));

        //Briefly set colour of button to purple when pressed
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }


        g.setColour(bc);
        g.fillPath(p);
    }



/// <summary>
/// Idea for this class is to do similar to the rewind and fastfoward button, but instead of the second triangle, we will draw a narrow high rectangle 
/// </summary>

    void MoveToPreviousSongButton::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
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
        p.addTriangle(width / 2, height / 2, width, y, width, height);

        p.addRectangle(juce::Rectangle<int>(width / 2, y, (width * 10) / 100, height));

        Colour bc(backgroundColour.withMultipliedSaturation(2));

        //Briefly set colour of button to purple when pressed
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }


        g.setColour(bc);
        g.fillPath(p);
    }






    void MoveToNextSongButton::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
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
        p.addTriangle(x, y, x, height, width / 2, height / 2);

        p.addRectangle(juce::Rectangle<int>(width / 2, y, (width * 10) / 100, height));

        Colour bc(backgroundColour.withMultipliedSaturation(2));

        //Briefly set colour of button to purple when pressed
        if (button.isMouseButtonDown())
        {
            bc = juce::Colours::purple;
        }


        g.setColour(bc);
        g.fillPath(p);
    }