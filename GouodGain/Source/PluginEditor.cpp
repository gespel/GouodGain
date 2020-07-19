

#include "PluginProcessor.h"
#include "PluginEditor.h"

GouodGainAudioProcessorEditor::GouodGainAudioProcessorEditor (GouodGainAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    sliderVolume.setSliderStyle(Slider::SliderStyle::Rotary);
    sliderVolume.setRange(0.0, 10.0, 0.01);
    sliderVolume.setTextValueSuffix(" Gain");
    sliderVolume.setTextBoxStyle(Slider::NoTextBox, true, 100, 25);
    sliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, "gain", sliderVolume);
    sliderVolume.addListener(this);
    addAndMakeVisible(&sliderVolume);
    sliderVolume.setVisible(true);
    setSize (140, 140);
    processor.gain = sliderVolume.getValue();
}

GouodGainAudioProcessorEditor::~GouodGainAudioProcessorEditor()
{
}
void GouodGainAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    processor.gain = sliderVolume.getValue();
}
void GouodGainAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("By GouodAudio", getLocalBounds(), Justification::bottomRight, 1);
}

void GouodGainAudioProcessorEditor::resized()
{
    sliderVolume.setBounds(5, 5, 130, 130);
}
