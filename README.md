# Colorizer-VST

**Last update: April 2022**

A MIDI-controlled polyphonic resonator VST plugin.

<p align="center">
  <img src="Images/Colorizer.png" alt="ColorizerGUI">
</p>

Colorizer is a MIDI-controlled polyphonic resonator. The algorithm adjusts the delay time of up to eight independent fractional delay lines with feedback to generate pitch-specific resonances in the input signal, based on the MIDI notes played by the user.

## Block Diagram

<p align="center">
  <img src="Images/SignalPath.png" alt="SignalPath">
</p>

## Parameters

- ADSR: controls the amplitude envelope applied to the wet signal.
- Feedback: adjusts the intensity and duration of the resonances by setting the feedback level of the delay lines.
- Negative: inverts the sign of the feedback multiplication factor, emphasizing odd harmonics over even ones.
- Wet Amount: sets the level of the wet signal.
- Damping: sets the cutoff frequency of the low-pass filter in the feedback path, affecting how quickly high frequencies decay.
- Tune: allows fine-tuning of the overall pitch of the resonated signal.

## Notes

The JUCE project was created using Juce version 6.0.8, and was compiled using c++14 and Visual Studio 2019.
