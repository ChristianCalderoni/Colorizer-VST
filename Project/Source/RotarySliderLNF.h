#pragma once
#include <JuceHeader.h>

class RotarySliderLNF : public LookAndFeel_V4
{
public:
    RotarySliderLNF() {};

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
        auto fill = slider.findColour(Slider::rotarySliderFillColourId);
        auto tickColour = slider.findColour(Slider::thumbColourId);
        auto innerCircleColour = slider.findColour(Slider::backgroundColourId);

        auto radius = (jmin(width * 0.5f, height * 0.5f) - 4.0f);
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f; // diameter

        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = jmin(6.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        Colour brightColour = Colour(0xff2b2d31);
        Colour darkColour = Colour(0xff0d0d11);

        Rectangle<float> innerCircle(rx + lineW, ry + lineW, rw - lineW * 2, rw - lineW * 2);

        // INNER CIRCLE OUTER BORDER
        //g.setGradientFill(ColourGradient(brightColour.brighter(0.2f), centreX, y, darkColour.darker(0.05f), centreX, y+height, false));
        g.setGradientFill(ColourGradient(brightColour.brighter(0.3f), centreX, y, darkColour.darker(0.1f), centreX, y + height, false));
        g.fillEllipse(innerCircle.expanded(0.5f));

        // INNER CIRCLE
        //g.setColour(Colour(0xff363636).darker(0.5f));
        g.setColour(fill);
        g.setGradientFill(ColourGradient(fill, centreX, y, fill.darker(0.3f), centreX, y + height, false));
        g.fillEllipse(innerCircle.reduced(2.0f));


        // TICK POINTER
        Path tick;
        auto tickLength = radius * 0.66f;
        auto tickThickness = 3.0f;

        // -tickThickness * 0.5f --> per centrare il tick in modo che l'origine si trovi a metà della larghezza del tick
        //-radius + lineW --> per shiftare il tick verso il bordo lasciando una piccola quantità pari al bordo esterno in modo che punti all'interno del bordo
        tick.addRoundedRectangle(-tickThickness * 0.5f, -radius + lineW, tickThickness, tickLength, 3.0f);

        tick.applyTransform(AffineTransform::rotation(toAngle).translated(centreX, centreY));

        g.setColour(tickColour);
        g.fillPath(tick);

        // OUTER BACKGROUND PATH
        Path backgroundArc;
        backgroundArc.addCentredArc(centreX,
            centreY,
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        Point<float> origin(centreX, centreY);
        Point<float> rotation(centreX, centreY * sliderPos + rw - lineW);
        //Point<float> rotation(centreX, -radius * sliderPos);

        //g.drawLine(Line<float>(origin, rotation));
        //g.setColour(outline);
        //g.setGradientFill(ColourGradient(brightColour.brighter(0.2f), origin, darkColour.darker(0.05f), rotation, true));
        g.setGradientFill(ColourGradient(slider.findColour(Slider::trackColourId), origin, slider.findColour(Slider::rotarySliderOutlineColourId), rotation, true));
        g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            // VALUE PATH
            Path valueArc;
            valueArc.addCentredArc(centreX,
                centreY,
                arcRadius,
                arcRadius,
                0.0f,
                rotaryStartAngle,
                toAngle,
                true);

            g.setGradientFill(ColourGradient(Colours::orange, x, centreY, Colours::red, x+width, centreY, false));
            g.strokePath(valueArc, PathStrokeType(lineW-4.0f, PathStrokeType::curved, PathStrokeType::rounded));
        }
    }
};