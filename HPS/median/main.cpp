#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace::std;

int NumberOfFrames = 0;

float* parseToArray(vector<float> in)
{
    float* array = new float[in.size()];
    for(int i = 0; i < in.size(); i++)
        array[i] = in[i];

    return array;
}

float** divideIntoElements(float* input)
{
    //int allElements = int(sizeof(input)/sizeof(float)) / 9;
    //NumberOfFrames = allElements;

    float** output = new float*[NumberOfFrames];

    for(int i = 0; i < NumberOfFrames; i++)
        output[i] = new float[9];

    for(int i = 0; i < NumberOfFrames; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            output[i][j] = input[i * 9 + j];

        }
    }

    return output;
}

int partition(float tablica[], int p, int r) // dzielimy tablice na dwie czesci, w pierwszej wszystkie liczby sa mniejsze badz rowne x, w drugiej wieksze lub rowne od x
{
    float x = tablica[p]; // obieramy x
    int i = p, j = r, w; // i, j - indeksy w tabeli
    while (true) // petla nieskonczona - wychodzimy z niej tylko przez return j
    {
         while (tablica[j] > x) // dopoki elementy sa wieksze od x
            j--;
         while (tablica[i] < x) // dopoki elementy sa mniejsze od x
            i++;
         if (i < j) // zamieniamy miejscami gdy i < j
         {
            w = tablica[i];
            tablica[i] = tablica[j];
            tablica[j] = w;
            i++;
            j--;
         }
         else // gdy i >= j zwracamy j jako punkt podzialu tablicy
            return j;
    }
}

void quicksort(float tablica[], int p, int r) // sortowanie szybkie
{
    int q;
    if (p < r)
    {
        q = partition(tablica,p,r); // dzielimy tablice na dwie czesci; q oznacza punkt podzialu
        quicksort(tablica, p, q); // wywolujemy rekurencyjnie quicksort dla pierwszej czesci tablicy
        quicksort(tablica, q+1, r); // wywolujemy rekurencyjnie quicksort dla drugiej czesci tablicy
    }
}

float calculateMedian(float* inputFrames)
{
    float* input = inputFrames;
    quicksort(input, 0, 8);

    return (input[3] + input[4])/2;
}

void contructOutputFile(vector<float> input)
{
    float* output = new float[input.size() * 9];

    for(int i = 0; i < input.size(); i++)
    {
        for(int j = 0; j < 9; j++)
            output[i * 9 + j] = input[i];
    }

    ofstream outfile;
    outfile.open("output_median.txt", ofstream::out);
    size_t size = 9 * input.size();

    for(int i = 0; i < size; i++)
        outfile << output[i] << endl;

    outfile.close();
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "wrong input" << std::endl;
        return 1;
    }

    ifstream inputFile;
    inputFile.open(argv[1]);
    vector<float> frequencies;

    if (inputFile.is_open())
    {
        while(!inputFile.eof())
        {
            float currNumber;
            inputFile >> currNumber;
            frequencies.push_back(currNumber);
        }
    }

    inputFile.close();

    NumberOfFrames = (int)(frequencies.size() / 9);
    float* freq = parseToArray(frequencies);
    float** frequenciesInFrames = divideIntoElements(freq);

    vector<float> resultingVector;

    for(int i = 0; i < NumberOfFrames; i++)
    {
        float medianOfFrame = calculateMedian(frequenciesInFrames[i]);
        resultingVector.push_back(medianOfFrame);
    }

    contructOutputFile(resultingVector);
    delete[] frequenciesInFrames;
    delete freq;

    return 0;
}
