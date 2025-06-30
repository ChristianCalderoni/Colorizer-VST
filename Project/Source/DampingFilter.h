#pragma once
#include <JuceHeader.h>
#include "PluginParameters.h"

class DampingFilter
{
public:
	DampingFilter()
	{
		for (int i = 0; i < DEFAULT_NUM_FILTERS; ++i)
			iirFilters.add(new IIRFilter());
	}

	~DampingFilter() {}

	void prepareToPlay(double sr)
	{
		sampleRate = sr;
		for (int i = iirFilters.size(); --i >= 0;)
			iirFilters.getUnchecked(i)->reset();
		
		updateCoefficients();
	}

	float processSample(float sample, int ch)
	{
		return iirFilters.getUnchecked(ch)->processSingleSampleRaw(sample);
	}

	void parameterChanged(const String& paramID, float newValue)
	{
		if (paramID == NAME_FREQUENCY)
			frequency = jmin(newValue, (float)sampleRate/2.0f);

		updateCoefficients();
	}

private:

	void updateCoefficients()
	{
		const IIRCoefficients coefficients = IIRCoefficients::makeLowPass(sampleRate, frequency, quality);

		for (int i = iirFilters.size(); --i >= 0;)
			iirFilters.getUnchecked(i)->setCoefficients(coefficients);
	}

	double sampleRate = 44100.0;
	float frequency = DEFAULT_FREQUENCY;
	float quality = DEFAULT_QUALITY;

	OwnedArray<IIRFilter> iirFilters; //each channel has to be filtered with a different instance of a filter.
};