# PunkOTT-MB

 [![PunkOTT-MB](https://github.com/gmoican/PunkOTT-MB/actions/workflows/build.yml/badge.svg)](https://github.com/gmoican/PunkOTT-MB/actions/workflows/build.yml)

![DemoImage](docs/images/demo.png)

## Introduction
This is a VST3/AU OTT-style compressor plugin made with [JUCE](https://juce.com/). OTT stands for Over-The-Top, it is a very aggressive dynamic effect that combines upward compression (raises the level of quieter sounds, making them louder) and downward compression (lowers the level of louder sounds, limiting the peaks). This combination results in a dense, loud and harmonically rich sound; check the demo below to hear some practical examples.

--- THERE WILL BE A VIDEO LINK HERE WHEN THE DEMO IS READY ---

The original OTT compressor was developed in Ableton Live's Multiband Dynamics and Xfer Records replicated this effect in a [free plugin](https://splice.com/plugins/3788-ott-vst-au-by-xfer-records). My plugin is simply a curious attempt to recreate this chain of processes with my own spin.

This repository contains the multi-band implementation of this effect. Check [this repo](https://github.com/gmoican/PunkOTT) for the single-band implementation.

## Actual state of this repository
For the time being, I consider this project to be finished. I recognize that some aspects could be improved or customized for a more niche effect, but I prefer to learn more about other topics and come back to this in a distant future.

If you want to mess with the code and work on top of my plugin, here are some details that could be looked after:
* **DSP**:
    * Investigating different compressor algorithms. I've implemented a feed-forward and feed-back processing in my `Compressor` and `Lifter` processor. It can be a fun experiment to find new algorithms and experiment with the behaviour of the overall plugin.
    * The `Lifter` tends to increase the volume a lot. The plugin uses a limiter in the end of the chain for safety purposes and a clipper that the user can turn on/off.
    * The mids filtering produce a nasal/megaphone-like output sound. I am not a fan of this outcome so I may have to investigate it more...
* **GUI**:
    * Graphic design is not my passion and I don't like my GUI... It is the best I could do without overstraining myself...
* **Extras**:
    * There are some details noted in the [Inner Processing](#inner-processing) that could be looked after for further customization.
* **Documentation**:
    * Check out my [punk_dsp repo](https://github.com/gmoican/punk_dsp/) for detailed documentation about my `Compressor`, `Lifter`, `Gate` and `Waveshaper` implementations. Documentation is boring so I procastinate a little in that matter, but I promise to get it done and nicely presented one of these days... 

## Inner Processing
The signal flow is as follows:
1. Input Gain:
    - From -24 to +24dB.
2. Noise Gate:
    - Threshold: from -90dB to 0dB.
    - Ratio: 6:1.
    - Attack: 100ms.
    - Release: 30ms.
    - Mix: 90% _(if it is set to 100% the gate is very hard)_.
3. Multi-band filtering:
    - The signal is splitted in a lows band, mids band and highs band.
    - The filters used are JUCE's Linkwitz-Riley filters.
    - The mids filtering produce a nasal/megaphone-like output sound. I am not a fan of this outcome so I may have to investigate it more...
4. Lifter (for each band):
    - Threshold: from -80dB to 0dB.
        - When the threshold goes beyond -40dB, there is a gain compensation in the output to compensate for the crazy volume increase that the `Lifter` may produce.
        - *IDEA*: Allow the gain compensation to be turned on/off. If the output volume goes off the rails, the user can use a different compressor/limiter plugin to tame the output signal.
    - Ratio: 6:1.
    - Attack: from 1ms to 500ms.
    - Release: from 10ms to 2500ms.
    - Mix: from 0% to 100%.
5. Compressor (for each band):
    - Threshold: from -24dB to 0dB.
    - Ratio: 8:1.
    - Attack: from 0.1ms to 250ms.
    - Release: from 10ms to 2500ms.
    - Mix: from 0% to 100%.
6. Multi-band leveling:
    - Adjust the level of each band between -24dB and 12dB.
7. Limiter _(hard-coded for safety measures, the `Lifter` can easily produce pops in your signal so this limiter aims to prevent these spikes)_:
    - Threshold: -3dB.
    - Ratio: 20:1.
    - Attack: 30ms.
    - Release: 100ms.
    - Mix: 100%.
8. Clipper:
    - Applies an _TanH_ waveshaping function.
    - By default, it is turned on for the same reason as the `Limiter`. If you have another clipper plugin that you particularly like, I suggest you turn off my built-in clipper and use yours instead.
9. Output Gain:
    - From -24 to +24dB.
