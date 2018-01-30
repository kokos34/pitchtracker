#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "fftw3.h"

using namespace::std;

int size;

float* readSamples(const char* name)
{
    ifstream myfile;
    vector<float> frequencies;
    string line;
    myfile.open(name, ifstream::in);

	fftw_complex* in;

    if(myfile.good())
    {
        while(getline(myfile, line))
        {
            frequencies.push_back(atof(line.c_str()));
        }
        myfile.close();
    }    
    else
        return NULL;

    size = frequencies.size();

    float* frequenciesBuffer = new float[frequencies.size()];
    for(int i = 0; i < frequencies.size(); i++)
        frequenciesBuffer[i] = frequencies[frequencies.size() - i];

    return frequenciesBuffer;
}

int main(int argc, char *argv[])
{
    float* frequencies = readSamples("out.txt");
    const float sampleRate = 44100.0;

    const long totalSize = size * 1024 / 44100.0;
    cout << totalSize << endl;

    double* buffer = new double[totalSize];
    int f = 1;

    for(int i = 0; i < totalSize; i++)
    {
        if(i % 1024 == 0)
            f++;

        double t = double(i) / (double)sampleRate;
        buffer[i] = sin(2.0 * M_PI * t * frequencies[f]);
//        cout << "b[" << i << "]= " << buffer[i] << endl;
    }
    cout << "out" << endl;

    SF_INFO info;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    info.channels = 1;
    info.samplerate = sampleRate;
    cout << "2" << endl;

    // Open sound file for writing
    SNDFILE *sndFile = sf_open("out.wav", SFM_WRITE, &info);
    if (sndFile == NULL)
    {
        fprintf(stderr, "Error opening sound file '%s': %s\n", "out.wav", sf_strerror(sndFile));
        free(buffer);
        return -1;
    }

    // Write frames
    cout << "3" << endl;
    long writtenFrames = sf_writef_double(sndFile, buffer, totalSize);

    // Check correct number of frames saved
    if (writtenFrames != totalSize)
    {
        fprintf(stderr, "Did not write enough frames for source\n");
        sf_close(sndFile);
        free(buffer);
        return -1;
    }

    sf_write_sync(sndFile);
    sf_close(sndFile);
    free(buffer);

    return 0;
}
