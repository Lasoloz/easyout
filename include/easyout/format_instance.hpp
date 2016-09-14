////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Heim László
//
// This library is distributed under MIT License
// (https://opensource.org/licenses/MIT)
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <chrono>
#include <thread>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>

namespace easyout
{

namespace format
{

namespace sc = std::chrono;

struct FormatMeta
{
    FormatMeta(const std::string& _message,
               const std::string& _severity,
               const std::string& _origin
               ) : message(_message), severity(_severity), origin(_origin)
    {
        time = sc::system_clock::now();
    }

    const std::string& message,& severity,& origin;
    sc::system_clock::time_point time;

};



class FormatInstance
{
public:
    FormatInstance() {}

    //Non-copyable object
    FormatInstance(const FormatInstance&) = delete;
    FormatInstance& operator= (const FormatInstance&) = delete;
    //Non-movable object
    FormatInstance(FormatInstance&&) = delete;
    FormatInstance& operator= (FormatInstance&&) = delete;

    virtual void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        oss << mes.message;
    }

};

class ConstStrFormatInstance : public FormatInstance
{
public:
    ConstStrFormatInstance() : const_str("Default") {}
    ConstStrFormatInstance(const std::string& cstr) : const_str(cstr) {}

    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        oss << const_str;
    }

private:
    const std::string const_str;

};

class ThreadIdFormatInstance : public FormatInstance
{
public:
    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        oss << std::setw(4) << std::this_thread::get_id();
    }

};

class DateTimeFormatInstance : public FormatInstance
{
public:
    DateTimeFormatInstance() : datetimeformat("%Y-%m-%s %H:%M:%S") {}
    DateTimeFormatInstance(const std::string& fmtstr
                           ) : datetimeformat(fmtstr) {}

    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        std::time_t tt = sc::system_clock::to_time_t (mes.time);
        struct std::tm * ptm = std::localtime(&tt);
        oss << std::put_time(ptm, datetimeformat.c_str());
    }

private:
    std::string datetimeformat;

};

class SeverityFormatInstance : public FormatInstance
{
public:
    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        oss << mes.severity;
    }

};

class OriginFormatInstance : public FormatInstance
{
public:
    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        oss << mes.origin;
    }

};

class MilliFormatInstance : public FormatInstance
{
public:
    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        sc::milliseconds ms = sc::duration_cast<sc::milliseconds>
                                (mes.time.time_since_epoch()
                                 );
        oss << std::setw(3) << (ms.count() % 1000);
    }

};

class MicroFormatInstance : public FormatInstance
{
public:
    void write(std::ostringstream& oss, const FormatMeta& mes) const
    {
        sc::microseconds ms = sc::duration_cast<sc::microseconds>
                                (mes.time.time_since_epoch()
                                 );
        oss << std::setw(6) << (ms.count() % 1000000);
    }

};

}//namespace format

}//namespace easyout
