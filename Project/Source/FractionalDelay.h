#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"
#include "DampingFilter.h"

class FractionalDelay
{
public:
	FractionalDelay()
	{
		smoothedTime.setCurrentAndTargetValue(DEFAULT_DELAY_TIME);
		smoothedFeedback.setCurrentAndTargetValue(DEFAULT_FB);
		smoothedNegativeFb.setCurrentAndTargetValue(DEFAULT_NEGATIVE_FB * (-2.0f) + 1.0f); //scaled between -1 and 1; at construction time its value is 1.
		//(DEFAULT_NEGATIVE_FB) defaults to 0 as it's not active in the first place; initialised to 1 as feedback would be 0 until this parameter is changed.
		smoothedFineTune.setCurrentAndTargetValue(DEFAULT_FINE_TUNE);
	}

	~FractionalDelay() {};

	void prepareToPlay(const int maxSamplesPerBlock, const double sampleRate)
	{
		delayBuffer.clear();

		damping.prepareToPlay(sampleRate);

		sr = sampleRate;
		const auto delayBufferSize = MAX_DELAY_TIME * sampleRate + maxSamplesPerBlock;
		delayBuffer.setSize(2, delayBufferSize);

		smoothedFeedback.reset(sr, FB_SMOOTHING_TIME);
		smoothedTime.reset(sr, TIME_SMOOTHING_TIME);
		smoothedNegativeFb.reset(sr, NEG_FB_SMOOTHING_TIME);
		smoothedFineTune.reset(sr, FINE_TUNE_SMOOTHING_TIME);
	}

	void releaseResources()
	{
		delayBuffer.setSize(0, 0);
	}

	void processBlock(AudioBuffer<float>& buffer, int startSample, int numSamples)
	{
		pullFrom(buffer, startSample, numSamples);

		pushTo(buffer, startSample, numSamples);

		updatePosition(numSamples);
	}

	void setDelayTime(float midiNoteNumber, float fineTune)
	{
		smoothedTime.setTargetValue(1.0f / midiToFrequency(midiNoteNumber + fineTune));
	}

	float midiToFrequency(const float midiNote) // converts midiPitch to frequency in Hz
	{
		return DEFAULT_CONCERT_PITCH * std::pow(2.0f, ((midiNote - 69.0f) / 12.0f));
	};

	void parameterChanged(const String& paramID, float newValue)
	{
		if (paramID == NAME_FB)
			smoothedFeedback.setTargetValue(newValue);

		if (paramID == NAME_NEGATIVE_FB)
			smoothedNegativeFb.setTargetValue(newValue ? -1.0f : 1.0f);

		if (paramID == NAME_FREQUENCY)
			damping.parameterChanged(paramID, newValue);

		if (paramID == NAME_FINE_TUNE)
			smoothedFineTune.setTargetValue(newValue);
	}

private:
	void pullFrom(const AudioBuffer<float>& inputSignal, int startSample, int numSamples)
	{
		const auto numCh = inputSignal.getNumChannels();
		const auto delayBufferSize = delayBuffer.getNumSamples();

		for (int ch = 0; ch < numCh; ++ch)
			if (writePosition + numSamples <= delayBufferSize)
				delayBuffer.copyFrom(ch, writePosition, inputSignal, ch, startSample, numSamples); // starts copying from the sample "startSample" in the input buffer, all the way to startSample + numSamples
			else
			{
				const auto fittingSamples = delayBufferSize - writePosition;
				const auto remainingSamples = numSamples - fittingSamples;
				delayBuffer.copyFrom(ch, writePosition, inputSignal, ch, startSample, fittingSamples);
				delayBuffer.copyFrom(ch, 0, inputSignal, ch, fittingSamples, remainingSamples);
			}
	}

	void pushTo(AudioBuffer<float>& outputSignal, int startSample, int numSamples)
	{
		const auto numCh = outputSignal.getNumChannels();
		const auto delayBufferSize = delayBuffer.getNumSamples();
		
		auto** delayData = delayBuffer.getArrayOfWritePointers();
		auto** outData = outputSignal.getArrayOfWritePointers();

		for (int smp = 0; smp < numSamples; ++smp)
		{
			//we avoid checking the boolean every time; multiply the number by either 1 or -1.
			const auto fb = smoothedFeedback.getNextValue() * smoothedNegativeFb.getNextValue(); 
			const auto dt = smoothedTime.getNextValue();

			const auto actualWritePosition = (writePosition + smp) % delayBufferSize;

			const float delayInSamples = dt * sr;
			const float actualReadPosition = ((float)actualWritePosition - delayInSamples); 

			const int integerPart = (int)floor(actualReadPosition);
			const float fractionalPart = actualReadPosition - (float)integerPart; 

			const int A = (integerPart + delayBufferSize) % delayBufferSize;

			const int B = (A + 1) % delayBufferSize;

			auto alpha = fractionalPart / (2.0f - fractionalPart); // Usato per il filtro allpass

			for (int ch = 0; ch < numCh; ++ch)
			{
				//  Allpass
				const auto sampleData = alpha * (delayData[ch][B] - oldOutput[ch]) + delayData[ch][A];
				oldOutput[ch] = sampleData;

				// Scrivo in output
				outData[ch][smp + startSample] = sampleData; //offset the position where it should write the output by startSample

				//Feedback & Damping
				float scaledSample = sampleData * fb;

				float filteredFb = damping.processSample(scaledSample, ch);

				delayData[ch][actualWritePosition] += filteredFb;
			}
		}
	}

	void updatePosition(const int leap)
	{
		writePosition += leap;
		writePosition %= delayBuffer.getNumSamples();
	}

	int writePosition = 0;
	double sr = 44100.0;

	DampingFilter damping;

	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFeedback;
	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedTime;
	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedNegativeFb;
	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFineTune;

	float oldOutput[2] = {0.0f, 0.0f}; // Array che contiene l'output precedente di entrambi i canali, usato per il filtro allpass

	AudioBuffer<float> delayBuffer;
};