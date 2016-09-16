# easyout Tutorial

[Table of contents](https://github.com/Lasoloz/easyout/tree/master/doc/tut)

## 1. How does easyout work?

To know how to use easyout, you should see the structure of the library, right?

So, let's learn about the following classes: formatter, target, output. The order is important: target uses formatter, output uses target(s). The output is the high-level part of the library: this takes the severity and the origin of the message (let's call them parameters), and the remaining arguments of the `operator()` call is the message you want to send. This output sends one `std::string` (and of course the parameters) to all owned targets, and that's all for the output. The target checks the parameters and if they're good, and the sends to it's own formatter, which makes the message look like you requested.

## 1.1 Formatter

Formatter has it's own subinstances: `FormatInstance` classes are used to represent a specific format. This formatter takes an `ostringstream` reference and puts all the required "stuff" in it. Each `FormatInstance` writes the very specific data in the formatter: like the actual message, the current time or the thread id.

## 1.2 Target

The target makes the whole thing thread-safe. It has it's very own mutex, and it also has two bitsets. This bitsets are called the "keyholes". There is a severity keyhole and an origin keyhole. Actually it just checks whether the message has the correct keys - whether it should be output. Currently there are two target types: the cout target and the file target.

## 1.3 Output

The output (it's probably not the best name) translates the argument list of its `operator()` call to an `std::string`, which is then transmitted to all of its targets.