

#ifndef _DueCan_HEADER_H
#define _DueCan_HEADER_H

//access to the standard types and constants of the Arduino language (this is automatically added to normal sketches, but not to libraries)
#include <Arduino.h>
// Includes for code in other files or third-party libraries, lowest-level first
#include "BoardDefinitions.h"

#ifdef BOARD_DUE
#include "component_can.h" // This is part of Arduino SAM core, but I have copied this file to local for visibility of definitions

//  D_DueCanDebug can be enabled to get debugging information displayed.
// #define D_DueCanDebug

#ifdef D_DueCanDebug
#define _D_DueCanDebug(x) SERIALOUT.println(x);
#define _D_DueCanDebugNoLn(x) SERIALOUT.print(x);
#else
#define _D_DueCanDebug(x)
#define _D_DueCanDebugNoLn(x)
#endif

#define D_DueCan_BPS_1000K  1000000
#define D_DueCan_BPS_800K  800000
#define D_DueCan_BPS_500K  500000
#define D_DueCan_BPS_250K  250000
#define D_DueCan_BPS_125K  125000
#define D_DueCan_BPS_50K   50000
#define D_DueCan_BPS_33333 33333
#define D_DueCan_BPS_25K   25000
#define D_DueCan_BPS_10K   10000
#define D_DueCan_BPS_5K    5000

// How do you want the enable pin output to behave?
// The 2562 transceiver chip is ENABLED when its pin is pulled LOW.
#define D_DueCan_ENABLE_OUTPUT_ENABLED LOW
#define D_DueCan_ENABLE_OUTPUT_DISABLED HIGH

// For many CANXXXX definitions, see:
// C:\Users\Andrew\AppData\Local\Arduino15\packages\arduino\hardware\sam\1.6.11\system\CMSIS\Device\ATMEL\sam3xa\include\component\component_can.h

#define D_DueCanNumberOfMailboxes CANMB_NUMBER // 8

/** Define the Mailbox mask for eight mailboxes. */
//#define GLOBAL_MAILBOX_MASK           0x000000ff

/** Disable all interrupt mask */
//#define CAN_DISABLE_ALL_INTERRUPT_MASK 0xffffffff

/** Define the mailbox mode. */
#define D_DueCan_MB_DISABLE_MODE           0
#define D_DueCan_MB_RX_MODE                1
#define D_DueCan_MB_RX_OVER_WR_MODE        2
#define D_DueCan_MB_TX_MODE                3
#define D_DueCan_MB_CONSUMER_MODE          4
#define D_DueCan_MB_PRODUCER_MODE          5

/** Define CAN mailbox transfer status code. */
#define D_DueCan_MAILBOX_TRANSFER_OK       0     //! Read from or write into mailbox successfully.
#define D_DueCan_MAILBOX_NOT_READY         0x01  //! Receiver is empty or transmitter is busy.
#define D_DueCan_MAILBOX_RX_OVER           0x02  //! Message overwriting happens or there're messages lost in different receive modes.
#define D_DueCan_MAILBOX_RX_NEED_RD_AGAIN  0x04  //! Application needs to re-read the data register in Receive with Overwrite mode.

/** Define the timemark mask. */
#define D_DueCan_TIMEMARK_MASK              0x0000ffff

/* CAN timeout for synchronization. */
#define D_DueCanSyncTimeoutTicks            100000

/** The max value for CAN baudrate prescale. */
#define D_DueCan_BAUDRATE_MAX_DIV       128

/** Define the scope for TQ. */
#define D_DueCan_MIN_TQ_NUM             8
#define D_DueCan_MAX_TQ_NUM             25

/** Define the fixed bit time value. */
#define D_DueCan_BIT_SYNC               1
#define D_DueCan_BIT_IPT                2

typedef struct {
  uint8_t uc_tq;      //! CAN_BIT_SYNC + uc_prog + uc_phase1 + uc_phase2 = uc_tq, 8 <= uc_tq <= 25.
  uint8_t uc_prog;    //! Propagation segment, (3-bits + 1), 1~8;
  uint8_t uc_phase1;  //! Phase segment 1, (3-bits + 1), 1~8;
  uint8_t uc_phase2;  //! Phase segment 2, (3-bits + 1), 1~8, CAN_BIT_IPT <= uc_phase2;
  uint8_t uc_sjw;     //! Resynchronization jump width, (2-bits + 1), min(uc_phase1, 4);
  uint8_t uc_sp;      //! Sample point value, 0~100 in percent.
} dueCan_bit_timing_t;

