#pragma once
#include <JuceHeader.h>

#define OUTLINE_WIDTH 2.0f

class LinearSliderLNF : public LookAndFeel_V4
{
public:
    LinearSliderLNF() {};

    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider) override
    {
        auto trackWidth = jmin(6.0f, slider.isHorizontal() ? (float)height * 0.2f : (float)width * 0.2f);

        Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                                slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

        Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                              slider.isHorizontal() ? startPoint.y : (float)y);

        Colour brightColour = Colour(0xff2b2d31);
        Colour darkColour = Colour(0xff0d0d11);

        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;

        Point<float> centrePoint(centreX, centreY);

        Point<float> currentCenterSliderPosition(slider.isVertical() ? centreX : sliderPos, 
                                                 slider.isVertical()? sliderPos : centreY);

        Point<float> currentRadiusSliderPosition(slider.isVertical() ? centreX : sliderPos, 
                                                 slider.isVertical() ? sliderPos + width*0.5f : centreY + height*0.5f);


        int numLines = 15;

        for (auto i = 0; i <= numLines; ++i)
        {
            g.setGradientFill(ColourGradient(Colours::orange, currentCenterSliderPosition, darkColour.darker(0.05f), currentRadiusSliderPosition, true)); // moving gradient

            if (slider.isVertical())
            {
                //Left lines
                g.fillRoundedRectangle(centreX - 10.0f - (float)width * 0.25f, y + height - i * (height/numLines), (float)width * 0.25f, 1.0f, 2.0f);
                
                //Right Lines
                g.fillRoundedRectangle(centreX + 10.0f, y + height - i * (height / numLines), (float)width * 0.25f, 1.0f, 2.0f);
            }

            if (slider.isHorizontal())
            {
                //Upper lines
                g.fillRoundedRectangle(x + i * (width / numLines), centreY - height * 0.4f, 1.0f, height * 0.2f, 2.0f);
                
                //Lower Lines
                //g.fillRoundedRectangle(x + width - i * (width / numLines), centreY + height * 0.25f, 1.0f, height * 0.25f, 2.0f);
            }
        }

        // BACKGROUND TRACK
        Path backgroundTrack;

        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(endPoint);
        g.setColour(slider.findColour(Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });        
        
        Point<float> minPoint, maxPoint;

        auto kx = slider.isHorizontal() ? sliderPos : centreX;
        auto ky = slider.isHorizontal() ? centreY : sliderPos;

        minPoint = startPoint; //minimum point the slider can reach. It's in the center (minimum) of the slider's track.
        maxPoint = { kx, ky };

        auto thumbWidth = getSliderThumbRadius(slider);

        //BACKGROUND VALUE TRACK
        Path valueTrack;

        valueTrack.startNewSubPath(slider.getMinimum() < 0 ? centrePoint : minPoint);
        valueTrack.lineTo(maxPoint);
        g.setColour(slider.findColour(Slider::trackColourId));
        g.strokePath(valueTrack, { trackWidth - 2.0f, PathStrokeType::curved, PathStrokeType::rounded });

        //INNER COLOURED VALUE TRACK

        Path valueTrackColoured;

        valueTrackColoured.startNewSubPath(slider.getMinimum() < 0 ? centrePoint : minPoint);
        valueTrackColoured.lineTo(maxPoint);
        g.setGradientFill(ColourGradient(Colours::orange, slider.getMinimum() < 0 ? centrePoint : minPoint, Colours::red, maxPoint, false));
        g.strokePath(valueTrackColoured, { trackWidth - 5.0f, PathStrokeType::curved, PathStrokeType::rounded });


        // POINTER
                
        //makes the gradient follow the pointer
        Point<float> startGradient(sliderPos, slider.isVertical() ? sliderPos - thumbWidth*0.5f : centreY - thumbWidth * 0.5f);
        Point<float> endGradient(sliderPos, slider.isVertical() ? sliderPos + thumbWidth*0.5f : centreY + thumbWidth * 0.5f);

        //SHADOW
        
        //g.setColour(Colours::black.withAlpha(0.3f));
        //g.fillRoundedRectangle(Rectangle<float>(static_cast<float> (thumbWidth + 10.0f), static_cast<float> (thumbWidth)).withCentre(Point<float>(kx + 3.0f, ky + 2.0f)), 3.0f);
       
        Rectangle<float> pointer(static_cast<float> (slider.isVertical() ? thumbWidth + 10.0f : thumbWidth), 
                                 static_cast<float> (slider.isVertical() ? thumbWidth : thumbWidth));

        // MAIN POINTER
        g.setGradientFill(ColourGradient(brightColour.brighter(0.05f), startGradient, darkColour.brighter(0.05f), endGradient, false));
        g.fillRoundedRectangle(pointer.withCentre(maxPoint), 3.0f);
        
        // MAIN POINTER OUTER GRADIENT
        g.setGradientFill(ColourGradient(brightColour.brighter(0.2f), startGradient, darkColour, endGradient, false));
        g.drawRoundedRectangle(pointer.reduced(0.5f).withCentre(maxPoint), 3.0f, 1.0f);

        // INDICATOR ON POINTER
        g.setColour(slider.findColour(Slider::thumbColourId).darker(1.0f));
        g.fillRect(Rectangle<float>(static_cast<float> (slider.isVertical() ? thumbWidth + 10.0f : thumbWidth * 0.1f), 
                                    static_cast<float> (slider.isVertical() ? thumbWidth*0.1f : thumbWidth)).withCentre(maxPoint));
    }
};