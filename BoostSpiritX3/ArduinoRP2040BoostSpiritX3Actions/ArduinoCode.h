/// @file ArduinoCode.h
/// @brief Arduino RP2040 code for Boost Spirit X3
///
/// This file is common for the Boost Spirit X3 examples.
///
/// It provides adaption for Boost 1.66.0 code to work with the Arduino RP2040.

#ifndef ARDUINO_CODE_H
#define ARDUINO_CODE_H

/// Solution to the sync_synchronize problem
/// https://www.vexforum.com/t/i-am-learning-arm-none-eabi-compiler-recently-i-want-to-ask-some-questions/73973
extern "C" void __sync_synchronize() {}

namespace boost {
  /// Dummies to sort out compilation
  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}

/// This provides an operator for streaming the output of std::string.
inline Print &operator <<(Print &stream, const std::string &arg)
{
  stream.print(arg.c_str());
  return stream;
}

/// This provides an operator for streaming the output of char*.
inline Print &operator <<(Print &stream, const char *arg)
{
  stream.print(arg);
  return stream;
}

#endif
