#pragma once

#include <fstream>

#include "basic_target.hpp"

namespace easyout
{

namespace
{

class FileTarget : public Target
{
public:
    FileTarget() : ofs("output.txt") {}

    FileTarget(const std::string& filename) : ofs(filename) {}

    ~FileTarget()
    {
        namespace f = format;

        ofs << f::newline
            << "[Message] Writing finished successfully on file target!"
            << f::newline << "[Statistics]" << f::newline << " [Severity]"
            << f::newline;

        for (std::size_t i = 0; i < 16; ++i)
        {
            if (s_keyhole.test(i))
            {
                ofs << "  " << std::setw(10) << s_statistics[i]->counter << ' '
                    << s_statistics[i]->keyname << " message"
                    << f::newline;
            }
        }

        ofs << " [End of severity statistics]" << f::newline
            << " [Origin of log]" << f::newline;

        for (std::size_t i = 0; i < 16; ++i)
        {
            if (o_keyhole.test(i))
            {
                ofs << "  " << std::setw(10) << o_statistics[i]->counter << ' '
                    << o_statistics[i]->keyname << " message"
                    << f::newline;
            }
        }

        ofs << " [End of origin statistics]" << f::newline
            << "[End of statistics]" << f::newline;
    }

    void writeToTarget(const std::ostringstream& oss)
    {
        ofs << oss.str();
    }

private:
    std::ofstream ofs;

};

}//hidden namespace

std::shared_ptr<Target> openFileTarget(const std::string& filename)
{
    std::shared_ptr<Target> ptr = std::make_shared<FileTarget>(filename);
    return ptr;
}

void setFileNormally(std::shared_ptr<Target> ptr,
                     const bool verboseFormat = true,
                     const bool verboseNames = true
                     )
{
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

}//namespace easyout
