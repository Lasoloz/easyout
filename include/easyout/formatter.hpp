////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Heim László
//
// This library is distributed under MIT License
// (https://opensource.org/licenses/MIT)
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef DEBUG_EASYOUT
#include <iostream>
#endif // DEBUG_EASYOUT

#include <list>
#include <memory>

#include "format_instance.hpp"

namespace easyout
{

namespace format
{

#define CRLF_NEWLINE ///TODO: add OS specific macros somewhere and make it portable
#ifdef CRLF_NEWLINE
const std::string newline("\r\n");
#else
cosnt char newline = '\n';
#endif // CRLF_NEWLINE

class Formatter
{
public:
    //Constructors
    Formatter()
    {
        setFormat("[%i] [%Y-%m-%d] [%H:%M:%S.%q] [%s] [%o] %v");
    }
    Formatter(const std::string& fmt)
    {
        setFormat(fmt);
    }

    //Non-copyable object
    Formatter(const Formatter&) = delete;
    Formatter& operator= (const Formatter&) = delete;

    //Destructor
    ~Formatter() {}

    void setFormat(const std::string& _fmt)
    {
        //Clear previous content...
        fmt_list.clear();

        //Note
        //
        //The following character specifiers are not used by the std::put_time
        //function:
        //
        //% + (f, i, J, k, K, l, L, N, o, P, q, Q, s, v)
        //
        //From these, we'll use the following specifiers for our formatter:
        //%i - thread id
        //%s - severity object
        //%o - origin object
        //%v - actual message container
        //%f - milliseconds since last second
        //%q - microseconds since last second

        //We'll put %% to a DateTimeFormatInstance or to a const string
        //depending on the case (did the date/time part started or not)

        #ifdef DEBUG_EASYOUT
        std::cout << "[Formatter info]" << newline
                  << "[Formatter starting string:]" << _fmt << newline
                  << "[Formatter elements list]" << newline;
        #endif // DEBUG_EASYOUT

        bool startedConstString = false;
        bool startedDateTimeString = false;
        std::string fmt = _fmt;
        std::string::iterator strBeg, strEnd;

        for (std::string::iterator it = fmt.begin(); it < fmt.end(); ++it)
        {
            if (*it == '%')
            {
                ++it;
                if (*it == '%')
                {
                    //add to date/time part or to constant string part
                    if (!startedDateTimeString)
                    {
                        //Delete one of the '%'s
                        fmt.erase(it);
                        --it;//Get back to the correct iterator

                        if (!startedConstString)
                        {
                            strBeg = it;
                            startedConstString = true;
                        }
                    }

                    strEnd = it;
                }
                else if (*it == 'i' ||
                         *it == 's' ||
                         *it == 'o' ||
                         *it == 'v' ||
                         *it == 'f' ||
                         *it == 'q'
                         )
                {
                    //a new object
                    if (startedDateTimeString)
                    {
                        endDateTimeString(strBeg, strEnd);
                        startedDateTimeString = false;
                    }
                    else if (startedConstString)
                    {
                        endConstString(strBeg, strEnd);
                        startedConstString = false;
                    }

                    addSimpleSpecifier(*it);
                }
                else
                {
                    if (!startedDateTimeString)
                    {
                        if (startedConstString)
                        {
                            endConstString(strBeg, strEnd);
                            startedConstString = false;
                        }

                        //Set the start point
                        strBeg = it - 1;
                        startedDateTimeString = true;
                    }

                    if (*it == 'E' || *it == 'O')
                    {
                        ++it;
                    }

                    //date/time part
                    strEnd = it;
                }
            }
            else
            {
                if (!startedDateTimeString && !startedConstString)
                {
                    strBeg = it;
                    startedConstString = true;
                }
                //Move end iterator to the correct loc.
                strEnd = it;
            }
        }

        //End const strings and date/time strings, if there is one not finished
        if (startedDateTimeString)
        {
            endDateTimeString(strBeg, strEnd);
        }
        else if (startedConstString)
        {
            endConstString(strBeg, strEnd);
        }

        #ifdef DEBUG_EASYOUT
        std::cout << "[End of formatter element list]" << newline << newline;
        #endif // DEGUG_EASYOUT
    }


    void writeToOss(std::ostringstream& oss,
                    const std::string& severity,
                    const std::string& origin,
                    const std::string& message
                    ) const
    {
        //Create a format meta which will hold the constant references of the
        //important strings
        FormatMeta fmeta(message, severity, origin);

        //Set filling character
        //(We need 12:34:56.078 as time, not 12:34:56. 78, which is technically
        //different)
        oss << std::setfill('0');

        //Write to all formatter parts
        for (auto& it : fmt_list)
        {
            it->write(oss, fmeta);
        }

        //End the entry with a new line
        oss << newline;
    }

private:
    void endConstString(const std::string::iterator& first,
                        const std::string::iterator& last
                        )
    {
        #ifdef DEBUG_EASYOUT
        std::string cstr(first, last+1);
        std::cout << " [Const string specifier:]" << cstr << newline;
        #endif // DEBUG_EASYOUT

        fmt_list.push_back(std::unique_ptr<FormatInstance>
                           (new ConstStrFormatInstance
                            (std::string
                             (first, last+1
                              ))));
    }

    void endDateTimeString(const std::string::iterator& first,
                           const std::string::iterator& last
                           )
    {
        #ifdef DEBUG_EASYOUT
        std::string cstr(first, last+1);
        std::cout << " [Date/Time specifier:]" << cstr << newline;
        #endif // DEBUG_EASYOUT

        fmt_list.push_back(std::unique_ptr<FormatInstance>
                           (new DateTimeFormatInstance
                            (std::string
                             (first, last+1
                              ))));
    }

    void addSimpleSpecifier(const char specifier)
    {
        std::unique_ptr<FormatInstance> pushable;

        switch (specifier)
        {
        case 'i':
            pushable.reset(new ThreadIdFormatInstance);
            #ifdef DEBUG_EASYOUT
            std::cout << " [Thread id specifier]" << newline;
            #endif // DEBUG_EASYOUT
            break;
        case 's':
            pushable.reset(new SeverityFormatInstance);
            #ifdef DEBUG_EASYOUT
            std::cout << " [Severity specifier]" << newline;
            #endif // DEBUG_EASYOUT
            break;
        case 'o':
            pushable.reset(new OriginFormatInstance);
            #ifdef DEBUG_EASYOUT
            std::cout << " [Origin specifier]" << newline;
            #endif // DEBUG_EASYOUT
            break;
        case 'f':
            pushable.reset(new MilliFormatInstance);
            #ifdef DEBUG_EASYOUT
            std::cout << " [Milliseconds specifier]" << newline;
            #endif // DEBUG_EASYOUT
            break;
        case 'q':
            pushable.reset(new MicroFormatInstance);
            #ifdef DEBUG_EASYOUT
            std::cout << " [Microseconds specifier]" << newline;
            #endif // DEBUG_EASYOUT
            break;
        default:
            //Nothing left, just the message specifier
            #ifdef DEBUG_EASYOUT
            std::cout << " [Message specifier]" << newline;
            #endif // DEBUG_EASYOUT
            pushable.reset(new FormatInstance);
        }

        fmt_list.push_back(std::move(pushable));
    }

    std::list<std::unique_ptr<FormatInstance>> fmt_list;

};

}//namespace format

}//namespace easyout
