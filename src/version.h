#pragma once


class VersionInfo
{
public:
    VersionInfo(unsigned major, unsigned minor, unsigned revision);

    unsigned major() const;
    unsigned minor() const;
    unsigned revision() const;

private:
    const unsigned major_;
    const unsigned minor_;
    const unsigned revision_;
};

const VersionInfo& currentVersion();
