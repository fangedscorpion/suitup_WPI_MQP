#include "incorrectdatalengthexception.h"

IncorrectDataLengthException::IncorrectDataLengthException(int len) throw() :QException()
{
    intendedLength = len;
}

