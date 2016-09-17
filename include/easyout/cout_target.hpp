////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Heim László
//
// This library is distributed under MIT License
// (https://opensource.org/licenses/MIT)
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>

#include "basic_target.hpp"

namespace easyout
{

namespace
{

class coutTarget : public Target
{
public:
    ~coutTarget()
    {
        namespace f = format;

        std::cout << f::newline
                  << "[Message] Writing finished successfully on cout target!"
                  << f::newline << "[Statistics]" << f::newline << " [Severity]"
                  << f::newline;

        for (std::size_t i = 0; i < 16; ++i)
        {
            if (s_keyhole.test(i))
            {
                std::cout << "  " << std::setw(10) << s_statistics[i]->counter
                          << ' ' << s_statistics[i]->keyname << " message"
                          << f::newline;
            }
        }

        std::cout << " [End of severity statistics]" << f::newline
                  << " [Origin of log]" << f::newline;

        for (std::size_t i = 0; i < 16; ++i)
        {
            if (o_keyhole.test(i))
            {
                std::cout << "  " << std::setw(10) << o_statistics[i]->counter
                          << ' ' << o_statistics[i]->keyname << " message"
                          << f::newline;
            }
        }

        std::cout << " [End of origin statistics]" << f::newline
                  << "[End of statistics]" << f::newline;
    }

    void writeToTarget(const std::ostringstream& oss)
    {
        std::cout << oss.str();
    }

};

std::shared_ptr<Target> coutTarget_ptr;

}//hidden namespace

//Get the cout target
inline std::shared_ptr<Target> getCoutTarget()
{
    if (coutTarget_ptr == nullptr)
    {
        coutTarget_ptr = std::make_shared<coutTarget>();
    }

    return coutTarget_ptr;
}

//Set the cout target normally (all severities and origins)
inline void setCoutNormally(const bool verboseFormat = false,
                            const bool verboseNames = false
                            )
{
    std::shared_ptr<Target> ptr(getCoutTarget());
    addAllDefaultSeverityKeys(ptr, verboseNames);
    addAllDefaultOriginKeys(ptr, verboseNames);
    if (verboseFormat)
    {
        ptr->setFormat();
    }
    else
    {
        ptr->setFormat("[%i] [%H:%M:%S.%f] [%s] [%o] %v");
    }
}

//Set the cout target with less options (less severity and all origins)
inline void setCoutRestricted(const bool verboseFormat = false,
                              const bool verboseNames = false
                              )
{
    std::shared_ptr<Target> ptr(getCoutTarget());
    addDefaultSeverityKey(ptr, Severity::Trace, verboseNames);
    addDefaultSeverityKey(ptr, Severity::Debug, verboseNames);
    addDefaultSeverityKey(ptr, Severity::Error, verboseNames);
    addDefaultSeverityKey(ptr, Severity::Critical, verboseNames);
    addAllDefaultOriginKeys(ptr, verboseNames);
    if (verboseFormat)
    {
        ptr->setFormat();
    }
    else
    {
        ptr->setFormat("[%i] [%H:%M:%S.%f] [%s] [%o] %v");
    }
}

}//namespace easyout
