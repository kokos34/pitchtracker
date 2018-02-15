#include "MaxValues.h"

MaxValues::MaxValues()
{

}

size_t MaxValues::getIndex()
{
	return index;
}
float MaxValues::getValue()
{
	return value;
}
void MaxValues::insertIndex(size_t currIndex)
{
	index = currIndex;
}
void MaxValues::insertValue(float currValue)
{
	value = currValue;
}