/** Values of bit time register for different baudrates, Sample point = ((1 + uc_prog + uc_phase1) / uc_tq) * 100%. */
const dueCan_bit_timing_t C_dueCan_bit_time[] = {
  //TQ     PROG     PH1      PH2      SJW    SAMP
  {8,   (2 + 1), (1 + 1), (1 + 1), (2 + 1), 75},
  {9,   (1 + 1), (2 + 1), (2 + 1), (1 + 1), 67},
  {10,  (2 + 1), (2 + 1), (2 + 1), (2 + 1), 70},
  {11,  (3 + 1), (2 + 1), (2 + 1), (2 + 1), 72},
  {12,  (2 + 1), (3 + 1), (3 + 1), (2 + 1), 67},
  {13,  (3 + 1), (3 + 1), (3 + 1), (2 + 1), 77},
  {14,  (3 + 1), (3 + 1), (4 + 1), (2 + 1), 64},
  {15,  (3 + 1), (4 + 1), (4 + 1), (2 + 1), 67},
  {16,  (4 + 1), (4 + 1), (4 + 1), (2 + 1), 69},
  {17,  (5 + 1), (4 + 1), (4 + 1), (2 + 1), 71},
  {18,  (4 + 1), (5 + 1), (5 + 1), (2 + 1), 67},
  {19,  (5 + 1), (5 + 1), (5 + 1), (2 + 1), 68},
  {20,  (6 + 1), (5 + 1), (5 + 1), (2 + 1), 70},
  {21,  (7 + 1), (5 + 1), (5 + 1), (2 + 1), 71},
  {22,  (6 + 1), (6 + 1), (6 + 1), (2 + 1), 68},
  {23,  (7 + 1), (7 + 1), (6 + 1), (2 + 1), 70},
  {24,  (6 + 1), (7 + 1), (7 + 1), (2 + 1), 67},
  {25,  (7 + 1), (7 + 1), (7 + 1), (2 + 1), 68}
};


//This structure presupposes little endian mode. If you use it on a big endian processor you're going to have a bad time.
typedef union {
  uint64_t value;
  struct {
    uint32_t low;
    uint32_t high;
  };
  struct {
    uint16_t s0;
    uint16_t s1;
    uint16_t s2;
    uint16_t s3;
  };
  uint8_t bytes[8];
  uint8_t byte[8]; //alternate name so you can omit the s if you feel it makes more sense
} dueCanBytesUnion_t;

typedef struct
{
  uint32_t id;    // EID if ide set, SID otherwise
  uint32_t fid;   // family ID
  uint8_t rtr;    // Remote Transmission Request
  uint8_t priority; // Priority but only important for TX frames and then only for special uses. Priority 0 is the highest priority, priority 15 is the lowest priority.
  uint8_t extended; // Extended ID flag
  uint8_t length;   // Number of data bytes // AJR 20190102 moved from after time to before time, so that 4 uint8_ts are together and time can align on the next 4-byte word without wasting memory space. Packs better!
  uint16_t time;      // CAN timer value when mailbox message was received.
  dueCanBytesUnion_t data;  // 64 bits - lots of ways to access it.
} dueCanFrame_t;

struct dueCanRingbuffer_t {
  volatile uint16_t head;
  volatile uint16_t tail;
  uint16_t size;
  volatile dueCanFrame_t *buffer;
};

// Here we tell the compiler what we want callbackFunctionPtr_t to mean
// This has to match the output and input parameters for:
// static void CanBus::staticServiceRxInterruptCallbacksFromDueCan(void *objectInstancePtr, dueCanFrame_t *incomingFramePtr);
typedef void (*dueCanCallbackFunctionPtr_t)(void*, dueCanFrame_t *incomingFramePtr);

struct dueCanCallbackPointer_t {
  dueCanCallbackFunctionPtr_t functionPointer; // Pointer to a static function inside the object where the callback function is.
  void* objectInstancePointer; // Pointer to the object (object->this) in which the callback function is.
};


