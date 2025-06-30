#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"

class ADSREnvelope : public ADSR
{
public:
	ADSREnvelope() 
	{
		adsrParams.attack = DEFAULT_ATTACK;
		adsrParams.decay = DEFAULT_DECAY;
		adsrParams.sustain = DEFAULT_SUSTAIN;
		adsrParams.release = DEFAULT_RELEASE;
	};
	~ADSREnvelope() {};

	void prepareToPlay(const double sampleRate)
	{
		setSampleRate(sampleRate);
		setParameters(adsrParams);
	}

	void parameterChanged(const String& paramID, float newValue)
	{
		if (paramID == NAME_ATTACK)
			adsrParams.attack = newValue;

		if (paramID == NAME_DECAY)
			adsrParams.decay = newValue;

		if (paramID == NAME_SUSTAIN)
			adsrParams.sustain = newValue;

		if (paramID == NAME_RELEASE)
			adsrParams.release = newValue;

		setParameters(adsrParams);
	}

private:
	ADSR::Parameters adsrParams;
};