# easyout

## Briefing

A simple (I hope) library for outputting message (i.e. logging).

I created this library when I wanted to output information to different targets (file, standard out), but I also had to share these targets (specific example: my chat client has to log the sent messages to a "transcript" target and to the logging targets).

**I'm certain, that the library has a few flaws, but I'll try to do my best to optimize and make it better. Don't use it, if you just need simple logging features - you can easily find better library on the github site.**

## Intruduction to the features

First let's see a little example using the library. Plain documention in the `DOC.md` (NOT CREATED YET) file.

### Making a logger using cout target

```c++
//For output class
#include <easyout/output.hpp>
//For the cout target
#include <easyout/cout_target.hpp>

int main()
{
    //Let's call our namespace eo for easier calling
    namespace eo = easyout;
    
    //Create an output object, which will feed all the targets with your message
    eo::Output logger;
    
    //Set up cout target normally - this will add names for severity and origin keys, and add them to the cout target
    eo::setCoutNormally();
    
    //We have to add the cout target to the logger
    logger.addTarget(eo::getCoutTarget());
    
    //Write something to the target
    logger(eo::Severity::Info, eo::Origin::IO, "Look: I can add ", 1, " with ", 2, " and I'll get ", 1 + 2, '!');
    
    return 0;
}
```
The code you just read will give the following result (the time will be different probably):
```
[0001] [18:03:31.720] [Info] [IO  ] Look: I can add 1 with 2 and I'll get 3!

[Message] Writing finished successfully on cout target!
[Statistics]
 [Severity]
           0 Trce message
           0 Dbg  message
           0 Err  message
           0 Warn message
           0 Info message
           0 Err  message
           0 Warn message
           1 Info message
           0 Crit message
           0 Msge message
 [End of severity statistics]
 [Origin of log]
           0 Core message
           1 IO   message
           0 Thrd message
           0 Netw message
           0 Wind message
           0 Grph message
           0 Aud  message
 [End of origin statistics]
[End of statistics]

```
As you can see, the output is formatted properly: you can see the thread id, the time, the severity and origin flags and the actual message. If you want, you can easily change the formatting of the output with the `setFormat()` method. (Try adding `logger.setFormat("<%t> <On %Y-%m-d, at %H:%M:%S; %s, from %o> <%v>");` before the logger's `operator()` call).

To get a full understanding about the design and the possibilities of the library, see `DOC.md` (NOT CREATED YET)

## Legal notice
The library is under MIT license.

Copyright (c) 2016 Heim László.

You can find more information in the `LICENSE-MIT` file. Otherwise see https://opensource.org/licenses/MIT