class DueCan
{
  public:
    // Constructor
    DueCan(Can* pCan, uint32_t En, uint32_t numTXBoxes, uint32_t sizeTxBuffer, uint32_t sizeRxBuffer);

    // BEGIN-related activities and basic configuration
    uint32_t begin(uint32_t baudrate);

    // Optional configations
    int setNumTxBoxes(int txBoxes); // These will be the highest boxes, i.e. getFirstTxBox() to (D_DueCanNumberOfMailboxes-1)
    void connectIsrCallback(uint8_t mailbox, void* objectInstancePtr, dueCanCallbackFunctionPtr_t functionPtr);  // Send the pointer to the current object (canBus instance of CanBus), and the address of the function
    void setMailBoxTxBufferSize(uint8_t mbox, uint16_t size); // Optionally, you can also define specific individual buffers for each Tx mailbox
    // to use instead of the normal buffer sizeTxBuffer which is then shared across all (other) Tx mailboxes.
    // CARE when using this, as it contains the new() statement and mallocs memory (which is never freed)
    // So you should call this soon after instantiation, and make sure it is NEVER called again for that mailbox, or you'll end up with multiple memory malloc's
    int setFilter(uint32_t id, uint32_t mask, bool extended);
    int setFilterSpecific(uint8_t mailbox, uint32_t id, uint32_t mask, bool extended);
    inline uint8_t getFirstTxBox() { // These will be the highest boxes, i.e. getFirstTxBox() to (D_DueCanNumberOfMailboxes-1)
      return getNumMailBoxes() - _numTxBoxes;
    }
    inline uint8_t getLastTxBox() {
      return getNumMailBoxes() - 1;
    }
    inline uint8_t getNumMailBoxes() {
      return D_DueCanNumberOfMailboxes;
    }
    inline uint8_t getNumRxBoxes() { // These will be the lowest 0 to (D_DueCanNumberOfMailboxes - _numTxBoxes) mailboxes
      return getNumMailBoxes() - _numTxBoxes;
    }

    // Sending
    bool firstFreeTxMailBox(byte* mBoxFreeForTx); // returns true (and mBoxFreeForTx) if it finds a free Tx mailbox that does NOT have a mailbox-specific ringbuffer defined.
    // For sending to mailbox(es) which use the global Tx ringbuffer and have NOT had their own individual ringbuffers configured using setMailBoxTxBufferSize().
    bool sendFrame(const dueCanFrame_t *txFramePtr); // Returns true if sent or added to the global Tx queue, false if the send (or add to queue) was unsuccessful.
    // For sending to mailbox(es) which do NOT use the global Tx ringbuffer and HAVE HAD their own individual ringbuffers configured using setMailBoxTxBufferSize().
    // OR, you can call it anyway, without declaring the mailbox-specific ringbuffer for this mailbox with setMailBoxTxBufferSize(),
    // and the routine will send the message directly if the mailbox is free, but it cannot add to the mailbox-specific Tx ringbuffer if it's not been defined yet.
    bool sendFrameToSpecificMailbox(const dueCanFrame_t *txFramePtr, uint8_t mbox); // Returns true if sent or added to the mailbox-specific Tx queue, false if the send (or add to queue) was unsuccessful.
    // Receiving
    bool pullFrameFromRxRingBuffer(dueCanFrame_t *rxFramePtr); // Returns true if a message was removed, false if the ring is empty.
    uint16_t rxRingNumberOfMessages(); // Returns the number of messages in the ring buffer
    int rxRingStatus(); // Returns 0:No Rx messages in ring, 1:1 or more messages, -1:Ring is full (or size==0)
    bool rxRingHasAtLeastOneMessageInIt(); // Returns true if there are frames waiting in buffer, otherwise false. If ring.size==0, returns false.
    // ISR entry point
    void interruptHandler();

  private:
    // Local Variables and Function prototypes for the ones you DONT want the user to see

