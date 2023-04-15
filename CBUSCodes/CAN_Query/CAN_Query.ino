// CAN_Query.ino
// copied from CANWrapper


/*
  Copyright (C) 2021 Martin Da Costa
  Including copyrights from CBUS_1in1out and Arduino CBUS Libraries


  This work is licensed under the:
      Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit:
      http://creativecommons.org/licenses/by-nc-sa/4.0/
   or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

   License summary:
    You are free to:
      Share, copy and redistribute the material in any medium or format
      Adapt, remix, transform, and build upon the material

    The licensor cannot revoke these freedoms as long as you follow the license terms.

    Attribution : You must give appropriate credit, provide a link to the license,
                  and indicate if changes were made. You may do so in any reasonable manner,
                  but not in any way that suggests the licensor endorses you or your use.

    NonCommercial : You may not use the material for commercial purposes. **(see note below)

    ShareAlike : If you remix, transform, or build upon the material, you must distribute
                 your contributions under the same license as the original.

    No additional restrictions : You may not apply legal terms or technological measures that
                                 legally restrict others from doing anything the license permits.

   ** For commercial use, please contact the original copyright holder(s) to agree licensing terms

    This software is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

*/

/*
      3rd party libraries needed for compilation:

      Streaming   -- C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)
      ACAN2515    -- library to support the MCP2515/25625 CAN controller IC
      CBUSSwitch  -- library access required by CBUS and CBUS Config
      CBUSLED     -- library access required by CBUS and CBUS Config
*/
///////////////////////////////////////////////////////////////////////////////////
// Pin Use map UNO:
// Digital pin 2          Interupt CAN
// Digital pin 3 (PWM)    Not Used
// Digital pin 4          Not Used
// Digital pin 5 (PWM)    Not Used
// Digital pin 6 (PWM)    Not Used
// Digital pin 7          Not Used
// Digital pin 8          Not Used
// Digital pin 9 (PWM)    Not Used
// Digital pin 10 (SS)    CS    CAN
// Digital pin 11 (MOSI)  SI    CAN
// Digital pin 12 (MISO)  SO    CAN
// Digital pin 13 (SCK)   Sck   CAN

// Digital / Analog pin 0     Not Used
// Digital / Analog pin 1     Not Used
// Digital / Analog pin 2     Not Used
// Digital / Analog pin 3     Not Used
// Digital / Analog pin 4     Not Used
// Digital / Analog pin 5     Not Used
//////////////////////////////////////////////////////////////////////////


#define DEBUG 1       // set to 0 for no serial debug

#if DEBUG
#define DEBUG_PRINT(S) Serial << S << endl
#else
#define DEBUG_PRINT(S)
#endif

// 3rd party libraries
#include <Streaming.h>

// CBUS library header files
#include <CBUS2515.h>            // CAN controller and CBUS class
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>

////////////DEFINE MODULE/////////////////////////////////////////////////

// module name
unsigned char mname[7] = { 'C', 'A', 'N', 'q', 'e', 'r', 'y' };

// constants
const byte VER_MAJ = 0;         // code major version
const byte VER_MIN = 1;       // code minor version
const byte VER_BETA = 0;        // code beta sub-version
const byte MODULE_ID = 99;      // CBUS module type

const unsigned long CAN_OSC_FREQ = 16000000;     // Oscillator frequency on the CAN2515 board

// Forward Function declarations
void framehandler(CANFrame *msg);

// User list of opcodes.  This list is an example and can be changed as required. List is retained in program memory
const byte opcodes[] PROGMEM = {OPC_DFNON, OPC_DFNOF, OPC_DSPD, OPC_GLOC, OPC_KLOC, OPC_PLOC,
      OPC_ACOF,OPC_ACON,OPC_AREQ,OPC_ASRQ,OPC_AROF,OPC_ARON,OPC_ARSOF,OPC_ARSON};

// Calculate number of opcodes in list
const byte nopcodes = sizeof(opcodes) / sizeof(opcodes[0]);
// This macro cannot be used outside a function e.g. setup() when DEBUG is 1
//DEBUG_PRINT(F("> Number of op codes ") << nopcodes);

//Define inputs here
//Define outputs here

//Allocate input pins here
//Allocate output pins here

// Define module objects here



//////////////////////////////////////////////////////////////////////////

