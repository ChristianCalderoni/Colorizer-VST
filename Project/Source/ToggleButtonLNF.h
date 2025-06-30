#pragma once
#include <JuceHeader.h>

class ToggleButtonLNF : public LookAndFeel_V4
{
public:

    void drawToggleButton(Graphics& g, ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto fontSize = jmin(25.0f, (float)button.getHeight() * 0.75f);

        auto tickWidth = fontSize;

        drawTickBox(g, button, ((float)button.getWidth() - tickWidth) * 0.5f, ((float)button.getHeight() - tickWidth) * 0.5f,
            tickWidth, tickWidth,
            button.getToggleState(),
            button.isEnabled(),
            shouldDrawButtonAsHighlighted,
            shouldDrawButtonAsDown);

        g.setColour(button.findColour(ToggleButton::textColourId));
        g.setFont(fontSize);

        if (!button.isEnabled())
            g.setOpacity(0.5f);

        g.drawFittedText(button.getButtonText(),
            button.getLocalBounds().withTrimmedLeft(roundToInt(tickWidth) + 10)
            .withTrimmedRight(2),
            Justification::centredLeft, 10);
    }

    void drawTickBox(Graphics& g, Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        const bool isEnabled,
        const bool shouldDrawButtonAsHighlighted,
        const bool shouldDrawButtonAsDown) override
    {
        ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        Colour brightColour = Colour(0xff2b2d31);
        Colour darkColour = Colour(0xff0d0d11);

        Rectangle<float> tickBounds(x, y, w, h);

        //=========== DEBUG ============
        //g.setColour(component.findColour(ToggleButton::tickDisabledColourId));
        //g.drawRect(x, y, w, h, 2.0f);                                                           //button bounds
        //g.drawRect(0.0f,0.0f, (float)component.getWidth(), (float)component.getHeight(), 1.0f); //global button bounds
        //=========== DEBUG ============

        // OUTLINE GRADIENT
        g.setGradientFill(ColourGradient(brightColour.brighter(0.5f), x + w * 0.5, y, darkColour, x + w * 0.5, h, false));
        g.fillEllipse(tickBounds);                      

        // BORDER
        g.setColour(Colour(0xff363636));
        g.fillEllipse(tickBounds.reduced(1.0f));        

        // BUTTON DISABLED OUTLINE GRADIENT
        g.setGradientFill(ColourGradient(brightColour.brighter(0.5f), x + w * 0.5, y, darkColour, x + w * 0.5, h, false));
        g.fillEllipse(tickBounds.reduced(5.0f));         


        // BUTTON DISABLED
        g.setGradientFill(ColourGradient(Colours::orangered.darker(0.99f), x + w * 0.5, y + h * 0.5, Colour(0xff363636).darker(0.5f), x + w * 0.5, y, true));
        g.fillEllipse(tickBounds.reduced(6.0f));        


        if (ticked)
        {
            // OUTER GLOW
            g.setColour(Colours::orangered.brighter(0.4f).withAlpha(0.15f));
            g.fillEllipse(tickBounds);
            
            // BUTTON ENABLED
            g.setGradientFill(ColourGradient(Colours::orangered, x + w * 0.5, y + h * 0.5, Colours::orangered.brighter(0.6f), x + w * 0.5, y, true));
            g.fillEllipse(tickBounds.reduced(6.0f));
        }
    }

    void changeToggleButtonWidthToFitText(ToggleButton& button) override
    {
        auto fontSize = jmin(20.0f, (float)button.getHeight() * 0.75f);
        auto tickWidth = fontSize * 1.1f;

        Font font(fontSize);

        button.setSize(font.getStringWidth(button.getButtonText()) + roundToInt(tickWidth) + 14, button.getHeight());
    }
};