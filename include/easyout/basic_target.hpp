#pragma once

#include <bitset>
#include <mutex>

#include "formatter.hpp"

namespace easyout
{

namespace Severity
{
    enum Severity_T : std::size_t
    {
        Trace=0,
        Debug,
        HiddenE,
        HiddenW,
        HiddenI,
        Error,
        Warning,
        Info,
        Critical,
        Message,
        AllDefaultSeverity
    };
}//namespace Severity

namespace Origin
{
    enum Origin_T : std::size_t
    {
        Core=0,
        IO,
        Threading,
        Network,
        Window,
        Graphics,
        Audio,
        AllDefaultOrigin
    };
}//namespace Origin

class Target
{
public:
    struct TargetMeta
    {
        TargetMeta(const std::string& newkeyname
                   ) :
                   counter(0),
                   keyname(newkeyname)
        {}

        unsigned int counter;
        std::string keyname;
    };

public:
    Target()
    {
        s_statistics.resize(16);
        o_statistics.resize(16);
    }

    inline void addSeverityKey(const std::size_t key, const std::string& keyname
                               )
    {
        if (key >= 0 && key < 16)
        {
            s_keyhole.set(key);
            s_statistics[key] = std::move(std::unique_ptr<TargetMeta>
                                          (new TargetMeta(keyname))
                                          );
        }
    }

    inline void addOriginKey(const std::size_t key, const std::string& keyname)
    {
        if (key >= 0 && key < 16)
        {
            o_keyhole.set(key);
            o_statistics[key] = std::move(std::unique_ptr<TargetMeta>
                                          (new TargetMeta(keyname))
                                          );
        }
    }

    inline void removeSeverityKey(const std::size_t key)
    {
        if (key>= 0 && key < 16)
        {
            o_keyhole.set(key, false);
            o_statistics[key].reset();
        }
    }

    void setFormat(const std::string& _fmt = "[%i] [%Y-%m-%d] [%H:%M:%S.%J] "
                   "[%s] [%o] %v"
                   )
    {
        fmt.setFormat(_fmt);
    }

    void write(const std::size_t severity, const std::size_t origin,
               const std::string& message
               )
    {
        if (testKeys(severity, origin))
        {
            //Lock the resource protection mutex
            std::lock_guard<std::mutex> lck(res_mtx);

            //Create an ostringstream for formatting
            std::ostringstream oss;

            //Format
            fmt.writeToOss(oss,
                           s_statistics[severity]->keyname,
                           o_statistics[origin]->keyname,
                           message
                           );
            //Write the ostringstream's content to the target
            writeToTarget(oss);
        }
    }

protected:
    inline bool testKeys(const std::size_t s_key,
                         const std::size_t o_key
                         )
    {
        if (s_keyhole.test(s_key) && o_keyhole.test(o_key))
        {
            s_statistics[s_key]->counter++;
            o_statistics[o_key]->counter++;
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual void writeToTarget(const std::ostringstream& oss)=0;

protected:
    std::mutex res_mtx;

    std::bitset<16> s_keyhole, o_keyhole;
    std::vector<std::unique_ptr<TargetMeta>> s_statistics, o_statistics;

    format::Formatter fmt;

};



//Add severity keys
void addDefaultSeverityKey(std::shared_ptr<Target> ptr,
                           const std::size_t which,
                           const bool fullName = true
                           )
{
    //Choose the correct name
    std::string name;
    switch (which)
    {
    case Severity::Trace:
        if (fullName)
        {
            name = "Trace   ";
        }
        else
        {
            name = "Trce";
        }
        break;
    case Severity::Debug:
        if (fullName)
        {
            name = "Debug   ";
        }
        else
        {
            name = "Dbg ";
        }
        break;
    case Severity::HiddenE:
    case Severity::Error:
        if (fullName)
        {
            name = "Error   ";
        }
        else
        {
            name = "Err ";
        }
        break;
    case Severity::HiddenW:
    case Severity::Warning:
        if (fullName)
        {
            name = "Warning ";
        }
        else
        {
            name = "Warn";
        }
        break;
    case Severity::HiddenI:
    case Severity::Info:
        if (fullName)
        {
            name = "Info    ";
        }
        else
        {
            name = "Info";
        }
        break;
    case Severity::Critical:
        if (fullName)
        {
            name = "Critical";
        }
        else
        {
            name = "Crit";
        }
        break;
    case Severity::Message:
        if (fullName)
        {
            name = "Message ";
        }
        else
        {
            name = "Msge";
        }
        break;
    default:
        if (fullName)
        {
            name = "DefaultS";
        }
        else
        {
            name = "DefS";
        }
    }

    //Add to the target
    ptr->addSeverityKey(which, name);
}

void addDefaultOriginKey(std::shared_ptr<Target> ptr,
                         const std::size_t which,
                         const bool fullName = true
                         )
{
    //Choose the correct name
    std::string name;
    switch (which)
    {
    case Origin::Core:
        if (fullName)
        {
            name = "Core     ";
        }
        else
        {
            name = "Core";
        }
        break;
    case Origin::IO:
        if (fullName)
        {
            name = "In/Output";
        }
        else
        {
            name = "IO  ";
        }
        break;
    case Origin::Threading:
        if (fullName)
        {
            name = "Threading";
        }
        else
        {
            name = "Thrd";
        }
        break;
    case Origin::Network:
        if (fullName)
        {
            name = "Network  ";
        }
        else
        {
            name = "Netw";
        }
        break;
    case Origin::Window:
        if (fullName)
        {
            name = "Window   ";
        }
        else
        {
            name = "Wind";
        }
        break;
    case Origin::Graphics:
        if (fullName)
        {
            name = "Graphics ";
        }
        else
        {
            name = "Grph";
        }
        break;
    case Origin::Audio:
        if (fullName)
        {
            name = "Audio    ";
        }
        else
        {
            name = "Aud ";
        }
        break;
    default:
        if (fullName)
        {
            name = "DefaultO ";
        }
        else
        {
            name = "DefO";
        }
    }

    //Add to the target
    ptr->addOriginKey(which, name);
}

void addAllDefaultSeverityKeys(std::shared_ptr<Target> ptr,
                               const bool fullName = true
                               )
{
    addDefaultSeverityKey(ptr, Severity::Trace   , fullName);
    addDefaultSeverityKey(ptr, Severity::Debug   , fullName);
    addDefaultSeverityKey(ptr, Severity::HiddenE , fullName);
    addDefaultSeverityKey(ptr, Severity::HiddenW , fullName);
    addDefaultSeverityKey(ptr, Severity::HiddenI , fullName);
    addDefaultSeverityKey(ptr, Severity::Error   , fullName);
    addDefaultSeverityKey(ptr, Severity::Warning , fullName);
    addDefaultSeverityKey(ptr, Severity::Info    , fullName);
    addDefaultSeverityKey(ptr, Severity::Critical, fullName);
    addDefaultSeverityKey(ptr, Severity::Message , fullName);
}

void addAllDefaultOriginKeys(std::shared_ptr<Target> ptr,
                             const bool fullName = true
                             )
{
    addDefaultOriginKey(ptr, Origin::Core     , fullName);
    addDefaultOriginKey(ptr, Origin::IO       , fullName);
    addDefaultOriginKey(ptr, Origin::Threading, fullName);
    addDefaultOriginKey(ptr, Origin::Network  , fullName);
    addDefaultOriginKey(ptr, Origin::Window   , fullName);
    addDefaultOriginKey(ptr, Origin::Graphics , fullName);
    addDefaultOriginKey(ptr, Origin::Audio    , fullName);
}

}//namespace easyout
