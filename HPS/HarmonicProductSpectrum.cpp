
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


#include "HarmonicProductSpectrum.h"

using namespace::std;

HarmonicProductSpectrum::HarmonicProductSpectrum(float inputSampleRate) :
    Plugin(inputSampleRate),
    m_blockSize(2048),
    m_inputSampleRate(44100.0)
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
{

}

HarmonicProductSpectrum::~HarmonicProductSpectrum()
{
}

string
HarmonicProductSpectrum::getIdentifier() const
{
    return "HPS";
}

string
HarmonicProductSpectrum::getName() const
{
    return "Harmonic Product Spectrum";
}

string
HarmonicProductSpectrum::getDescription() const
{
    // Return something helpful here!
    return "HPS function";
}

string
HarmonicProductSpectrum::getMaker() const
{
    // Your name here
    return "";
}

int
HarmonicProductSpectrum::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
HarmonicProductSpectrum::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "";
}

HarmonicProductSpectrum::InputDomain
HarmonicProductSpectrum::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
HarmonicProductSpectrum::getPreferredBlockSize() const
{
    return 2048; // 0 means "I can handle any block size"
}

size_t
HarmonicProductSpectrum::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
HarmonicProductSpectrum::getMinChannelCount() const
{
    return 1;
}

size_t
HarmonicProductSpectrum::getMaxChannelCount() const
{
    return 1;
}

HarmonicProductSpectrum::ParameterList
HarmonicProductSpectrum::getParameterDescriptors() const
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
    d.name = "Fundamental frequency";
    d.description = "Fundamental frequency of given frame";
    d.unit = "Hz";
    d.minValue = 0;
    d.maxValue = 20000;
    d.isQuantized = false;
    list.push_back(d);

    return list;
}

float
HarmonicProductSpectrum::getParameter(string identifier) const
{
    if (identifier == "ff") {
        return 5; // return the ACTUAL current value of your parameter here!
    }
    return 0;
}

void
HarmonicProductSpectrum::setParameter(string identifier, float value)
{
    if (identifier == "parameter") {
        // set the actual value of your parameter
    }
}

HarmonicProductSpectrum::ProgramList
HarmonicProductSpectrum::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
HarmonicProductSpectrum::getCurrentProgram() const
{
    return ""; // no programs
}

void
HarmonicProductSpectrum::selectProgram(string name)
{
}

HarmonicProductSpectrum::OutputList
HarmonicProductSpectrum::getOutputDescriptors() const
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
HarmonicProductSpectrum::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
        channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!

    return true;
}

void
HarmonicProductSpectrum::reset()
{
    // Clear buffers, reset stored values, etc
}

HarmonicProductSpectrum::FeatureSet
HarmonicProductSpectrum::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    FeatureSet fs;

    float* spectrum = getSpectrum(inputBuffers);
    float ff = (float)m_inputSampleRate / (float)m_blockSize * findFundamentalFrequency(spectrum);
            //(44100.0 / 1024.0) * findFundamentalFrequency(spectrum);

//    if(ff > 10000)
//        ff = 0.0;

    Feature f;
    f.values.push_back(ff);

    fs[0].push_back(f);
    return fs;
}

float* HarmonicProductSpectrum::getSpectrum(const float *const *inputBuffers)
{
    float* spectrum = new float[m_blockSize/2];

    for(unsigned int n = 0; n < m_blockSize / 2; n++)
    {
        float real = (inputBuffers[0][n * 2]);// * 0.5 * (1.0 - cos(2.0 * M_PI * n / (m_blockSize/2 - 1))));
        float img = (inputBuffers[0][n * 2 + 1]);//   * 0.5 * (1.0 - cos(2.0 * M_PI * n / (m_blockSize/2 - 1))));

        spectrum[n] = std::sqrt(real* real + img * img);
    }
    return spectrum;
}

float HarmonicProductSpectrum::findFundamentalFrequency(float* spectrum)
{
    float* result = findCorrelationArray(spectrum);
    return (float)FindMaxInCorrelationArray(result);
}

float* HarmonicProductSpectrum::findCorrelationArray(float* spectrum)
{
    //const int multiplier = 3;
    const short order = 15;
    int spectrumLen = m_blockSize / 2;
    int HPSLen = spectrumLen / (order+1);
//    cout <<" len "<< HPSLen << endl;
    float* hps = new float[spectrumLen];

    for (int i = 0; i < spectrumLen; i++)
    {
        if(i < HPSLen)
            hps[i] = spectrum[i];
        else
            hps[i] = -100000000000;
    }

    // do every harmonic in a big loop:
    for (int harmonic = 1; harmonic <= order; harmonic++)
    {
        int downsamplingFactor = harmonic + 1;
        for (int index = 0; index < HPSLen; index++)
        {
            // Calculate the average (downsampling):
            float avg = 0;
            for (int i = 0; i < downsamplingFactor; i++)
                avg += spectrum[index * downsamplingFactor + i];

            hps[index] += avg / downsamplingFactor;
        }
    }
    return hps;

//    float* result = new float[m_blockSize / 2];

//    for(size_t i = 0; i < m_blockSize/2; i++)
//        result[i] = 1.0000000000000;

//    for(size_t w = 1; w < m_blockSize/2; w++)
//    {
//        float avg = 0;
//        for(size_t r = 1; r < multiplier;  r++)
//        {
//            if(w*r < m_blockSize/2)
//                result[w] *= fabs(spectrum[w * r]);
//        }
//        //result[w] = initialPeak;

//    }
//    return result;
}

size_t HarmonicProductSpectrum::FindMaxInCorrelationArray(float* corArray)
{
    float max = 0.0;
    size_t period = 0;
    int HPSLen = (m_blockSize/2) / 16;

    for(size_t i = 1; i < HPSLen; i++)
    {
        if(corArray[i] > max)
        {
            period = i;
            max = corArray[i];
        }
    }

    corArray[period] = 0.0;

//    //REMOVING OCTAVE ERRORS
    float secondMax = 0.0;
    size_t secondPeriod = 0;

    for(size_t i = 0; i < HPSLen; i++)
    {
        if(corArray[i] > secondMax)
        {
            secondPeriod = i;
            secondMax = corArray[i];
        }
    }
    float diff = (2*secondMax - max);
    if(diff < 0 && secondMax / max > 0.05)
    {
        cout << "in " << max << " " << secondMax << " diff = " << diff << endl;
        period = secondPeriod;
    }

    return period;
}

HarmonicProductSpectrum::FeatureSet
HarmonicProductSpectrum::getRemainingFeatures()
{
    return FeatureSet();
}

