
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


#include "HarmonicProductSpectrum.h"

using namespace::std;

HarmonicProductSpectrum::HarmonicProductSpectrum(float inputSampleRate) :
    Plugin(inputSampleRate),
    m_blockSize(1024),
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
    return 1024; // 0 means "I can handle any block size"
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

    fftw_complex* resultOfFourier = calculateFourier(inputBuffers);
    double* spectrum = getSpectrum(resultOfFourier);
    float ff = (m_inputSampleRate / (m_blockSize/2)) * findFundamentalFrequency(spectrum);

    Feature f;
    f.values.push_back(ff);

    fs[0].push_back(f);
    return fs;
}

fftw_complex* HarmonicProductSpectrum::calculateFourier(const float *const *inputBuffers)
{
    fftw_plan result;
    fftw_complex *output, *input;

    input = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_blockSize/2);
    output = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_blockSize/2);

    for(unsigned int n = 0; n < m_blockSize / 2; n++)
    {
        input[n][0] = static_cast<double>(inputBuffers[0][n * 2]) *
                0.5 * (1.0 - cos(2.0 * M_PI * n / (m_blockSize/2 - 1)));
        input[n][1] = static_cast<double>(inputBuffers[0][n * 2 + 1]);
    }

    result = fftw_plan_dft_1d((int)m_blockSize / 2,
                              input,
                              output,
                              FFTW_FORWARD,
                              FFTW_ESTIMATE);

    fftw_execute(result);
    fftw_destroy_plan(result);
    fftw_free(input);

    return output;
}

double* HarmonicProductSpectrum::getSpectrum(fftw_complex* fourierOfFrame)
{
    double* spectrum = new double[m_blockSize/2];
    cout << endl << endl;
    for(size_t i = 0; i < m_blockSize/2; i++)
    {
        double realPart = fourierOfFrame[i][0];
        double imaginaryPart = fourierOfFrame[i][0];

        spectrum[i] = realPart * realPart + imaginaryPart * imaginaryPart;
        cout << spectrum[i] << endl;
    }
    return spectrum;
}

double HarmonicProductSpectrum::findFundamentalFrequency(double* spectrum)
{
    int period = 0;
    double max = spectrum[0];
    for(size_t i = 1; i < m_blockSize/2; i++)
    {
        if(spectrum[i] > max)
        {
            period = i;
            max = spectrum[i];
        }
    }
    if(period == 0)
        return 0.0;
    else
        return 1 / period;
}

HarmonicProductSpectrum::FeatureSet
HarmonicProductSpectrum::getRemainingFeatures()
{
    return FeatureSet();
}