//CBUS pins
const byte CAN_INT_PIN = 2;  // Only pin 2 and 3 support interrupts
const byte CAN_CS_PIN = 10;
//const byte CAN_SI_PIN = 11;  Pin 50 on Mega. Cannot be changed. Here as a comment for information only.
//const byte CAN_SO_PIN = 12;  Pin 51 on Mega. Cannot be changed. Here as a comment for information only. 
//const byte CAN_SCK_PIN = 13;  Pin 52 on Mega. Cannot be changed. Here as a comment for information only.

// CBUS objects
CBUS2515 CBUS;                      // CBUS object
CBUSConfig config;                  // configuration object

//
///  setup CBUS - runs once at power on called from setup()
//
void setupCBUS()
{
  // set config layout parameters
  config.EE_NVS_START = 10;
  config.EE_NUM_NVS = 10;
  config.EE_EVENTS_START = 50;
  config.EE_MAX_EVENTS = 64;
  config.EE_NUM_EVS = 4;
  config.EE_BYTES_PER_EVENT = (config.EE_NUM_EVS + 4);

  // initialise and load configuration
  config.setEEPROMtype(EEPROM_INTERNAL);
  config.begin();

  Serial << F("> mode = ") << ((config.FLiM) ? "FLiM" : "SLiM") << F(", CANID = ") << config.CANID;
  Serial << F(", NN = ") << config.nodeNum << endl;

  // show code version and copyright notice
  printConfig();

  // set module parameters
  CBUSParams params(config);
  params.setVersion(VER_MAJ, VER_MIN, VER_BETA);
  params.setModuleId(MODULE_ID);
  params.setFlags(PF_FLiM | PF_COMBI);

  // assign to CBUS
  CBUS.setParams(params.getParams());
  CBUS.setName(mname);

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);
  // register our CBUS frame handler to receive messages in user list
  CBUS.setFrameHandler(framehandler, (byte *) opcodes, nopcodes);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(2);         // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  CBUS.setPins(CAN_CS_PIN, CAN_INT_PIN);           // select pins for CAN bus CE and interrupt connections
  CBUS.begin();
}
//
///  setup Module - runs once at power on called from setup()
//

void setupModule()
{
  //Module setup code goes here for calling by setup()
}


void setup()
{
  Serial.begin (115200);
  Serial << endl << endl << F("> ** CBUS Query ** ") << __FILE__ << endl;

  setupCBUS();
  setupModule();

  DEBUG_PRINT(F("> Number of op codes ") << nopcodes);

  // end of setup
  DEBUG_PRINT(F("> ready"));
}


void loop()
{
  // do CBUS message, switch and LED processing
  CBUS.process();

  // process console commands
  processSerialInput();

  // Put module loop() code here



}

// Send an event
bool sendEvent(byte opCode, unsigned int eventNo)
{
  CANFrame msg;
  msg.id = config.CANID;
  msg.len = 5;
  msg.data[0] = opCode;
  msg.data[1] = highByte(config.nodeNum);
  msg.data[2] = lowByte(config.nodeNum);
  msg.data[3] = highByte(eventNo);
  msg.data[4] = lowByte(eventNo);

  bool success = CBUS.sendMessage(&msg);
  if (success) {
    DEBUG_PRINT(F("> sent CBUS event with Event Number ") << eventNo);
  } else {
    DEBUG_PRINT(F("> error sending CBUS message"));
  }
  return success;
}

// Send an event with one data byte
bool sendEvent(byte opCode, unsigned int eventNo, byte data1)
{
  CANFrame msg;
  msg.id = config.CANID;
  msg.len = 6;
  msg.data[0] = opCode;
  msg.data[1] = highByte(config.nodeNum);
  msg.data[2] = lowByte(config.nodeNum);
  msg.data[3] = highByte(eventNo);
  msg.data[4] = lowByte(eventNo);
  msg.data[5] = data1;

  bool success = CBUS.sendMessage(&msg);
  if (success) {
    DEBUG_PRINT(F("> sent CBUS event with data byte ") << data1);
  } else {
    DEBUG_PRINT(F("> error sending CBUS message"));
  }
  return success;
}

// Send a frame with op code only (eg OPC_ARST)
bool sendFrame(byte opCode)
{
	CANFrame msg;
	msg.id = config.CANID;
	msg.len = 1;
	msg.data[0] = opCode;
	
	bool success = CBUS.sendMessage(&msg);
	if (success) {
    DEBUG_PRINT(F("> sent CBUS message op code only ") << _HEX(opCode));
  } else {
    DEBUG_PRINT(F("> error sending CBUS message"));
  }
  return success;
}

