#include <iostream>
#include <sstream>

#include "../../include/easyout/formatter.hpp"

int main()
{
    namespace ef = easyout::format;

    ef::Formatter format("[%i] [%Y-%m-%d] [%H:%M:%S.%f] [%s] [%o] %v");

    std::ostringstream oss;

    format.writeToOss(oss, "Debug", "Core", "This is the actual message!");
    format.writeToOss(oss, "Warning", "Network", "This is another message!");

    format.setFormat("%i Year: %Y; Time: %H:%M:%S.%f %m-%d %s %o %v");

    format.writeToOss(oss, "Debug", "Core", "This is the actual message!");

    std::cout << oss.str();

    return 0;
}
