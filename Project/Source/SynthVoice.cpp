#include "SynthVoice.h"

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	setCurrentPlaybackSampleRate(sampleRate);

	resonatorUnit.prepareToPlay(samplesPerBlock, sampleRate);

	adsr.prepareToPlay(sampleRate);

	voiceBuffer.setSize(2, samplesPerBlock);
	voiceBuffer.clear();
}

void SynthVoice::releaseResources()
{
	voiceBuffer.setSize(0, 0);
}

bool SynthVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<SynthesiserSound*>(sound) != nullptr;
};

void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{
	adsr.noteOn();

	currentMidiNote = midiNoteNumber;

	currentVelocity = velocity;

	resonatorUnit.setDelayTime(midiNoteNumber, fineTune);
};

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();

	if (!allowTailOff || !adsr.isActive()) //If allowTailOff is false or the voice doesn't want to tail-off, then it must stop all sound immediately, and must call clearCurrentNote() 
		clearCurrentNote();
};

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

};

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

};

void SynthVoice::parameterChanged(const String& paramID, float newValue)
{
	if (paramID == NAME_ATTACK || paramID == NAME_DECAY || paramID == NAME_SUSTAIN || paramID == NAME_RELEASE)
		adsr.parameterChanged(paramID, newValue);

	if (paramID == NAME_FB || paramID == NAME_NEGATIVE_FB || paramID == NAME_FREQUENCY)
		resonatorUnit.parameterChanged(paramID, newValue);

	if (paramID == NAME_FINE_TUNE)
	{
		fineTune = newValue;
		resonatorUnit.setDelayTime(currentMidiNote, fineTune);
	}
}

void SynthVoice::setDrySignal(const AudioBuffer<float> dryBuffer, int startSample, int numSamples)
{
	for (int ch = 0; ch < dryBuffer.getNumChannels(); ++ch)
		voiceBuffer.copyFrom(ch, startSample, dryBuffer, ch, startSample, numSamples);
}

void SynthVoice::renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{		
	resonatorUnit.processBlock(voiceBuffer, startSample, numSamples);

	if (isVoiceActive())
	{
		adsr.applyEnvelopeToBuffer(voiceBuffer, startSample, numSamples);

		for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
			outputBuffer.addFrom(ch, startSample, voiceBuffer, ch, startSample, numSamples, currentVelocity);

		if (!adsr.isActive())
			clearCurrentNote();
	}
};