// Send a frame with op code and 2 data bytes (eg OPC_DFNON)
bool sendFrame(byte opCode, byte data1, byte data2)
{
	CANFrame msg;
	msg.id = config.CANID;
	msg.len = 3;
	msg.data[0] = opCode;
	msg.data[1] = data1;
	msg.data[2] = data2;
	
	bool success = CBUS.sendMessage(&msg);
	if (success) {
    DEBUG_PRINT(F("> sent CBUS message op Code ") << _HEX(opCode) << F("> data byte 1 ") << _HEX(data1) << F("> data byte 2 ") << _HEX(data2));
  } else {
    DEBUG_PRINT(F("> error sending CBUS message"));
  }
  return success;
}

//
/// called from the CBUS library when a learned event is received
//
void eventhandler(byte index, CANFrame *msg)
{

  DEBUG_PRINT(F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]));
  DEBUG_PRINT(F("> event handler: length = ") << msg->len);

  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
  DEBUG_PRINT(F("> NN = ") << node_number << F(", EN = ") << event_number);
  DEBUG_PRINT(F("> op_code = ") << msg->data[0]);

  switch (msg->data[0])
  {
// Add a case for each taught event op code used
    case OPC_ACON:
    
      // case code goes here

      break;

    case OPC_ACOF:
   
      // case code goes here

      break;
  }
}

//
/// called from the CBUS library when a message is received with an OpCode in the user list
//
void framehandler(CANFrame *msg)
{

  DEBUG_PRINT(F("> handling frame with opcode = 0x") << _HEX(msg->data[0]));

  switch (msg->data[0])
  {
    // insert a case for each opcode in the user list

    case OPC_DFNON:

      // case code goes here

      break;

    case OPC_DFNOF:

      //case code goes here
      break;

      // and so on for all user list op codes
      // Copied from CANCMDDC from 2400
      // This needs defines sorting out.
      // -------------------------------------------------------------------
    case OPC_ACOF:
    case OPC_ACON:
#if DEBUG
           Serial << F("Message handled with Opcode [ 0x") << _HEX(msg->data[0]) << F(" ]")<< endl;
           Serial << F("Test code to see if a message is getting sent") << endl;
#endif
#if ACCESSORY_REQUEST_EVENT
#if USE_SHORT_EVENTS
      {
         // Local variable definition needs to be in { } 
         unsigned int device_number = 513;
         Serial << F("Send request short event with device number ") << device_number << endl;
         sendEvent(OPC_ASRQ,device_number); // Test of short event request.
      }
#else
    //if (moduleSwitch.isPressed() ) { // Send when button is pressed.
      Serial << F("Send request long event") << endl;
      sendEvent(OPC_AREQ,requestEvent); // Test of long event request.
    //}
#endif
#endif
      break;
        // -------------------------------------------------------------------
    case OPC_AROF:
    case OPC_ARON:
    case OPC_ARSOF:
    case OPC_ARSON:
        // Process responses to AREQ and ASRQ messages.
        // I may want to set this up to handle only certain device nos.
        // It will also be possible to send data or events based on this information.
    {
        byte local_opcode = msg->data[0];
#if ACCESSORY_REQUEST_EVENT
         unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
         unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
#if USE_SHORT_EVENTS
        if (local_opcode == OPC_ARSON) {
          Serial << F(" ON message from device ") << event_number << endl;
        } else if (local_opcode == OPC_ARSOF) {
          Serial << F(" OFF message from device ") << event_number << endl;
        }
#endif
        if (local_opcode == OPC_ARON) {
          Serial << F(" ON message from event ") << node_number << F(" event ") << event_number << endl;
        } else if (local_opcode == OPC_AROF) {
          Serial << F(" OFF message from node ") << node_number << F(" event ") << event_number << endl;
        }
#endif   
        }     
        break;
        // -------------------------------------------------------------------
        default:
          // ignore any other CBus messages
#if DEBUG
           Serial << F("Message handled with Opcode [ 0x") << _HEX(msg->data[0]) << F(" ]")<< endl;
#endif
          break;
   }
}


void printConfig(void)
{
  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Martin Da Costa (MERG M6223) 2021 & 2022") << endl;
  Serial << F("> © Duncan Greenwood (MERG M5767) 2021") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2021") << endl;
  Serial << F("> © Sven Rosvall (MERG M3777) 2021") << endl;
}

