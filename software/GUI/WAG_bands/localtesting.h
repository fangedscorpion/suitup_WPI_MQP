#ifndef LOCALTESTING_H
#define LOCALTESTING_H

// comment this out if not in testing mode
#define TESTING


class LocalTesting
{
public:
#ifdef TESTING
    static bool inTestingMode() { return true; }
#else
    static bool inTestingMode() { return false; }
#endif
};


#endif // LOCALTESTING_H

