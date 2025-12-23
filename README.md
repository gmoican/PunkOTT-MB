# PunkOTT-MB

 [![PunkOTT-MB](https://github.com/gmoican/PunkOTT-MB/actions/workflows/build.yml/badge.svg)](https://github.com/gmoican/PunkOTT-MB/actions/workflows/build.yml)

![DemoImage](docs/images/demo.png)

## Introduction
This is a VST3/AU OTT-style compressor plugin made with [JUCE](https://juce.com/). OTT stands for Over-The-Top, it is a very aggressive dynamic effect that combines upward compression (raises the level of quieter sounds, making them louder) and downward compression (lowers the level of louder sounds, limiting the peaks). This combination results in a dense, loud and harmonically rich sound; check the demo below to hear some practical examples.

--- THERE WILL BE A VIDEO LINK HERE WHEN THE DEMO IS READY ---

The original OTT compressor was developed in Ableton Live's Multiband Dynamics and Xfer Records replicated this effect in a [free plugin](https://splice.com/plugins/3788-ott-vst-au-by-xfer-records). My plugin is simply a curious attempt to recreate this chain of processes with my own spin.

This repository contains the multi-band implementation of this effect. Check [this repo](https://github.com/gmoican/PunkOTT) for the single-band implementation.

## Actual state of this repository
I am adjusting the [single-band version](https://github.com/gmoican/PunkOTT) to a multi-band version so my scope is not big for this project. Here are some aspects that could be considered for further improvements:
* **DSP**:
    * Investigating different compressor algorithms. I've implemented a feed-forward and feed-back processing in my `Compressor` processor. It can be a fun experiment to find new algorithms and experiment with the behaviour of the overall plugin.
* **GUI**:
    * Graphic design is not my passion and I don't like my GUI... It is the best I could do without overstraining myself...
* **Documentation**:
    * Documentation is boring, but I promise to write a detailed `Readme` one of these days...
* **Extras**:
    * I am working on a JUCE module to have all my DSP classes in one place. That is a project for another day, but I wanted to mention it here.