    // Private methods
    // BEGIN-related activities and basic configuration
    uint32_t set_baudrate(uint32_t ul_baudrate);
    void reset_all_mailboxes();
    void mailbox_init(uint8_t mailbox);
    void initRingBuffer(dueCanRingbuffer_t *ringPtr, volatile dueCanFrame_t *buffer, uint16_t size);
    // Optional configations
    int findFreeRXMailbox();
    // Ring buffer operations
    uint16_t ringBufferCount(dueCanRingbuffer_t *ringPtr); // Will always return 0 for ring.size==0 so long as ring.tail and ring.head are stuck at 0.
    bool addToRingBuffer(dueCanRingbuffer_t *ringPtr, const dueCanFrame_t *msgPtr);  // Returns true if added, false if the ring is full.
    bool removeFromRingBuffer(dueCanRingbuffer_t *ringPtr, dueCanFrame_t *msgPtr);   // Returns true if a message was removed, false if the ring is empty.
    inline bool isRingBufferEmpty(dueCanRingbuffer_t *ringPtr) {
      return (ringPtr->head == ringPtr->tail); // Will always return true (buffer Empty) for ring.size==0 so long as ring.tail and ring.head are stuck at 0.
    }
    bool usesGlobalTxRing(uint8_t mbox) {
      return (mbox < getNumMailBoxes() ? _txRings[mbox].size == 0 : true);
    }
    // Low-level functions
    void enable();
    void disable();
    void setModeBit(uint32_t bit);
    void unsetModeBit(uint32_t bit);
    void disable_autobaud_listen_mode();
    void enable_interrupt(uint32_t dw_mask);
    void disable_interrupt(uint32_t dw_mask);
    void global_send_transfer_cmd(uint8_t uc_mask);
    void mailbox_send_transfer_cmd(uint8_t mailbox);
    void writeTxRegisters(const dueCanFrame_t *txFramePtr, uint8_t mb);
    uint32_t mailbox_read(uint8_t mailbox, volatile dueCanFrame_t *rxFramePtr);
    void mailbox_set_mode(uint8_t mailbox, uint8_t mode);
    void mailbox_set_priority(uint8_t mailbox, uint8_t pri);
    void mailbox_set_accept_mask(uint8_t mailbox, uint32_t mask, bool ext);
    void mailbox_set_id(uint8_t mailbox, uint32_t id, bool extended);
    void mailbox_set_databyte(uint8_t mailbox, uint8_t bytepos, uint8_t val);
    void  mailbox_set_rtr (uint8_t mbox,  uint8_t rtr) ;
    void mailbox_set_datalen(uint8_t mailbox, uint8_t dlen);
    // ISRs
    uint32_t getMailboxIer(int8_t mailbox); // Returns the IER of the specified mailbox
    void mailbox_int_handler(uint8_t mb /*, uint32_t ul_status */);
    void irqLock() {
      NVIC_DisableIRQ(_nIRQ);
    }
    void irqRelease() {
      NVIC_EnableIRQ(_nIRQ);
    }
    bool isTxBox(uint8_t mbox) {
      return (mbox >= getFirstTxBox() && mbox < getNumMailBoxes() );
    }

    // Private local variables
    int _numTxBoxes; //There are 8 mailboxes, anything not TX (the upper _numTxBoxes mailboxes) will be set RX (the lower 8-_numTxBoxes mailboxes)
    uint16_t _sizeRxBuffer;
    uint16_t _sizeTxBuffer;

    Can* _m_pCan;     // CAN peripheral, set by constructor
    IRQn_Type _nIRQ;

    volatile dueCanFrame_t *_rxFrameBuff; // becomes dueCanFrame_t[_sizeTxBuffer]
    volatile dueCanFrame_t *_txFrameBuff; // becomes dueCanFrame_t[_sizeRxBuffer]
    dueCanRingbuffer_t _txRing;
    dueCanRingbuffer_t _rxRing;
    dueCanRingbuffer_t _txRings[D_DueCanNumberOfMailboxes]; // Sizes of each buffer set by setMailBoxTxBufferSize() and declared (if needed) in setMailBoxTxBufferSize
    uint8_t _filterSet[D_DueCanNumberOfMailboxes]; // Booleans just to tell you if a filter has been set or not for each mailbox

    uint32_t _numBusErrors;
    uint32_t _numRxFrames;
    int _enablePin;

    dueCanCallbackPointer_t _isrCallbackPointers[D_DueCanNumberOfMailboxes];   //  array of pointers to the callback functions for each mailbox
};

#endif // #ifdef BOARD_DUE

#endif // #ifndef _DueCan_HEADER_H
