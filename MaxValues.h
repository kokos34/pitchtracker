#include <iostream>

class MaxValues
{
public:
    MaxValues();
    size_t getIndex();
    float getValue();
    void insertIndex(size_t currIndex);
    void insertValue(float currValue);
    
private:
    size_t index;
    float value;
};
