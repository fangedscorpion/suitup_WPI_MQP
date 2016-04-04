#ifndef INCORRECTDATALENGTHEXCEPTION_H
#define INCORRECTDATALENGTHEXCEPTION_H

#include <QException>

class IncorrectDataLengthException : public QException
{
public:
    IncorrectDataLengthException(int intendedDataLength) throw();
    ~IncorrectDataLengthException() throw() {}
    int getIntendedLength() {return intendedLength; }
    void raise() const {throw *this;}
    IncorrectDataLengthException *clone() const { return new IncorrectDataLengthException(intendedLength); }


private:
    int intendedLength;
};

#endif // INCORRECTDATALENGTHEXCEPTION_H
