////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Heim László
//
// This library is distributed under MIT License
// (https://opensource.org/licenses/MIT)
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <forward_list>
#include <memory>
#include <sstream>

#include "basic_target.hpp"
#include "file_target.hpp"

namespace easyout
{

typedef std::forward_list<std::shared_ptr<Target>>::iterator TargIterator;

class Output
{
public:
    Output() {}
    ~Output() {}

    Output(const Output& other) : targets(other.targets) {}
    Output& operator= (const Output& other)
    {
        targets = other.targets;
        return *this;
    }

    Output(Output&& other) : targets(std::move(other.targets)) {}
    Output& operator= (Output&& other)
    {
        targets = std::move(other.targets);
        return *this;
    }

    TargIterator addTarget(std::shared_ptr<Target> targ)
    {
        targets.push_front(targ);
        return targets.before_begin();
    }

    TargIterator addTarget(const std::string& filename)
    {
        std::shared_ptr<Target> targ = std::make_shared<FileTarget>(filename);
        setFileNormally(targ);
        targets.push_front(targ);
        return targets.before_begin();
    }

    void removeTarget(std::forward_list<std::shared_ptr<Target>>::iterator& it)
    {
        targets.erase_after(it);
    }

    template <typename... Messages>
    void operator()(const std::size_t severity,
                    const std::size_t origin,
                    Messages... mes
                    )
    {
        std::ostringstream oss;
        addMessage(oss, mes...);
        for (auto& it : targets)
        {
            it->write(severity, origin, oss.str());
        }
    }

private:
    template <typename Message>
    void addMessage(std::ostringstream& oss, const Message  mes)
    {
        oss << mes;
    }

    template <typename Message, typename... Messages>
    void addMessage(std::ostringstream& oss,
                    const Message mes,
                    const Messages... allmes
                    )
    {
        oss << mes;
        addMessage(oss, allmes...);
    }

private:
    std::forward_list<std::shared_ptr<Target>> targets;

};

}//namespace easyout
