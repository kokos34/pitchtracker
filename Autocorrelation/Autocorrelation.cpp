
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


#include "Autocorrelation.h"

Autocorrelation::Autocorrelation(float inputSampleRate) :
    Plugin(inputSampleRate),
    m_blockSize(1024),
    m_inputSampleRate(44100.0),
    counter(0)
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
{

}

Autocorrelation::~Autocorrelation()
{
}

string
Autocorrelation::getIdentifier() const
{
    return "autocorrelation";
}

string
Autocorrelation::getName() const
{
    return "Autocorrelation Pitch Tracker";
}

string
Autocorrelation::getDescription() const
{
    // Return something helpful here!
    return "";
}

string
Autocorrelation::getMaker() const
{
    // Your name here
    return "";
}

int
Autocorrelation::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
Autocorrelation::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "";
}

Autocorrelation::InputDomain
Autocorrelation::getInputDomain() const
{
    return TimeDomain;
}

size_t
Autocorrelation::getPreferredBlockSize() const
{
    return 1024; // 0 means "I can handle any block size"
}

size_t 
Autocorrelation::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
Autocorrelation::getMinChannelCount() const
{
    return 1;
}

size_t
Autocorrelation::getMaxChannelCount() const
{
    return 1;
}

Autocorrelation::ParameterList
Autocorrelation::getParameterDescriptors() const
{
    ParameterList list;

    // If the plugin has no adjustable parameters, return an empty
    // list here (and there's no need to provide implementations of
    // getParameter and setParameter in that case either).

    // Note that it is your responsibility to make sure the parameters
    // start off having their default values (e.g. in the constructor
    // above).  The host needs to know the default value so it can do
    // things like provide a "reset to default" function, but it will
    // not explicitly set your parameters to their defaults for you if
    // they have not changed in the mean time.

    ParameterDescriptor d;
    d.identifier = "ff";
    d.name = "Fundamental Frequency";
    d.description = "Fundamental frequency of the frame";
    d.unit = "Hz";
    d.minValue = 0;
    d.maxValue = 20000;
    d.defaultValue = 0;
    d.isQuantized = false;
    list.push_back(d);

    return list;
}

float
Autocorrelation::getParameter(string identifier) const
{
    if (identifier == "ff") {
        return 5; // return the ACTUAL current value of your parameter here!
    }
    return 0;
}

void
Autocorrelation::setParameter(string identifier, float value) 
{
    if (identifier == "parameter") {
        // set the actual value of your parameter
    }
}

Autocorrelation::ProgramList
Autocorrelation::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
Autocorrelation::getCurrentProgram() const
{
    return ""; // no programs
}

void
Autocorrelation::selectProgram(string name)
{
}

Autocorrelation::OutputList
Autocorrelation::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "ff";
    d.name = "Fundamental frequency";
    d.description = "";
    d.unit = "";
    d.hasFixedBinCount = true;
    d.binCount = 1;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::OneSamplePerStep;
    d.hasDuration = false;
    list.push_back(d);

    return list;
}

bool
Autocorrelation::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!

    return true;
}

void
Autocorrelation::reset()
{
    // Clear buffers, reset stored values, etc
}

Autocorrelation::FeatureSet
Autocorrelation::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    float block_fundFreq = 0.0f;
//    std::allocator<float> alloc0;
    std::vector<float> input;

    FeatureSet fs;

    // Cast to vector
    for(size_t i = 0; i < m_blockSize; i++)
        input.push_back(inputBuffers[0][i]);

    // Find AC function
    std::vector<float> autocorrelationFunction;
    for(size_t m = 0; m < m_blockSize; m++)
        autocorrelationFunction.push_back(findSetAutocorrelationFunction(input, m));

    // Find its minumum
    int minimum = findFirstMinimumInAC(autocorrelationFunction);

    // If failed to find fundamental freq, just go
    // to next frame
    if(minimum == 0)
        block_fundFreq = 0;
    else
        block_fundFreq = 1.0 / minimum * m_inputSampleRate;

    Feature f;
    f.hasTimestamp = false;
    f.values.push_back(block_fundFreq);

    fs[0].push_back(f);
    return fs;
}
float Autocorrelation::findSetAutocorrelationFunction(std::vector<float> samples, int m)
{
    float autocorrelationFunction = 0.0;
    for(size_t i = 0; i < samples.size() - m; i++)
        autocorrelationFunction += (samples[i] * samples[i + m]);

    return autocorrelationFunction;
}

int Autocorrelation::findFirstMinimumInAC(std::vector<float> autocorrelationFunction)
{
    short framePeriod = 0;
    short shift = 20;
    int size = autocorrelationFunction.size();

    while(shift < size)
    {
        if (autocorrelationFunction[shift] > 0.7 * autocorrelationFunction[0])
        {
            for(int i = shift; i < size - 1; i++)
            {
                if(autocorrelationFunction[i] < autocorrelationFunction[i+1])
                {
                    framePeriod = i;
                    break;
                }
            }

            for(int i = framePeriod + 1; i < size - 1; i++)
            {
                if(autocorrelationFunction[i] > autocorrelationFunction[i+1])
                {
                    framePeriod = i;
                    break;
                }
            }
            break;
        }
        shift++;
    }
    return framePeriod;
}

size_t Autocorrelation::findMinimumInAscending(std::vector<float> autocorrelationFunction)
{
    size_t firstMax = 1;
    size_t minimumPos = 1;

    for(size_t i = 2; i < autocorrelationFunction.size() - 1; i++)
    {
        if(autocorrelationFunction[i-1] < autocorrelationFunction[i]
                && autocorrelationFunction[i] > autocorrelationFunction[i+1])
        {
            if(autocorrelationFunction[i-2] < autocorrelationFunction[i-1]
                    && autocorrelationFunction[i+1] > autocorrelationFunction[i+2])
            {
                firstMax = i;
                break;
            }
        }
    }

    for(size_t i = firstMax; i < autocorrelationFunction.size() - 3; i++)
    {
        float currValue = autocorrelationFunction[i];
        if(currValue < autocorrelationFunction[i+1]
                && currValue < autocorrelationFunction[i+2])
        {
            minimumPos = i;
            break;
        }
    }

    return minimumPos;
}

size_t Autocorrelation::findMinimumInDescending(std::vector<float> autocorrelationFunction)
{
    size_t minimumPos = 1;

    for(size_t i = 0; i < autocorrelationFunction.size() - 3; i++)
    {
        float currValue = autocorrelationFunction[i];
        if(currValue < autocorrelationFunction[i+1]
                && currValue < autocorrelationFunction[i+2])
        {
            minimumPos = i;
            break;
        }
    }

    return minimumPos;
}

Autocorrelation::FeatureSet
Autocorrelation::getRemainingFeatures()
{
    return FeatureSet();
}

