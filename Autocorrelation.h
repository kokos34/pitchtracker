
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


// Remember to use a different guard symbol in each header!

#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

#include <vamp-sdk/Plugin.h>
#include <vector>
#include <memory>
#include <iostream>

using std::string;

class Autocorrelation : public Vamp::Plugin
{
public:
    Autocorrelation(float inputSampleRate);
    virtual ~Autocorrelation();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    OutputList getOutputDescriptors() const;

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();

protected:
    // plugin-specific data and methods go here
	size_t m_blockSize;
    float m_inputSampleRate;
    float findSetAutocorrelationFunction(std::vector<float> samples, int m);
    int findFirstMinimumInAC(std::vector<float> autocorrelationFunction);

};

#endif

