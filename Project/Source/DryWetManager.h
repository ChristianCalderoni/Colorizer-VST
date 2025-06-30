#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"

class DryWetManager : public AudioProcessorValueTreeState::Listener
{
public:
	DryWetManager()
	{
		wetGain = Decibels::decibelsToGain(DEFAULT_WET_AMOUNT);
		updateInternalState(DEFAULT_WET_AMOUNT);
	}

	~DryWetManager() {};

	void prepareToPlay(const int maxSamplesPerBlock)
	{
		drySignal.setSize(2, maxSamplesPerBlock);
	}

	void releaseResources()
	{
		drySignal.setSize(0, 0);
	}

	void storeFrom(const AudioBuffer<float>& inputSignal)
	{
		const auto numSamples = inputSignal.getNumSamples();

		for (int ch = inputSignal.getNumChannels(); --ch >= 0;)
			drySignal.copyFrom(ch, 0, inputSignal, ch, 0, numSamples);
	}

	void mergeTo(AudioBuffer<float>& outputSignal)
	{
		const auto numSamples = outputSignal.getNumSamples();
		
		applyWetAmount(outputSignal, numSamples);

		for (int ch = outputSignal.getNumChannels(); --ch >= 0;)
			outputSignal.addFrom(ch, 0, drySignal, ch, 0, numSamples);
	}

	void applyWetAmount(AudioBuffer<float>& outputSignal, const int numSamples)
	{
		if (doSmooth)
		{
			outputSignal.applyGainRamp(0, numSamples, wetGainOld, wetGain);

			doSmooth = false;
		}
		else
		{
			outputSignal.applyGain(wetGain);
		}
	}

private:
	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_WET_AMOUNT)
			updateInternalState(newValue);
	}

	void updateInternalState(float newValue)
	{
		wetGainOld = wetGain;

		wetGain = newValue <= FLOOR_OUT_LEVEL ? 0.0f : Decibels::decibelsToGain(newValue);

		doSmooth = true;
	}

	AudioBuffer<float> drySignal;

	float wetGain, wetGainOld;

	bool doSmooth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWetManager)
};