#pragma once

#include <JuceHeader.h>

class SynthSound : public SynthesiserSound
{
	bool appliesToNote(int midiNoteNumber) override { return true; }
	bool appliesToChannel(int midiChannel) override { return true; }
};