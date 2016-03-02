#include "incorrectdatalengthexception.h"

IncorrectDataLengthException::IncorrectDataLengthException(int len):QException()
{
    intendedLength = len;
}

