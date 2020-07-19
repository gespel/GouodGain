

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class GouodGainAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    GouodGainAudioProcessorEditor (GouodGainAudioProcessor&);
    ~GouodGainAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
    Slider sliderVolume;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttach;
private:
    void sliderValueChanged(Slider* slider) override;
    GouodGainAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GouodGainAudioProcessorEditor)
};
