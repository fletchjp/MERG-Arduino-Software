// CBUSMCPCAN_example3
// Version adding External EEPROM
// Pin changes for CAN HAT
// Make long message code optional.
// Not needed as it does not have long message code yet

// Arduino core libraries
#include <Wire.h>
#include <SPI.h>

// Arduino libraries
#include <Arduino.h>
#include <Streaming.h>

/// This activates the long message code.
//#define CBUS_LONG_MESSAGE

#define USE_EXTERNAL_EEPROM

// constants

constexpr byte VER_MAJ = 1;                  // code major version
constexpr char VER_MIN = 'a';                // code minor version
constexpr byte VER_BETA = 3;                 // code beta sub-version

constexpr byte MODULE_ID = 99;

constexpr byte LED_GRN = 21;                 // CBUS green SLiM LED pin
constexpr byte LED_YLW = 20;                 // CBUS yellow FLiM LED pin
constexpr byte SW_PIN = 22;                  // CBUS push button switch pin

constexpr byte NUM_CAN_RX_BUFFS = 16;        // number of CAN receive buffers
constexpr byte NUM_CAN_TX_BUFFS = 4;         // number of CAN transmit buffers

// pins for Pico CAN shield
constexpr byte CAN_INT = 22;          // INT output of MCP2515
constexpr byte MCP2515_SCK  = 2;      // SCK input of MCP2515
constexpr byte MCP2515_MOSI = 3;      // SDI input of MCP2515
constexpr byte MCP2515_MISO = 4;      // SDO output of MCP2515
constexpr byte CAN_CS  = 5;           // CS input of MCP2515

#ifdef USE_EXTERNAL_EEPROM
#define WIRE Wire1
#ifdef ARDUINO_ARCH_RP2040
static const byte WIRE_SDA = 6;
static const byte WIRE_SCL = 7;
#endif
#endif

// CBUS libraries
#include <CBUSMCP_CAN.h>
#include <CBUSswitch.h>
#include <CBUSLED.h>
#include <CBUSParams.h>
#include <CBUSconfig.h>
#include <cbusdefs.h>

// forward function declarations
void eventhandler(byte index, CANFrame *msg);
void framehandler(CANFrame *msg);

// list of opcodes to listen for
byte frames[] = {};

// node parameters - we define 20 params (1-20) plus 0 = number of params
unsigned char params[21];
unsigned char mname[7] = { 'R', 'P', 'I', 'P', 'I', 'C', 'O' };

// CBUS objects
CBUSConfig module_config;
CBUSMCP_CAN CBUS(&module_config);
CBUSLED cbus_led_green, cbus_led_yellow;
CBUSSwitch cbus_switch;

//
/// setup - runs once at power on
//

