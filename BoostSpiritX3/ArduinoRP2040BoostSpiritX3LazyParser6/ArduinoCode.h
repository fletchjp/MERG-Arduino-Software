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

//////////////////////////////////////////////////////////

// This comes from the cdc_multi example
/// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  /// return false if we're still "delaying", true if time ms has passed.
  /// this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time) {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}

#ifdef BOOST_SPIRIT_X3_NO_EXCEPTION
namespace boost { namespace spirit { namespace x3
{
  using It = std::string::const_iterator;
  /// This has to be declared somewhere in the user code.
  std::vector<std::string> where_was_I;
  std::vector<It> what_iterator;
}}}

/// This is here to avoid a failure to define in the main program. This namespace may need changing.
  namespace parser {
    struct position_cache_tag;
  }
  
#endif
#endif
