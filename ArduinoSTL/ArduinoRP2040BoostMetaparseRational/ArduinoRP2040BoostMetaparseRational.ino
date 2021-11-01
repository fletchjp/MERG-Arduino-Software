// ArduinoRP2040BoostMetaparseRational
// I have put -Wno_narrowing into platform.txt

#include <Streaming.h>

#undef F
#undef min // This is also needed.
#include <cstdio>
#include <string>
#include <boost_metaparse.hpp>
#include <boost_type_index.hpp>
#include <boost_rational.hpp>
#include <boost_config.hpp>

// This will need to be implemented somehow.
template <class Num, class Denom>
struct rational;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
