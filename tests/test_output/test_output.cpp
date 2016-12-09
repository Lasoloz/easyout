////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Heim László
//
// This library is distributed under MIT License
// (https://opensource.org/licenses/MIT)
//
////////////////////////////////////////////////////////////////////////////////

#include <thread>

#include "../../include/easyout/output.hpp"
#include "../../include/easyout/cout_target.hpp"
#include "../../include/easyout/file_target.hpp"

easyout::Output logger;
easyout::Output out2;

void threadFunc()
{
    for (unsigned int i = 0; i < 100; ++i)
    {
        logger(easyout::Severity::Info, easyout::Origin::Threading, "Some message ", i);
        out2(easyout::Severity::Warning, easyout::Origin::Network, "Other message ", i);
    }
}

int main()
{
    easyout::setCoutNormally();
    logger.addTarget(easyout::getCoutTarget());
    logger.addTarget("log.txt");
    out2.addTarget(easyout::getCoutTarget());

    std::thread thr1(threadFunc);
    std::thread thr2(threadFunc);

    for (unsigned int i = 0; i < 100; ++i)
    {
        logger(easyout::Severity::Error, easyout::Origin::Core, "From main!", i);
    }

    thr1.join();
    thr2.join();

    logger(easyout::Severity::Info, easyout::Origin::Core, "Threads finished!");

    return 0;
}
