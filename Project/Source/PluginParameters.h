#pragma once

#include <JuceHeader.h>

//==================================
//			FractionalDelay

#define MAX_DELAY_TIME   2.0f

#define NAME_DELAY_TIME			"time"
#define DEFAULT_DELAY_TIME		0.002f //resonates at 440Hz(A4) by default.

#define NAME_FB					"fb"
#define DEFAULT_FB				0.8f

#define NAME_NEGATIVE_FB		"negativeFb"
#define DEFAULT_NEGATIVE_FB		0.0f

#define NAME_FINE_TUNE			"fineTune"
#define DEFAULT_FINE_TUNE		0.0f

#define FB_SMOOTHING_TIME		0.01f
#define TIME_SMOOTHING_TIME		0.01f
#define NEG_FB_SMOOTHING_TIME	0.01f
#define FINE_TUNE_SMOOTHING_TIME 0.01f


//==================================
//			ADSREnvelope

#define DEFAULT_ATTACK			0.1f
#define NAME_ATTACK				"attack"

#define DEFAULT_DECAY			0.2f
#define NAME_DECAY				"decay"

#define DEFAULT_SUSTAIN			0.5f
#define NAME_SUSTAIN			"sustain"

#define DEFAULT_RELEASE			1.0f
#define NAME_RELEASE			"release"

//=================================
//			DampingFilter

#define DEFAULT_NUM_FILTERS		2

#define DEFAULT_QUALITY			0.7f

#define DEFAULT_FREQUENCY		18000.0f
#define NAME_FREQUENCY			"frequency"

//=================================
//			DryWetManager

//#define DEFAULT_WET_AMOUNT		0.7f

#define DEFAULT_WET_AMOUNT		-2.0f //dB
#define FLOOR_OUT_LEVEL			-56.0f //dB
#define NAME_WET_AMOUNT			"wetAmount"


//=================================
//			GLOBAL

#define WET_SCALE_FACTOR		0.5f
#define NUM_VOICES				8

#define NAME_WET_ONLY			"wetOnly"
#define DEFAULT_WET_ONLY		false

#define DEFAULT_CONCERT_PITCH	440.0f

//=================================

static AudioProcessorValueTreeState::ParameterLayout addParameterLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

	//ADSR
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_ATTACK, "Attack", NormalisableRange<float>(0.0f, 10.0f, 0.01f, 0.5f), DEFAULT_ATTACK));
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_DECAY, "Decay", NormalisableRange<float>(0.0f, 10.0f, 0.01f, 0.5f), DEFAULT_DECAY));
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_SUSTAIN, "Sustain", NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.6f), DEFAULT_SUSTAIN));
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_RELEASE, "Release", NormalisableRange<float>(0.0f, 10.0f, 0.01f, 0.5), DEFAULT_RELEASE));

	//Fractional Delay
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_FB, "Feedback", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 3.0f), DEFAULT_FB));
	params.push_back(std::make_unique<AudioParameterBool>(NAME_NEGATIVE_FB, "Negative Feedback", DEFAULT_NEGATIVE_FB));
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_FINE_TUNE, "Fine Tune", -0.5f, 0.5f, DEFAULT_FINE_TUNE));

	//Dry wet manager
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_WET_AMOUNT, "Wet Amount", NormalisableRange<float>(FLOOR_OUT_LEVEL, 0.0f, 0.1f, 5.0f), DEFAULT_WET_AMOUNT));
	params.push_back(std::make_unique<AudioParameterBool>(NAME_WET_ONLY, "Wet Only", DEFAULT_WET_ONLY));
	
	//Damping Filter
	params.push_back(std::make_unique<AudioParameterFloat>(NAME_FREQUENCY, "Damping", NormalisableRange<float>(20.0f, 20000.0f, 10.0f, 0.5f), DEFAULT_FREQUENCY));

	return {params.begin(), params.end()}; //these are iterators
}