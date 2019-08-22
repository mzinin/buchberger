#ifndef VERSION_H
#define VERSION_H

#include "current_version.h"

class VersionInfo
{
private:
    unsigned major;
    unsigned minor;
    unsigned revision;

public:
    VersionInfo(unsigned initialMajor, unsigned initialMinor, unsigned initialRevision);
    unsigned GetMajor() const;
    unsigned GetMinor() const;
    unsigned GetRevision() const;
};

const VersionInfo& GetVersion();
#endif