void setup() {

  uint32_t t = millis();

  delay(5000);
  Serial.begin(115200);
  //while (!Serial && millis() - t < 5000);
  delay(5000);
  Serial << endl << endl << F("> ** CBUS test program for RPi Pico with MCP_CAN library ") << __FILE__ << endl;

  // set config layout parameters
  module_config.EE_NVS_START = 10;
  module_config.EE_NUM_NVS = 16;

  module_config.EE_EVENTS_START = 30;
  module_config.EE_MAX_EVENTS = 64;
  module_config.EE_NUM_EVS = 2;
  module_config.EE_BYTES_PER_EVENT = (module_config.EE_NUM_EVS + 4);

  // initialise and load module configuration
#ifdef USE_EXTERNAL_EEPROM
  module_config.setExtEEPROMAddress(0x50, &WIRE);
#ifdef ARDUINO_ARCH_RP2040
  module_config.setExtEEPROMPins(WIRE_SDA, WIRE_SCL);
#endif
  module_config.setEEPROMtype(EEPROM_EXTERNAL);
#else
  module_config.setEEPROMtype(EEPROM_INTERNAL);
#endif
  module_config.begin();

  // set LED and switch pins
  cbus_led_green.setPin(LED_GRN);
  cbus_led_yellow.setPin(LED_YLW);
  cbus_switch.setPin(SW_PIN, LOW);

  // module reset - if switch is depressed at startup and module is in SLiM mode
  cbus_switch.run();

  if (cbus_switch.isPressed() && !module_config.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode - resetting module") << endl;
    module_config.resetModule(cbus_led_green, cbus_led_yellow, cbus_switch);

    // module resets and restarts
    /*NOTREACHED*/
  }

  //
  /// opportunity to do one-time configuration after module reset, e.g. set default NVs
  /// reset defaults all NVs to zero
  //

  if (module_config.isResetFlagSet()) {            // flag set by module reset
    Serial << F("> module has been reset, set default config") << endl;
    module_config.clearResetFlag();               // clear the reset flag
  }

  // show code version and copyright notice
  print_config();

  // set module parameters
  CBUSParams params(module_config);
  params.setVersion(VER_MAJ, VER_MIN, VER_BETA);
  params.setModuleId(MODULE_ID);
  params.setFlags(PF_FLiM | PF_COMBI);
#ifdef USE_EXTERNAL_EEPROM
  module_config.writeBytesEEPROM(module_config.getEEPROMsize()+1,params.getParams(),params.size());
#endif

  // assign to CBUS
  CBUS.setParams(params.getParams());
  CBUS.setName(mname);

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // register our frame handler to receive non-event type CBUS messages
  CBUS.setFrameHandler(framehandler);

  // assign LEDs and switch to CBUS
  CBUS.setLEDs(cbus_led_green, cbus_led_yellow);
  CBUS.setSwitch(cbus_switch);

  // set CBUS LEDs to indicate current mode
  CBUS.indicateMode(module_config.FLiM);

  // configure and start CAN bus and CBUS message processing
  Serial << F("> initialising CBUS") << endl;
  CBUS.setNumBuffers(NUM_CAN_RX_BUFFS, NUM_CAN_TX_BUFFS);   // message receive buffers, more buffers = more memory used, fewer = less
  CBUS.setOscFreq(12000000UL);                              // crystal frequency of the CAN controller

#ifdef ARDUINO_ARCH_RP2040
  // Pico SPI peripherals have no default pins so all values must be provided
  CBUS.setPins(CAN_CS, CAN_INT, MCP2515_MOSI, MCP2515_MISO, MCP2515_SCK);
  //Serial << "> CS pin is " << CBUS.getCSpin() << endl;
  //Serial << "> INT pin is " << CBUS.getINTpin() << endl;
#else
  CBUS.setPins(CAN_CS, CAN_INT);
#endif

  if (CBUS.begin()) {
    Serial << F("> CBUS initialised ok") << endl;
  } else {
    Serial << F("> CBUS init error") << endl;
  }

  Serial << F("> setup complete") << endl << endl;
  Serial.flush();

  //
  /// end of setup
  //
}

//
/// loop - repeats forever
//

void loop() {


  //
  /// do CBUS message, switch and LED processing
  //

  CBUS.process();

  //
  /// process serial console input
  //

  get_serial_input();

  //
  /// bottom of loop()
  //

}

//
/// event handler called when a previously taught CBUS accessory event is received
//

void eventhandler(byte index, CANFrame *msg) {

  // display the formatted message
  (void)index;
  display_CAN_message(msg);

  return;
}

//
/// frame handler called when non-event messages of interest are received
//

void framehandler(CANFrame *msg) {

  // display the formatted message
  display_CAN_message(msg);

  return;
}

//
/// process console commands
//

void get_serial_input(void) {

  static char buff[32];
  static byte index = 0;

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (index == 0)  {
        Serial << endl;
      } else {
        buff[index] = 0;
        process_command(buff);
        index = 0;
        buff[index] = 0;
        continue;
      }
    }

    if (isalnum(c) || c == ' ' || c == '*') {
      buff[index] = c;
      index = (index + 1) % sizeof(buff);
    }
  }
}

