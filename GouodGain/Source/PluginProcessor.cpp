#include "PluginProcessor.h"
#include "PluginEditor.h"

GouodGainAudioProcessor::GouodGainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                #if ! JucePlugin_IsMidiEffect
                    #if ! JucePlugin_IsSynth
                        .withInput("Input", AudioChannelSet::stereo(), true)
                    #endif
                    .withOutput("Output", AudioChannelSet::stereo(), true)
                #endif
    ),
    parameters(*this, nullptr, Identifier("GouodGain"),
        {
            std::make_unique<AudioParameterFloat>("gain",            // parameter ID
                                                   "Gain",            // parameter name
                                                   0.0f,              // minimum value
                                                   10.0f,              // maximum value
                                                   0.5f),             // default value
        })
#endif
{
    gainParameter = parameters.getRawParameterValue ("gain");
    
}

GouodGainAudioProcessor::~GouodGainAudioProcessor()
{
}

const String GouodGainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GouodGainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GouodGainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GouodGainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GouodGainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GouodGainAudioProcessor::getNumPrograms()
{
    return 1;

}

int GouodGainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GouodGainAudioProcessor::setCurrentProgram (int index)
{
}

const String GouodGainAudioProcessor::getProgramName (int index)
{
    return {};
}

void GouodGainAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GouodGainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gain = *gainParameter;
}

void GouodGainAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GouodGainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GouodGainAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            channelData[sample] *= gain;
        }
    }
}

//==============================================================================
bool GouodGainAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* GouodGainAudioProcessor::createEditor()
{
    return new GouodGainAudioProcessorEditor (*this);
}

//==============================================================================
void GouodGainAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    MemoryOutputStream(destData, true).writeFloat(gain);
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GouodGainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
    gain = MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat();
}
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GouodGainAudioProcessor();
}
