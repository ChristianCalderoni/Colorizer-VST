#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "FractionalDelay.h"
#include "ADSREnvelope.h"

class SynthVoice : public SynthesiserVoice, public AudioProcessorValueTreeState::Listener
{
public:
	void prepareToPlay(double sampleRate, int samplesPerBlock);
	void releaseResources();
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void parameterChanged(const String& paramID, float newValue) override;
	void setDrySignal(const AudioBuffer<float> dryBuffer, int startSample, int numSamples);
	void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;

private:
	AudioBuffer<float> voiceBuffer;
	FractionalDelay resonatorUnit;
	ADSREnvelope adsr;

	float fineTune = DEFAULT_FINE_TUNE;

	int currentMidiNote = 0;

	float currentVelocity = 1.0f;
};