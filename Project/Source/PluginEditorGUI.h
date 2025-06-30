/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotarySliderLNF.h"
#include "LinearSliderLNF.h"
#include "ToggleButtonLNF.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginEditorGUI  : public juce::AudioProcessorEditor
{
public:
    //==============================================================================
    PluginEditorGUI (PolyphonicResonatorAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~PluginEditorGUI() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* brushedMetalRounded_png;
    static const int brushedMetalRounded_pngSize;
    static const char* brushedMetal_png;
    static const int brushedMetal_pngSize;
    static const char* colorizerLogo_svg;
    static const int colorizerLogo_svgSize;
    static const char* logoN_svg;
    static const int logoN_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PolyphonicResonatorAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& valueTreeState;
    RotarySliderLNF rotarySlider;
    LinearSliderLNF linearSlider;
    ToggleButtonLNF toggleButton;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    std::unique_ptr<SliderAttachment> feedbackAttachment;
    std::unique_ptr<ButtonAttachment> negativeFbAttachment;

    std::unique_ptr<SliderAttachment> wetAmountAttachment;
    std::unique_ptr<ButtonAttachment> wetOnlyAttachment;

    std::unique_ptr<SliderAttachment> dampingAttachment;

    std::unique_ptr<SliderAttachment> fineTuneAttachment;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> attackSlider;
    std::unique_ptr<juce::Slider> decaySlider;
    std::unique_ptr<juce::Slider> sustainSlider;
    std::unique_ptr<juce::Slider> releaseSlider;
    std::unique_ptr<juce::Slider> feedbackSlider;
    std::unique_ptr<juce::ToggleButton> negativeFeedbackBtn;
    std::unique_ptr<juce::Slider> wetAmountSlider;
    std::unique_ptr<juce::ToggleButton> wetOnlyBtn;
    std::unique_ptr<juce::Slider> dampingSlider;
    std::unique_ptr<juce::Slider> fineTuneSlider;
    juce::Image cachedImage_brushedMetal_png_1;
    juce::Image cachedImage_brushedMetalRounded_png_2;
    std::unique_ptr<juce::Drawable> drawable3;
    std::unique_ptr<juce::Drawable> drawable4;
    std::unique_ptr<juce::Drawable> drawable5;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditorGUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

