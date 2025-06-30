/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditorGUI.h"

//==============================================================================
PolyphonicResonatorAudioProcessor::PolyphonicResonatorAudioProcessor() :
    parameters(*this, nullptr, Identifier("PolyphonicResonatorParameters"), addParameterLayout())
{

    for (auto i = 0; i < NUM_VOICES; ++i)
    {
        synth.addVoice(new SynthVoice());

        parameters.addParameterListener(NAME_ATTACK, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_DECAY, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_SUSTAIN, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_RELEASE, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_FB, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_NEGATIVE_FB, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_FREQUENCY, dynamic_cast<SynthVoice*>(synth.getVoice(i)));
        parameters.addParameterListener(NAME_FINE_TUNE, dynamic_cast<SynthVoice*>(synth.getVoice(i)));

    }
    parameters.addParameterListener(NAME_WET_AMOUNT, &dryWetManager);
    parameters.addParameterListener(NAME_WET_ONLY, this);

    synth.addSound(new SynthSound()); //no need to call distructor as the synthesiser will take care of it.
}

PolyphonicResonatorAudioProcessor::~PolyphonicResonatorAudioProcessor()
{
}

//==============================================================================
void PolyphonicResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (auto i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) //casting SynthetiserVoice to SynthVoice, as we need to call OUR prepareToPlay.
            voice->prepareToPlay(sampleRate, samplesPerBlock);       
    }

    dryWetManager.prepareToPlay(samplesPerBlock);
}

void PolyphonicResonatorAudioProcessor::releaseResources()
{
    for (auto i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            voice->releaseResources();
    }

    dryWetManager.releaseResources();
}

void PolyphonicResonatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    dryWetManager.storeFrom(buffer); // DRY SIGNAL

    buffer.applyGain(WET_SCALE_FACTOR); // SCALE FACTOR

    for (auto i = 0; i < NUM_VOICES; ++i)
    {
        dynamic_cast<SynthVoice*>(synth.getVoice(i))->setDrySignal(buffer, 0, buffer.getNumSamples());
    }

    buffer.clear();

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples()); //calls every voice's renderNextBlock.

    dryWetManager.applyWetAmount(buffer, buffer.getNumSamples()); // WET AMOUNT

    if (!wetOnly)
        dryWetManager.mergeTo(buffer); // DRY + WET
}

void PolyphonicResonatorAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_WET_ONLY)
        wetOnly = newValue > 0.5f;
}

//==============================================================================
bool PolyphonicResonatorAudioProcessor::hasEditor() const
{
    //return false;
    return true;
}

juce::AudioProcessorEditor* PolyphonicResonatorAudioProcessor::createEditor()
{
    //return nullptr;
    return new PluginEditorGUI(*this, parameters);
}

//==============================================================================
void PolyphonicResonatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState(); 
    std::unique_ptr<XmlElement> xml(state.createXml()); 
    copyXmlToBinary(*xml, destData);
}

void PolyphonicResonatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) 
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState)); 
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PolyphonicResonatorAudioProcessor();
}
