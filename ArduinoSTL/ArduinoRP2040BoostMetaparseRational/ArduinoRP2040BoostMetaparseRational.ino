// ArduinoRP2040BoostMetaparseRational
// Arduino: 1.8.15 (Windows 7), Board: "Arduino Nano RP2040 Connect, 16MB (no FS), 125 MHz, Disabled, None, Pico SDK"

/*

E:\Program Files (x86)\Arduino\arduino-builder -dump-prefs -logger=machine -hardware E:\Program Files (x86)\Arduino\hardware -hardware C:\Users\User\AppData\Local\Arduino15\packages -tools E:\Program Files (x86)\Arduino\tools-builder -tools E:\Program Files (x86)\Arduino\hardware\tools\avr -tools C:\Users\User\AppData\Local\Arduino15\packages -built-in-libraries E:\Program Files (x86)\Arduino\libraries -libraries C:\Users\User\Documents\Arduino\libraries -fqbn=rp2040:rp2040:arduino_nano_connect:flash=16777216_0,freq=125,dbgport=Disabled,dbglvl=None,usbstack=picosdk -vid-pid=2E8A_0058 -ide-version=10815 -build-path C:\Users\User\AppData\Local\Temp\arduino_build_304068 -warnings=none -build-cache C:\Users\User\AppData\Local\Temp\arduino_cache_306616 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.pqt-pioasm.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-pioasm\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-pioasm-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-pioasm\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-python3.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed -prefs=runtime.tools.pqt-python3-1.0.1-base-3a57aed.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed -prefs=runtime.tools.pqt-elf2uf2.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-elf2uf2\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-elf2uf2-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-elf2uf2\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-openocd.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-openocd\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-openocd-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-openocd\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-gcc.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-gcc\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-gcc-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-gcc\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-mklittlefs.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-mklittlefs\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-mklittlefs-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-mklittlefs\1.3.1-a-7855b0c -verbose E:\GitHub\Arduino\MERG-Arduino-Software\ArduinoSTL\ArduinoRP2040BoostMetaparseRational\ArduinoRP2040BoostMetaparseRational.ino

E:\Program Files (x86)\Arduino\arduino-builder -compile -logger=machine -hardware E:\Program Files (x86)\Arduino\hardware -hardware C:\Users\User\AppData\Local\Arduino15\packages -tools E:\Program Files (x86)\Arduino\tools-builder -tools E:\Program Files (x86)\Arduino\hardware\tools\avr -tools C:\Users\User\AppData\Local\Arduino15\packages -built-in-libraries E:\Program Files (x86)\Arduino\libraries -libraries C:\Users\User\Documents\Arduino\libraries -fqbn=rp2040:rp2040:arduino_nano_connect:flash=16777216_0,freq=125,dbgport=Disabled,dbglvl=None,usbstack=picosdk -vid-pid=2E8A_0058 -ide-version=10815 -build-path C:\Users\User\AppData\Local\Temp\arduino_build_304068 -warnings=none -build-cache C:\Users\User\AppData\Local\Temp\arduino_cache_306616 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.pqt-pioasm.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-pioasm\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-pioasm-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-pioasm\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-python3.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed -prefs=runtime.tools.pqt-python3-1.0.1-base-3a57aed.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed -prefs=runtime.tools.pqt-elf2uf2.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-elf2uf2\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-elf2uf2-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-elf2uf2\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-openocd.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-openocd\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-openocd-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-openocd\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-gcc.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-gcc\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-gcc-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-gcc\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-mklittlefs.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-mklittlefs\1.3.1-a-7855b0c -prefs=runtime.tools.pqt-mklittlefs-1.3.1-a-7855b0c.path=C:\Users\User\AppData\Local\Arduino15\packages\rp2040\tools\pqt-mklittlefs\1.3.1-a-7855b0c -verbose E:\GitHub\Arduino\MERG-Arduino-Software\ArduinoSTL\ArduinoRP2040BoostMetaparseRational\ArduinoRP2040BoostMetaparseRational.ino

Using board 'arduino_nano_connect' from platform in folder: C:\Users\User\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\1.9.5

Using core 'rp2040' from platform in folder: C:\Users\User\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\1.9.5

Detecting libraries used...

"C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\1.3.1-a-7855b0c/bin/arm-none-eabi-g++" -c -Werror=return-type "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/tools/libpico" -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DUSB_VID=0x2341 -DUSB_PID=0x0058 "-DUSB_MANUFACTURER=\"Arduino\"" "-DUSB_PRODUCT=\"Nano RP2040 Connect\"" -Os -march=armv6-m -mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -fno-exceptions "-iprefixC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/" "@C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/lib/platform_inc.txt" -std=gnu++17 -g -w -x c++ -E -CC -DSERIALUSB_PID=0x0058 -DF_CPU=125000000L -DARDUINO=10815 -DARDUINO_ARDUINO_NANO_RP2040_CONNECT "-DBOARD_NAME=\"ARDUINO_NANO_RP2040_CONNECT\"" -DARDUINO_ARCH_RP2040 "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\cores\\rp2040" "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\variants\\arduino_nano_connect" "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\sketch\\ArduinoRP2040BoostMetaparseRational.ino.cpp" -o nul

Alternatives for boost_metaparse.hpp: [BoostForArduino@1.66.0]

ResolveLibrary(boost_metaparse.hpp)

  -> candidates: [BoostForArduino@1.66.0]

"C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\1.3.1-a-7855b0c/bin/arm-none-eabi-g++" -c -Werror=return-type "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/tools/libpico" -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DUSB_VID=0x2341 -DUSB_PID=0x0058 "-DUSB_MANUFACTURER=\"Arduino\"" "-DUSB_PRODUCT=\"Nano RP2040 Connect\"" -Os -march=armv6-m -mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -fno-exceptions "-iprefixC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/" "@C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/lib/platform_inc.txt" -std=gnu++17 -g -w -x c++ -E -CC -DSERIALUSB_PID=0x0058 -DF_CPU=125000000L -DARDUINO=10815 -DARDUINO_ARDUINO_NANO_RP2040_CONNECT "-DBOARD_NAME=\"ARDUINO_NANO_RP2040_CONNECT\"" -DARDUINO_ARCH_RP2040 "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\cores\\rp2040" "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\variants\\arduino_nano_connect" "-IC:\\Users\\User\\Documents\\Arduino\\libraries\\BoostForArduino\\src" "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\sketch\\ArduinoRP2040BoostMetaparseRational.ino.cpp" -o nul

Generating function prototypes...

"C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\1.3.1-a-7855b0c/bin/arm-none-eabi-g++" -c -Werror=return-type "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/tools/libpico" -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DUSB_VID=0x2341 -DUSB_PID=0x0058 "-DUSB_MANUFACTURER=\"Arduino\"" "-DUSB_PRODUCT=\"Nano RP2040 Connect\"" -Os -march=armv6-m -mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -fno-exceptions "-iprefixC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/" "@C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/lib/platform_inc.txt" -std=gnu++17 -g -w -x c++ -E -CC -DSERIALUSB_PID=0x0058 -DF_CPU=125000000L -DARDUINO=10815 -DARDUINO_ARDUINO_NANO_RP2040_CONNECT "-DBOARD_NAME=\"ARDUINO_NANO_RP2040_CONNECT\"" -DARDUINO_ARCH_RP2040 "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\cores\\rp2040" "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\variants\\arduino_nano_connect" "-IC:\\Users\\User\\Documents\\Arduino\\libraries\\BoostForArduino\\src" "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\sketch\\ArduinoRP2040BoostMetaparseRational.ino.cpp" -o "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\preproc\\ctags_target_for_gcc_minus_e.cpp"

"E:\\Program Files (x86)\\Arduino\\tools-builder\\ctags\\5.8-arduino11/ctags" -u --language-force=c++ -f - --c++-kinds=svpf --fields=KSTtzns --line-directives "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\preproc\\ctags_target_for_gcc_minus_e.cpp"

Compiling sketch...

"C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\1.3.1-a-7855b0c/bin/arm-none-eabi-g++" -c -Werror=return-type "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/tools/libpico" -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DUSB_VID=0x2341 -DUSB_PID=0x0058 "-DUSB_MANUFACTURER=\"Arduino\"" "-DUSB_PRODUCT=\"Nano RP2040 Connect\"" -Os -march=armv6-m -mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -fno-exceptions "-iprefixC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/" "@C:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5/lib/platform_inc.txt" -std=gnu++17 -g -DSERIALUSB_PID=0x0058 -DF_CPU=125000000L -DARDUINO=10815 -DARDUINO_ARDUINO_NANO_RP2040_CONNECT "-DBOARD_NAME=\"ARDUINO_NANO_RP2040_CONNECT\"" -DARDUINO_ARCH_RP2040 "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\cores\\rp2040" "-IC:\\Users\\User\\AppData\\Local\\Arduino15\\packages\\rp2040\\hardware\\rp2040\\1.9.5\\variants\\arduino_nano_connect" "-IC:\\Users\\User\\Documents\\Arduino\\libraries\\BoostForArduino\\src" "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\sketch\\ArduinoRP2040BoostMetaparseRational.ino.cpp" -o "C:\\Users\\User\\AppData\\Local\\Temp\\arduino_build_304068\\sketch\\ArduinoRP2040BoostMetaparseRational.ino.cpp.o"

In file included from C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse/v1/cpp11/string.hpp:209,

                 from C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse/v1/string.hpp:12,

                 from C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse/v1/grammar.hpp:27,

                 from C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse/grammar.hpp:9,

                 from C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse.hpp:60,

                 from C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost_metaparse.hpp:1,

                 from E:\GitHub\Arduino\MERG-Arduino-Software\ArduinoSTL\ArduinoRP2040BoostMetaparseRational\ArduinoRP2040BoostMetaparseRational.ino:5:

C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse/v1/cpp11/impl/remove_trailing_no_chars.hpp:26:65: error: narrowing conversion of '-1' from 'int' to 'char' [-Wnarrowing]

   26 |         struct remove_trailing_no_chars<string<BOOST_NO_CHAR, Cs...>> :

      |                                                                 ^~~

C:\Users\User\Documents\Arduino\libraries\BoostForArduino\src/boost/metaparse/v1/cpp11/impl/remove_trailing_no_chars.hpp:26:68: error: template argument 1 is invalid

   26 |         struct remove_trailing_no_chars<string<BOOST_NO_CHAR, Cs...>> :

      |                                                                    ^~

Using library BoostForArduino at version 1.66.0 in folder: C:\Users\User\Documents\Arduino\libraries\BoostForArduino 

exit status 1

Error compiling for board Arduino Nano RP2040 Connect.

*/

#undef F
#include <cstdio>
#include <boost_metaparse.hpp>

// This will need to be implemented somehow.
template <class Num, class Denom>
struct rational;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
