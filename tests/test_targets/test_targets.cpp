////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Heim László
//
// This library is distributed under MIT License
// (https://opensource.org/licenses/MIT)
//
////////////////////////////////////////////////////////////////////////////////

#include "../../include/easyout/cout_target.hpp"
#include "../../include/easyout/file_target.hpp"

int main()
{
    easyout::setCoutNormally();
    std::shared_ptr<easyout::Target> targ = easyout::getCoutTarget();
    std::shared_ptr<easyout::Target> alternative = easyout::getCoutTarget();
    std::shared_ptr<easyout::Target> file = easyout::openFileTarget("log.txt");
    easyout::setFileNormally(file);

    for (unsigned int i = 0; i < 10; ++i)
    {
        targ->write(easyout::Severity::Debug, easyout::Origin::Core, "Hello world!");
        alternative->write(easyout::Severity::Debug, easyout::Origin::Core, "Alternative");
        file->write(easyout::Severity::Warning, easyout::Origin::IO, "Testing warnings");
    }

    return 0;
}