void process_command(char cmd[]) {

  char tokens[6][8], *ptr;
  byte uev = 0, nvnum = 0, nvval = 0, index = 0;
  char msgstr[64], dstr[16];
  CANFrame cf;

  ptr = strtok(cmd, " ");

  while (ptr && index < 6) {
    strncpy(tokens[index++], ptr, 7);
    ptr = strtok(nullptr, " ");
  }

  if (index == 0) {
    return;
  }

  Serial << F("= ");

  for (byte i = 0; i < index && i < 6; i++) {
    Serial << tokens[i] << " ";
  }

  Serial << endl;

  switch (tolower(tokens[0][0])) {

    case 'n':
      // node identity & config
      print_config();
      break;

    // New option to see some raw EEPROM
    case 'd':
      Serial << F("> stored EEPROM ") << endl;
      for (byte j = 0; j < 20; j++)
      {
          Serial << j << " " << module_config.readEEPROM(j) << endl;
      }
      break;

    case 'e':
      // EEPROM event data table
      Serial << F("> stored events ") << endl;

      sprintf(msgstr, "  max events = %d, EVs per event = %d, bytes per event = %d", module_config.EE_MAX_EVENTS, module_config.EE_NUM_EVS, module_config.EE_BYTES_PER_EVENT);
      Serial << msgstr << endl;

      for (byte j = 0; j < module_config.EE_MAX_EVENTS; j++) {
        if (module_config.getEvTableEntry(j) != 0) {
          ++uev;
        }
      }

      Serial << F("  stored events = ") << uev << F(", free = ") << (module_config.EE_MAX_EVENTS - uev) << endl;
      Serial << F("  using ") << (uev * module_config.EE_BYTES_PER_EVENT) << F(" of ") << (module_config.EE_MAX_EVENTS * module_config.EE_BYTES_PER_EVENT) << F(" bytes") << endl << endl;

      Serial << F("  Ev#  |  NNhi |  NNlo |  ENhi |  ENlo | ");

      for (byte j = 0; j < (module_config.EE_NUM_EVS); j++) {
        sprintf(dstr, " EV%2d | ", j + 1);
        Serial << dstr;
      }

      Serial << F("Hash | ") << endl;

      Serial << F(" --------------------------------------------------------------") << endl;

      // for each event data line
      if (uev > 0) {
        for (byte j = 0; j < module_config.EE_MAX_EVENTS; j++) {

          if (module_config.getEvTableEntry(j) != 0) {
            sprintf(dstr, "  %3d  | ", j);
            Serial << dstr;

            // for each data byte of this event
            for (byte e = 0; e < (module_config.EE_NUM_EVS + 4); e++) {
              sprintf(dstr, " 0x%02hx | ", module_config.readEEPROM(module_config.EE_EVENTS_START + (j * module_config.EE_BYTES_PER_EVENT) + e));
              Serial << dstr;
            }

            sprintf(dstr, " %3d | ", module_config.getEvTableEntry(j));
            Serial << dstr << endl;
          }
        }
      } else {
        Serial << F("  (empty)") << endl;
      }

      Serial << endl;

      break;

    case 'v':
      // display NVs
      // note NVs number from 1, not 0
      Serial << "> Node variables" << endl;
      Serial << F("   NV   Val") << endl;
      Serial << F("  --------------------") << endl;

      for (byte j = 1; j <= module_config.EE_NUM_NVS; j++) {
        sprintf(msgstr, " - %02d | %3hd | 0x%02hx", j, module_config.readNV(j), module_config.readNV(j));
        Serial << msgstr << endl;
      }

      Serial << endl;
      break;

    case 'p':
      // set an NV
      nvnum = atoi(tokens[1]);
      nvval = atoi(tokens[2]);
      module_config.writeNV(nvnum, nvval);
      Serial << F("> NV ") << nvnum << F(" = ") << module_config.readNV(nvnum) << endl;
      break;

    case 'h':
      // event hash table
      module_config.printEvHashTable(false);
      break;

    case 'c':
      // CAN bus status
      CBUS.printStatus();
      // Serial << F("> hwm = ") << CBUS.canp->receiveBufferPeakCount() << endl;
      Serial << F("> rx buffer puts = ") << CBUS.rx_buffer->puts() << F(", gets = ") << CBUS.rx_buffer->gets() << F(", hwm = ") << CBUS.rx_buffer->hwm() << F(", overflows = ") << CBUS.rx_buffer->overflows() << endl;
      break;

    case 'r':
      // reset CAN bus and CBUS message processing
      CBUS.reset();
      break;

    case '*':
      // reboot
      module_config.reboot();
      // does not return
      break;

    case 'm':
      // free memory
      Serial << F("> free SRAM = ") << module_config.freeSRAM() << F(" bytes") << endl;
      break;

    case 'x':
      // send a CBUS message
      cf.id = 0;
      cf.len = 8;
      if (CBUS.sendMessage(&cf) != CAN_OK) {
        Serial << F("> error sending message") << endl;
      }
      break;

    default:
      Serial << F("> unknown command ") << tokens[0][0] << endl;
      break;
  }
}

//
/// format and display a CBUS message
//

void display_CAN_message(CANFrame *cf) {

  Serial.printf("> [%03lu] [%01d] [ ", (cf->id & 0x7f), cf->len);

  for (byte i = 0; i < cf->len; i++) {
    Serial.printf("0x%02x ", cf->data[i]);
  }

  Serial << "]";

  if (cf->rtr) {
    Serial << " R";
  }

  if (cf->ext) {
    Serial << " X";
  }

  Serial << endl;

  return;
}

//
/// print code version details and copyright notice
//

void print_config(void) {

  // node config
  Serial << F("> mode = ") << ((module_config.FLiM) ? "FLiM" : "SLiM") << F(", CANID = ") << module_config.CANID;
  Serial << F(", NN = ") << module_config.nodeNum << endl;

  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Duncan Greenwood (MERG M5767) 2020, 2022") << endl;
#ifdef ARDUINO_ARCH_RP2040
  Serial << "> running on ARDUINO_ARCH_RP2040" << endl;
#endif
#ifdef USE_EXTERNAL_EEPROM
  Serial << "> using EXTERNAL EEPROM size " << module_config.getEEPROMsize() << endl;
#endif

  return;
}