//
/// command interpreter for serial console input
//

void processSerialInput(void)
{
  byte uev = 0;
  char msgstr[32];

  if (Serial.available()) {
    char c = Serial.read();

    switch (c) {

      case 'n':
        // node config
        printConfig();

        // node identity
        Serial << F("> CBUS node configuration") << endl;
        Serial << F("> mode = ") << (config.FLiM ? "FLiM" : "SLiM") << F(", CANID = ") << config.CANID << F(", node number = ") << config.nodeNum << endl;
        Serial << endl;
        break;

      case 'e':
        // EEPROM learned event data table
        Serial << F("> stored events ") << endl;
        Serial << F("  max events = ") << config.EE_MAX_EVENTS << F(" EVs per event = ") << config.EE_NUM_EVS << F(" bytes per event = ") << config.EE_BYTES_PER_EVENT << endl;

        for (byte j = 0; j < config.EE_MAX_EVENTS; j++) {
          if (config.getEvTableEntry(j) != 0) {
            ++uev;
          }
        }

        Serial << F("  stored events = ") << uev << F(", free = ") << (config.EE_MAX_EVENTS - uev) << endl;
        Serial << F("  using ") << (uev * config.EE_BYTES_PER_EVENT) << F(" of ") << (config.EE_MAX_EVENTS * config.EE_BYTES_PER_EVENT) << F(" bytes") << endl << endl;

        Serial << F("  Ev#  |  NNhi |  NNlo |  ENhi |  ENlo | ");

        for (byte j = 0; j < (config.EE_NUM_EVS); j++) {
          sprintf(msgstr, "EV%03d | ", j + 1);
          Serial << msgstr;
        }

        Serial << F("Hash |") << endl;

        Serial << F(" --------------------------------------------------------------") << endl;

        // for each event data line
        for (byte j = 0; j < config.EE_MAX_EVENTS; j++) {
          if (config.getEvTableEntry(j) != 0) {
            sprintf(msgstr, "  %03d  | ", j);
            Serial << msgstr;

            // for each data byte of this event
            for (byte e = 0; e < (config.EE_NUM_EVS + 4); e++) {
              sprintf(msgstr, " 0x%02hx | ", config.readEEPROM(config.EE_EVENTS_START + (j * config.EE_BYTES_PER_EVENT) + e));
              Serial << msgstr;
            }

            sprintf(msgstr, "%4d |", config.getEvTableEntry(j));
            Serial << msgstr << endl;
          }
        }

        Serial << endl;

        break;

      // NVs
      case 'v':
        // note NVs number from 1, not 0
        Serial << "> Node variables" << endl;
        Serial << F("   NV   Val") << endl;
        Serial << F("  --------------------") << endl;

        for (byte j = 1; j <= config.EE_NUM_NVS; j++) {
          byte v = config.readNV(j);
          sprintf(msgstr, " - %02d : %3hd | 0x%02hx", j, v, v);
          Serial << msgstr << endl;
        }

        Serial << endl << endl;

        break;

      // CAN bus status
      case 'c':
        CBUS.printStatus();
        break;

      case 'h':
        // event hash table
        config.printEvHashTable(false);
        break;

      case 'y':
        // reset CAN bus and CBUS message processing
        CBUS.reset();
        break;

      case '*':
        // reboot
        config.reboot();
        break;

      case 'm':
        // free memory
        Serial << F("> free SRAM = ") << config.freeSRAM() << F(" bytes") << endl;
        break;

      case 'r':
        // renegotiate
        CBUS.renegotiate();
        break;

      case 'z':
        // Reset module, clear EEPROM
        static bool ResetRq = false;
        static unsigned long ResWaitTime;
        if (!ResetRq) {
          // start timeout timer
          Serial << F(">Reset & EEPROM wipe requested. Press 'z' again within 2 secs to confirm") << endl;
          ResWaitTime = millis();
          ResetRq = true;
        }
        else {
          // This is a confirmed request
          // 2 sec timeout
          if (ResetRq && ((millis() - ResWaitTime) > 2000)) {
            Serial << F(">timeout expired, reset not performed") << endl;
            ResetRq = false;
          }
          else {
            //Request confirmed within timeout
            Serial << F(">RESETTING AND WIPING EEPROM") << endl;
            config.resetModule();
            ResetRq = false;
          }
        }
        break;

      default:
        // Serial << F("> unknown command ") << c << endl;
        break;
    }
  }
}
