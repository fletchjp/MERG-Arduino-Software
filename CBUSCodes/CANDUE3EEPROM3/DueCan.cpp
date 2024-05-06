
#include "DueCan.h"

// This library comes from https://github.com/collin80/due_can.  author=Collin Kidder <kidderc@gmail.com>, Thibaut Viard, Wilfredo Molina
// It has been extensively pruned and streamlined into a single class.
// ISR callbacks for Rx messages are defined using static function pointers to static functions in the 'callee' class.
// Andrew J Roscoe 20190104

//

/* Example of istantiation in the main application (global context)
    DueCan dueCan0(CAN0, D_CAN0_EN, 1, D_CAN0_SIZE_TX_BUFFER, D_CAN0_SIZE_RX_BUFFER); // DueCan(Can* pCan, uint32_t En, uint32_t numTXBoxes, uint32_t sizeTxBuffer, uint32_t sizeRxBuffer);
*/

/* Example of ISR in the main application (global context)
    void CAN0_Handler(void)
    {
    dueCan0.interruptHandler();
    }

    CAN0 and CAN1 seem to be defined for the DUE by sam3x8e.h, which Arduino includes for you when you pick the Due board.
    The other parameters you can choose pins and mailbox sizes as you prefer for your application.
*/

/* Example of how to configure the ISR callback for Rx messages

   void CanBus::staticServiceRxInterruptCallbacksFromDueCan(void *objectInstancePtr, dueCanFrame_t *incomingFramePtr) // declared static ONLY in the .h file otherwise compiler tries to make 2 distinct static copies!

   {

   // Get back into the class by treating objectInstance as the "this" pointer.
   ((CanBus *)objectInstancePtr) -> serviceRxInterruptCallbacksFromDueCan(incomingFramePtr);
   }

   // Now here's the real code
   void CanBus::serviceRxInterruptCallbacksFromDueCan(dueCanFrame_t *incomingFramePtr) {
   ... your code
   }
*/

/* To enable the callback functionality of CanBus::serviceInterruptCallbackFromDueCan() above,
    you must first tell due_can about the callback. For example:...

    void CanBus::configureRxInterruptCallbacksFromDueCan(void) {
    for (uint8_t mailbox = 0; mailbox < _dueCan0.getNumRxBoxes(); mailbox++) // dueCan Rx mailboxes are the lowest ones, from 0 to _dueCan0.getNumRxBoxes()
    {
      _dueCan0.connectIsrCallback(mailbox, this, this->staticServiceRxInterruptCallbacksFromDueCan); // Send the pointer to the current object (canBus instance of CanBus), and the address of the function
    }
    }
*/







#ifdef BOARD_DUE

// Constructor for the class
// pCan = Which canbus hardware to use (CAN0 or CAN1)
// En = pin to use for transceiver enable. Polarity determined by D_DueCan_ENABLE_OUTPUT_ENABLED and D_DueCan_ENABLE_OUTPUT_DISABLED
// numTXBoxes = Number of Tx mailboxes (out of the 8). The Tx boxes will be the
// sizeTxBuffer = size of the Tx ring buffer. Can be as small as 0, if D_canBusUseRxCallBacksToBypassDueCanRingBuffer is defined and Rx interrupt callbacks are defined for all Rx mailboxes.
// sizeRxBuffer = size of the Rx ring buffer. Minimum 0.
DueCan::DueCan(Can* pCan, uint32_t En, uint32_t numTXBoxes, uint32_t sizeTxBuffer, uint32_t sizeRxBuffer)
{
  _m_pCan = pCan;
  _nIRQ = (_m_pCan == CAN0 ? CAN0_IRQn : CAN1_IRQn);
  _enablePin = En;

  _rxFrameBuff = 0;
  _txFrameBuff = 0;
  _numTxBoxes = numTXBoxes;


  disable_autobaud_listen_mode();
  disable();

  for (uint8_t i = 0; i < getNumMailBoxes(); i++) {
    _txRings[i].size = 0;   // Initialize all Tx message-box specific ring buffer sizes to 0 (not configured)
    _filterSet[i] = 0; // Mark all flters as not set
  }

  // Trap for zeros in ringbuffer sizes.

  // User might want a zero-sized Tx buffer, so that they get close visibility of when a (or the) Tx mailbox is actually free,
  // in case messages contain timestamped data which is updated at the very last minute. (RoscoeBus CanBus does this).
  // The user could ask for sizeTxBuffer = 0 and we need to make sure that we never try to load/use the sharec Tx ringbuffer.
  // (Individual ringbuffers for each Tx mailbox could still be defined using setMailBoxTxBufferSize().
  // There's no risk of segmentation fault by setting sizeTxBuffer=0,
  // it just means that the user may often obtain a false return from sendFrame() if they send many Tx messages in quick succession
  // and the Tx mailbox(es) cannot keep up.
  if (sizeTxBuffer > 0)
    _txFrameBuff = new dueCanFrame_t[sizeTxBuffer];
  // Still initialise the ringbuffer if sizeTxBuffer=0, but note that it's pointer to buffer is meaningless. Importantly, it will record that size is 0.
  initRingBuffer (&_txRing, _txFrameBuff, sizeTxBuffer);

  // User might want a zero-sized Rx buffer, if they are defining callback functions for every Rx mailbox from the ISR via connectIsrCallback().
  // If a callback function is defined for every Rx mailbox then the Rx ringbuffer is not required and is never loaded with anything.
  // In this case the user could ask for sizeRxBuffer=0 and save RAM.
  // This is at the user's risk. If they set sizeRxBuffer=0 but do NOT define callback functions for every Rx mailbox, there will be a segmentation fault.
  if (sizeRxBuffer > 0)
    _rxFrameBuff = new dueCanFrame_t[sizeRxBuffer];
  // Still initialise the ringbuffer if sizeRxBuffer=0, but note that it's pointer to buffer is meaningless. Importantly, it will record that size is 0.
  initRingBuffer (&_rxRing, _rxFrameBuff, sizeRxBuffer);
}

// =======================================================================
// =                                                                     =
// =                        BEGIN-related activities                     =
// =                                                                     =
// =======================================================================


/*
   Initialize CAN controller.
   ul_baudrate CAN communication baudrate in bps.
   Returns 0 If failed to initialize the CAN module; otherwise successful
*/
uint32_t DueCan::begin(uint32_t baudrate)
{
  uint32_t ul_flag;
  uint32_t ul_tick;

#ifdef D_DueCanDebug
  SERIALOUT.print(F("DueCan::begin. sizeof(dueCanBytesUnion_t) :"));
  SERIALOUT.println(sizeof(dueCanBytesUnion_t));
  SERIALOUT.print(F("offsetof(dueCanBytesUnion_t, low) :"));
  SERIALOUT.println(offsetof(dueCanBytesUnion_t, low));
  SERIALOUT.print(F("offsetof(dueCanBytesUnion_t, high) :"));
  SERIALOUT.println(offsetof(dueCanBytesUnion_t, high));
  SERIALOUT.print(F("offsetof(dueCanBytesUnion_t, s0) :"));
  SERIALOUT.println(offsetof(dueCanBytesUnion_t, s0));
  SERIALOUT.print(F("offsetof(dueCanBytesUnion_t, s1) :"));
  SERIALOUT.println(offsetof(dueCanBytesUnion_t, s1));
  SERIALOUT.print(F("offsetof(dueCanBytesUnion_t, s2) :"));
  SERIALOUT.println(offsetof(dueCanBytesUnion_t, s2));
  SERIALOUT.print(F("offsetof(dueCanBytesUnion_t, s3) :"));
  SERIALOUT.println(offsetof(dueCanBytesUnion_t, s3));
  SERIALOUT.print(F("DueCan::begin. sizeof(dueCanFrame_t) :"));
  SERIALOUT.println(sizeof(dueCanFrame_t));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, id) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, id));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, fid) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, fid));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, rtr) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, rtr));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, priority) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, priority));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, extended) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, extended));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, length) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, length));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, time) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, time));
  SERIALOUT.print(F("offsetof(dueCanFrame_t, data) :"));
  SERIALOUT.println(offsetof(dueCanFrame_t, data));
#endif

  if (_enablePin != 255) {
    pinMode(_enablePin, OUTPUT);
  }

  can_disable(_m_pCan); //immediately disable the CAN hardware if it had previously been enabled

  uint32_t ul_status = _m_pCan->CAN_SR; //read the status register just to be sure it gets cleared out
  (void)ul_status;  // avoid compiler warnings

  _numBusErrors = 0;
  _numRxFrames = 0;

  // clear all ISR callbacks
  for (int mailbox = 0; mailbox < getNumMailBoxes(); mailbox++)
    connectIsrCallback(mailbox, (void*)0, (dueCanCallbackFunctionPtr_t)0);  // Send the pointer to the current object (canBus instance of CanBus), and the address of the function

  //arduino 1.5.2 doesn't init canbus so make sure to do it here.
#ifdef ARDUINO152
  PIO_Configure(PIOA, PIO_PERIPH_A, PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0, PIO_DEFAULT);
  PIO_Configure(PIOB, PIO_PERIPH_A, PIO_PB15A_CANRX1 | PIO_PB14A_CANTX1, PIO_DEFAULT);
#endif

  if (_m_pCan == CAN0) pmc_enable_periph_clk(ID_CAN0);
  if (_m_pCan == CAN1) pmc_enable_periph_clk(ID_CAN1);

  /* Initialize the baudrate for CAN module. */
  ul_flag = set_baudrate(baudrate);
  if (ul_flag == 0) {
    return 0;
  }


  /* Reset the eight message mailboxes. */
  reset_all_mailboxes();

  //Also disable all interrupts by default
  disable_interrupt(CAN_DISABLE_ALL_INTERRUPT_MASK);

  //By default use one mailbox for TX
  setNumTxBoxes(_numTxBoxes);

  /* Enable the CAN controller. */
  enable();

  /* Wait until the CAN is synchronized with the bus activity. */
  ul_flag = 0;
  ul_tick = 0;
  while (!(ul_flag & CAN_SR_WAKEUP) && (ul_tick < D_DueCanSyncTimeoutTicks)) {
    ul_flag = _m_pCan->CAN_SR;
    ul_tick++;
  }

  //set a fairly low priority so almost anything can preempt.
  //this has the effect that most anything can interrupt our interrupt handler
  //that's a good thing because the interrupt handler is long and complicated
  //and can send callbacks into user code which could also be long and complicated.
  //But, keep in mind that user code in callbacks runs in interrupt context
  //but can still be preempted at any time.
  NVIC_SetPriority(_nIRQ, 12);

  NVIC_EnableIRQ(_nIRQ); //tell the nested interrupt controller to turn on our interrupt

  /* Timeout or the CAN module has been synchronized with the bus. */
  if (ul_tick == D_DueCanSyncTimeoutTicks)
    return 0;
  else
    return baudrate;
}


/**
   \brief Configure CAN baudrate.

   \param ul_baudrate Baudrate value in bits per second

   \retval Set the baudrate successfully or not.
*/
uint32_t DueCan::set_baudrate(uint32_t ul_baudrate)
{
  uint8_t uc_tq;
  uint8_t uc_prescale;
  uint32_t ul_mod;
  uint32_t ul_cur_mod;
  dueCan_bit_timing_t *p_bit_time;

  static uint32_t ul_mck = SystemCoreClock;

  /* Check whether the baudrate prescale will be greater than the max divide value. */
  if (((ul_mck + (ul_baudrate * D_DueCan_MAX_TQ_NUM - 1)) /
       (ul_baudrate * D_DueCan_MAX_TQ_NUM)) > D_DueCan_BAUDRATE_MAX_DIV) {
    return 0;
  }

  /* Check whether the input MCK is too small. */
  if (ul_mck  < ul_baudrate * D_DueCan_MIN_TQ_NUM) {
    return 0;
  }

  /* Initialize it as the minimum Time Quantum. */
  uc_tq = D_DueCan_MIN_TQ_NUM;

  /* Initialize the remainder as the max value. When the remainder is 0, get the right TQ number. */
  ul_mod = 0xffffffff;
  /* Find out the approximate Time Quantum according to the baudrate. */
  for (uint8_t i = D_DueCan_MIN_TQ_NUM; i <= D_DueCan_MAX_TQ_NUM; i++) {
    if ((ul_mck / (ul_baudrate * i)) <= D_DueCan_BAUDRATE_MAX_DIV) {
      ul_cur_mod = ul_mck % (ul_baudrate * i);
      if (ul_cur_mod < ul_mod) {
        ul_mod = ul_cur_mod;
        uc_tq = i;
        if (!ul_mod) {
          break;
        }
      }
    }
  }

  /* Calculate the baudrate prescale value. */
  uc_prescale = ul_mck / (ul_baudrate * uc_tq);

  /* Get the right CAN BIT Timing group. */
  p_bit_time = (dueCan_bit_timing_t *)&C_dueCan_bit_time[uc_tq - D_DueCan_MIN_TQ_NUM];

  /* Before modifying the CANBR register, disable the CAN controller. */
  can_disable(_m_pCan);

  /* Write into the CAN baudrate register. */
  _m_pCan->CAN_BR = CAN_BR_PHASE2(p_bit_time->uc_phase2 - 1) |
                    CAN_BR_PHASE1(p_bit_time->uc_phase1 - 1) |
                    CAN_BR_PROPAG(p_bit_time->uc_prog - 1) |
                    CAN_BR_SJW(p_bit_time->uc_sjw - 1) |
                    CAN_BR_BRP(uc_prescale - 1);
  return 1;
}


// Reset the eight mailboxes.
void DueCan::reset_all_mailboxes()
{
  for (uint8_t  mailbox = 0;  mailbox < D_DueCanNumberOfMailboxes;  mailbox++)
  {
    mailbox_init(mailbox);
    _filterSet[mailbox] = 0;
  }
}


// Initialize the mailbox to a default, known state.
void DueCan::mailbox_init(uint8_t mailbox)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;

  _m_pCan->CAN_MB[mailbox].CAN_MMR = 0;
  _m_pCan->CAN_MB[mailbox].CAN_MAM = 0;
  _m_pCan->CAN_MB[mailbox].CAN_MID = 0;
  _m_pCan->CAN_MB[mailbox].CAN_MDL = 0;
  _m_pCan->CAN_MB[mailbox].CAN_MDH = 0;
  _m_pCan->CAN_MB[mailbox].CAN_MCR = 0;
}


// Initializes mailboxes to the requested mix of RX and TX boxes
// txBoxes = How many of the 8 boxes should be used for TX
// Returns the number of tx boxes set.
int DueCan::setNumTxBoxes(int txBoxes)
{
  int mailbox;

  if (txBoxes > getNumMailBoxes())  txBoxes = getNumMailBoxes() ;
  if (txBoxes < 0)                  txBoxes = 0 ;
  _numTxBoxes = txBoxes;

  //Inialize RX boxes
  for (mailbox = 0; mailbox < getNumRxBoxes(); mailbox++) {
    mailbox_set_mode(mailbox, CAN_MB_RX_MODE);
    mailbox_set_id(mailbox, 0x0, false);
    mailbox_set_accept_mask(mailbox, 0x7FF, false);
  }

  //Initialize TX boxes
  for (mailbox = getFirstTxBox(); mailbox < getNumMailBoxes(); mailbox++) {
    mailbox_set_mode(mailbox, CAN_MB_TX_MODE);
    mailbox_set_priority(mailbox, 10);
    mailbox_set_accept_mask(mailbox, 0x7FF, false);
  }

  return (_numTxBoxes);
}


// Initialize the specified ring buffer.
// ring - ring buffer to initialize.
// buffer - buffer to use for storage.
// size - size of the buffer in bytes.
void DueCan::initRingBuffer(dueCanRingbuffer_t *ringPtr, volatile dueCanFrame_t *buffer, uint16_t size)
{
  ringPtr->buffer = buffer;
  ringPtr->size   = size;
  ringPtr->head   = 0;
  ringPtr->tail   = 0;
}


// =======================================================================
// =                                                                     =
// =                      OPTIONAL CONFIGS                               =
// =                                                                     =
// =======================================================================


void DueCan::connectIsrCallback(uint8_t mailbox, void* objectInstancePtr, dueCanCallbackFunctionPtr_t functionPtr)  // Send the pointer to the current object (canBus instance of CanBus), and the address of the function
{
  _isrCallbackPointers[mailbox].functionPointer = functionPtr; // Pointer to a static function inside the object where the callback function is.
  _isrCallbackPointers[mailbox].objectInstancePointer = objectInstancePtr; // Pointer to the object (object->this) in which the callback function is.
}


// Optionally, you can also define specific individual buffers for each Tx mailbox,
// to use instead of the normal buffer sizeTxBuffer which is then shared across all (other) Tx mailboxes.
// This only makes any sense when you have more than one Tx mailbox set up.
void DueCan::setMailBoxTxBufferSize(uint8_t mailbox, uint16_t size)
{
  if ( mailbox >= getNumMailBoxes() || mailbox < getFirstTxBox() || _txRings[mailbox].size != 0 ) return;
  volatile dueCanFrame_t *bufPtr = new dueCanFrame_t[size];
  initRingBuffer(&_txRings[mailbox], bufPtr, size);
}


// Find unused RX mailbox and return its number
int  DueCan::findFreeRXMailbox ()
{
  for (int  c = 0;  c < getNumRxBoxes();  c++)
    if (_filterSet[c] == 0) return c ;
  return -1;
}


// Set up an RX mailbox (first free) for the given parameters.
// id = the post mask ID to match against
// mask = the mask to use for this filter
// extended = Whether this should be extended mask or not
// Returns the number of mailbox we just used (or -1 if there are no free boxes to use)
int DueCan::setFilter(uint32_t id, uint32_t mask, bool extended)
{
  int  mailbox = findFreeRXMailbox();
  if (mailbox < 0)  return -1 ;
  mailbox_set_accept_mask(mailbox, mask, extended);
  mailbox_set_id(mailbox, id, extended);
  enable_interrupt(getMailboxIer(mailbox));
  _filterSet[mailbox] = 1;
  return mailbox;
}


// Set up an RX mailbox (given MB number) filter
// mailbox  = Which mailbox to use (0-7)
// id = The ID to match against
// mask = the post mask ID to match against
// extended = Whether this should be extended mask or not
// Returns the mailbox number if successful or -1 on failure
int DueCan::setFilterSpecific(uint8_t mailbox, uint32_t id, uint32_t mask, bool extended)
{
  if (mailbox >= getNumRxBoxes())  return -1 ;
  mailbox_set_accept_mask(mailbox, mask, extended);
  mailbox_set_id(mailbox, id, extended);
  enable_interrupt(getMailboxIer(mailbox));
  _filterSet[mailbox] = 1;
  return mailbox;
}


// =======================================================================
// =                                                                     =
// =                      SENDING                                        =
// =                                                                     =
// =======================================================================


// You can call this before a call to sendFrame() or (probably more effectively) sendFrameToSpecificMailbox()
// so you know, in advance, whether there is a free Tx mailbox, and what its number is.
// Returns 1 if there is a free box, 0 if not. If 1, mBoxFreeForTx will contain the mailbox number that's free.
// The Tx mailboxes are the TOP ones in the stack (see definition of getFirstTxBox(),
// So. for example, on the Due there are 8 boxes available and if only 1 is set up as a Tx mailbox then
// the loop below will start at mbox 7 and only be able to search that one iteration with mbox=7
bool DueCan::firstFreeTxMailBox(byte* mBoxFreeForTx)
{
  bool result = false; // No mailbox found yet;

  *mBoxFreeForTx = 0xFF;
  // Find a free mailbox
  for (uint8_t mbox = getFirstTxBox(); mbox < D_DueCanNumberOfMailboxes; mbox++) { // Only check the Tx mailboxes!
    if (((_m_pCan->CAN_MB[mbox].CAN_MMR >> 24) & 7) == CAN_MB_TX_MODE) // is this mailbox set up as a TX box? (it should be!)
    {
      if (_m_pCan->CAN_MB[mbox].CAN_MSR & CAN_MSR_MRDY) // Is it available for use (not sending anything?)
      {
        result = true;
        *mBoxFreeForTx = mbox;
        _D_DueCanDebugNoLn(F("DueCan::Found a free Tx mailbox, number "));
        _D_DueCanDebug(mbox);
      }
    }
  }
  return (result);
}


// Send a frame out of this canbus port
// Will do one of two things
// - 1. Send the given frame out of the first available mailbox that is using the global Tx ringbuffer.
// - or 2. queue the frame for sending later via interrupt, in the global Tx ringbuffer. Automatically turns on TX interrupt if necessary.
// This routine will only search or and use Tx mailbox(es) which are using the global Tx ringbuffer,
// i.e. those which have NOT had their own individual ringbuffers configured using setMailBoxTxBufferSize().
// The Tx mailboxes are the TOP ones in the stack (see definition of getFirstTxBox(),
// So. for example, on the Due there are 8 boxes available and if only 1 is set up as a Tx mailbox then
// the loop below will start at mbox 7 and only be able to search that one iteration with mbox=7
// txFrame = The filled out frame structure to use for sending
// Returns true if sent or added to the global Tx queue, false if the send (or queue) was unsuccessful.
bool DueCan::sendFrame(const dueCanFrame_t *txFramePtr)
{
  bool result = false; // Nothing has been sent;

  irqLock();
  if (isRingBufferEmpty(&_txRing)) // Global ring buffer check. If there is nothing buffered (or ring buffer size = 0, which always appears empty AND full), then we try to actually send this message.
  { // There is a global ringbuffer available, with a size>0, AND, it's empty.  OR, there is NO global Tx ringbuffer defined.
    // So first, try and actually send the message
    for (uint8_t mbox = getFirstTxBox(); mbox < D_DueCanNumberOfMailboxes; mbox++) // Only check the Tx mailboxes!
    { // mbox loop
      if (((_m_pCan->CAN_MB[mbox].CAN_MMR >> 24) & 7) == CAN_MB_TX_MODE) // is this mailbox set up as a TX box? (it should be!)
      { // This is a Tx mailbox
        if (usesGlobalTxRing(mbox) && (_m_pCan->CAN_MB[mbox].CAN_MSR & CAN_MSR_MRDY)) // is it using the global Tx ringbuffer (i.e. a mailbox-specific ringbuffer is not defined), and is the mailbox available (free) to take a message?
        {
          _D_DueCanDebugNoLn(F("DueCan::sendFrame. Sending a Tx message to mailbox "));
          _D_DueCanDebugNoLn(mbox);
          _D_DueCanDebug(F(" which was free, with no mailbox-specific ringbuffer defined and nothing in the global Tx ringbuffer ..."));
          writeTxRegisters(txFramePtr, mbox);
          enable_interrupt(0x01u << mbox); //enable the TX interrupt for this box
          result = true; //we've sent it. mission accomplished.
          break; //no need to keep going. We sent our message
        }
        else
        {
          _D_DueCanDebugNoLn(F("DueCan::sendFrame. Mailbox "));
          _D_DueCanDebugNoLn(mbox);
          _D_DueCanDebug(F(" was busy. Trying next box or if all busy will attempt to buffer in global Tx ringbuffer ..."));
          result = false;
        }
      } // End: This is a Tx mailbox
    } // End: mbox loop
  } // End: There is a global ringbuffer available, with a size>0, AND, it's empty.  OR, there is NO global Tx ringbuffer defined.

  if (!result)
  { // Not sent to mailbox
    if (_txRing.size > 0)
    { // Attempt to add frame to global Tx ringbuffer
      result = addToRingBuffer(&_txRing, txFramePtr); // 1 if it succeeded, 0 if not
#ifdef D_DueCanDebug
      if (result)
      {
        _D_DueCanDebug(F("DueCan::sendFrame. Added the Tx message to the global Tx ringbuffer."));
      }
      else
      {
        _D_DueCanDebug(F("DueCan::sendFrame. FAILED to add the Tx message to the global Tx ringbuffer (which is FULL)."));
      }
#endif
    } // End: Attempt to add frame to global Tx ringbuffer
#ifdef D_DueCanDebug
    else
    {
      _D_DueCanDebug(F("DueCan::sendFrame. Could not add the Tx message to the NOT-DEFINED global Tx ringbuffer."));
    }
#endif
  } // End: Not sent to mailbox
  irqRelease();
  return result; // Returns true if sent or added to the Tx queue, false if the send (or queue) was unsuccessful.
}


// Send a frame out of this canbus port, to a specific (Tx) mailbox.
// Will do one of two things
// - 1. Send the given frame out of the first available mailbox
// - or 2. queue the frame for sending later via interrupt in the mailbox-specific ringbuffer. Automatically turns on TX interrupt if necessary.
// You should have declared a Tx mailbox-specific ringbuffer using setMailBoxTxBufferSize() before using this routine.
// OR, you can call it anyway, without declaring the mailbox-specific ringbuffer for this mailbox,
// and the routine will send the message directly if the mailbox is free,
// but it cannot add to the mailbox-specific Tx ringbuffer if it's not been defined yet.
// This routine will not attempt to use the global Tx ringbuffer.
// txFrame = The filled out frame structure to use for sending
// Returns true if sent or added to the mailbox-specific Tx queue, false if the send (or queue) was unsuccessful.
bool DueCan::sendFrameToSpecificMailbox(const dueCanFrame_t *txFramePtr, uint8_t mbox)
{
  bool  result = false;

  if (!isTxBox(mbox))
    return result ;
  else
  { // isTxBox(mbox)
    irqLock();
    if (isRingBufferEmpty(&_txRings[mbox])) // Mailbox-specific ringbuffer check. If there is nothing buffered (or ring buffer size = 0, which always appears empty AND full), then we try to actually send this message.
    { // There is a specific ring buffer configured for this mailbox, with a size>0, AND, it's empty. OR, there is NO mailbox-specific ringbuffer defined.
      // So first, try and actually send the message
      if (_m_pCan->CAN_MB[mbox].CAN_MSR & CAN_MSR_MRDY) // Is the mailbox available (free) to take a message?
      {
        writeTxRegisters(txFramePtr, mbox);
        enable_interrupt(0x01u << mbox); //enable the TX interrupt for this box
        result = true; //we've sent it. mission accomplished.
        _D_DueCanDebugNoLn(F("DueCan::sendFrameToSpecificMailbox. Sent a Tx message direct to mailbox "));
        _D_DueCanDebugNoLn(mbox);
        _D_DueCanDebug(F(" which was free, with nothing in its mailbox-specific ringbuffer (or a mailbox-specific ringbuffer was not defined for this mailbox) ..."));
      }
      else
      {
        _D_DueCanDebugNoLn(F("DueCan::sendFrameToSpecificMailbox. Mailbox-specific ringbuffer was empty or not defined, but mailbox "));
        _D_DueCanDebugNoLn(mbox);
        _D_DueCanDebug(F(" was busy, so attempting instead to buffer the Tx message in the mailbox-specific ringbuffer ..."));
        result = false;
      }
    } // End: There is a specific ring buffer configured for this mailbox, with a size>0, AND, it's empty
    if (!result)
    { // Not sent to mailbox
      if (_txRings[mbox].size > 0)
      { // Attempt to add frame to mailbox-specific ringbuffer
        result = addToRingBuffer(&_txRings[mbox], txFramePtr); // 1 if it succeeded, 0 if not
#ifdef D_DueCanDebug
        if (result)
        {
          _D_DueCanDebugNoLn(F("DueCan::sendFrameToSpecificMailbox. Added the Tx message to the mailbox-specific ringbuffer for mailbox "));
          _D_DueCanDebug(mbox);
        }
        else
        {
          _D_DueCanDebugNoLn(F("DueCan::sendFrameToSpecificMailbox. FAILED to add the Tx message to the mailbox-specific ringbuffer (which is FULL) for mailbox "));
          _D_DueCanDebug(mbox);
        }
#endif
      } // End: Attempt to add frame to mailbox-specific ringbuffer
#ifdef D_DueCanDebug
      else
      {
        _D_DueCanDebugNoLn(F("DueCan::sendFrameToSpecificMailbox. Could not add the Tx message to the NOT-DEFINED mailbox-specific ringbuffer for mailbox "));
        _D_DueCanDebug(mbox);
      }
#endif
    } // End: Not sent to mailbox
    irqRelease();
    return result; // Returns true if sent or added to the Tx queue, false if the send (or queue) was unsuccessful.
  } // End: isTxBox(mbox)
}


// =======================================================================
// =                                                                     =
// =                      RING BUFFER OPERATION                          =
// =                                                                     =
// =======================================================================


// Count the number of entries in the specified ring buffer.
// ring - ring buffer to use.
// Returns a count of the number of elements in the ring buffer.
uint16_t DueCan::ringBufferCount(dueCanRingbuffer_t *ringPtr)
{
  if (ringPtr->tail == ringPtr->head) return 0 ;
  if (ringPtr->tail  < ringPtr->head) return ringPtr->head - ringPtr->tail ;
  else return ringPtr->size - ringPtr->tail + ringPtr->head ;
}


// Add a CAN message to the specified ring buffer.
// ring = ring buffer to use.
// msg = message structure to add.
// Returns true if added, false if the ring is full.
bool DueCan::addToRingBuffer(dueCanRingbuffer_t *ringPtr, const dueCanFrame_t *msgPtr)
{
  uint16_t nextEntry;
  //nextEntry = (ring.head + 1) % ring.size; // AJR recoded this so it is a) faster, and b) tolerant to ring.size=0
  nextEntry = ringPtr->head;
  nextEntry++;
  if (nextEntry >= ringPtr->size) nextEntry = 0;
  /* check if the ring buffer is full */
  if (nextEntry == ringPtr->tail)
    return false; // If ring.size==0 then nextEntry remains stuck at 0, and buffer will always appear full here as long as ring.tail is also stuck at 0 (which it is in removeFromRingBuffer).
  // If ring.size==0 then buffer will always appear full if we try to add to it, and empty if we try to remove anything!
  else
  {
    /* add the element to the ring */
    memcpy ((void *) & (ringPtr->buffer[ringPtr->head]), (void *)msgPtr, sizeof (dueCanFrame_t));
    /* bump the head to point to the next free entry */
    ringPtr->head = nextEntry;
    return (true);
  }
}


// Remove a CAN message from the specified ring buffer.
// ring = ring buffer to use.
// msg = message structure to fill in.
// Returns true if a message was removed, false if the ring is empty.
bool DueCan::removeFromRingBuffer(dueCanRingbuffer_t *ringPtr, dueCanFrame_t *msgPtr) // Returns true if a message was removed, false if the ring is empty.
{
  /* check if the ring buffer has data available */
  if (isRingBufferEmpty(ringPtr) == true)
    return false ;  // Will always return true (buffer Empty) for ring.size==0 so long as ring.tail and ring.head are stuck at 0.
  else
  {
    /* copy the message */
    memcpy ((void *)msgPtr, (void *) & (ringPtr->buffer[ringPtr->tail]), sizeof (dueCanFrame_t));
    /* bump the tail pointer */
    // ring.tail = (ring.tail + 1) % ring.size; // AJR recoded this so it is a) faster, and b) tolerant to ring.size=0
    (ringPtr->tail)++;
    if (ringPtr->tail >= ringPtr->size) ringPtr->tail = 0; // If ring.size==0 then ring.tail remains stuck at 0, and ring.head remains stuck at 0 in addToRingBuffer.
    // If ring.size==0 then buffer will always appear full if we try to add to it, and empty if we try to remove anything!
    return (true);
  }
}


// =======================================================================
// =                                                                     =
// =                      LOW-LEVEL FUNCTIONS                            =
// =                                                                     =
// =======================================================================


// Enable CAN Controller.
void DueCan::enable()
{
  _m_pCan->CAN_MR |= CAN_MR_CANEN;
  if (_enablePin != 255)  digitalWrite(_enablePin, D_DueCan_ENABLE_OUTPUT_ENABLED);
}


// Disable CAN Controller.
void DueCan::disable()
{
  _m_pCan->CAN_MR &= ~CAN_MR_CANEN;
  if (_enablePin != 255)  digitalWrite(_enablePin, D_DueCan_ENABLE_OUTPUT_DISABLED);
}


// Set a CAN controller mode bit (disabling/reenabling if necessary)
void DueCan::setModeBit(uint32_t bit)
{
  uint32_t savedMR = _m_pCan->CAN_MR;
  _m_pCan->CAN_MR &= ~CAN_MR_CANEN;
  _m_pCan->CAN_MR |= bit;
  if (savedMR & CAN_MR_CANEN) {
    savedMR |= bit;
    _m_pCan->CAN_MR = savedMR;
  }
}

// Unset a CAN controller mode bit (disabling/reenabling if necessary)
void DueCan::unsetModeBit(uint32_t bit)
{
  uint32_t savedMR = _m_pCan->CAN_MR;
  _m_pCan->CAN_MR &= ~CAN_MR_CANEN;
  _m_pCan->CAN_MR &= ~bit;
  if (savedMR & CAN_MR_CANEN) {
    savedMR &= ~bit;
    _m_pCan->CAN_MR = savedMR;
  }
}


// Disable CAN Controller autobaud/listen mode.
void DueCan::disable_autobaud_listen_mode()
{
  unsetModeBit(CAN_MR_ABM);
}


// Enable CAN interrupt.
// dw_mask = Interrupt to be enabled.
void DueCan::enable_interrupt(uint32_t dw_mask)
{
  _m_pCan->CAN_IER = dw_mask;
}


// Disable CAN interrupt.
// dw_mask =  Interrupt to be disabled.
void DueCan::disable_interrupt(uint32_t dw_mask)
{
  _m_pCan->CAN_IDR = dw_mask;
}


//Send global transfer request.
// uc_mask = Mask for mailboxes that are requested to transfer.
void DueCan::global_send_transfer_cmd(uint8_t uc_mask)
{
  _m_pCan->CAN_TCR = uc_mask & GLOBAL_MAILBOX_MASK;
}


// Send single mailbox transfer request.
// mailbox = Indicate which mailbox is to be configured.
void DueCan::mailbox_send_transfer_cmd(uint8_t mailbox)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;

  _m_pCan->CAN_MB[mailbox].CAN_MCR |= CAN_MCR_MTCR;
}


void DueCan::writeTxRegisters(const dueCanFrame_t *txFramePtr, uint8_t mb)
{
  mailbox_set_id      (mb, txFramePtr->id, txFramePtr->extended);
  mailbox_set_datalen (mb, txFramePtr->length);
  mailbox_set_rtr(mb, txFramePtr->rtr);
  mailbox_set_priority(mb, txFramePtr->priority);

  for (uint8_t cnt = 0; cnt < txFramePtr->length; cnt++) // AJR 20190102. Only send txFrame.length, not a hard-coded 8 bytes
    mailbox_set_databyte(mb, cnt, txFramePtr->data.bytes[cnt]);
  global_send_transfer_cmd((0x1u << mb));
}


// Read a frame from out of the mailbox and into a software buffer
// mailbox which mailbox to read
// rxframe Pointer to a receive frame structure which we'll fill out
// Returns CAN mailbox transfer status.
uint32_t DueCan::mailbox_read(uint8_t mailbox, volatile dueCanFrame_t *rxFramePtr)
{
  uint32_t ul_status;
  uint32_t ul_retval;
  uint32_t ul_id;
  uint32_t ul_datal, ul_datah;

  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;
  ul_retval = 0;
  ul_status = _m_pCan->CAN_MB[mailbox].CAN_MSR;
  /* Check whether there is overwriting happening in Receive with Overwrite mode,
     or there're messages lost in Receive mode. */
  if ((ul_status & CAN_MSR_MRDY) && (ul_status & CAN_MSR_MMI))
    ul_retval = CAN_MAILBOX_RX_OVER;

  ul_id = _m_pCan->CAN_MB[mailbox].CAN_MID;
  if ((ul_id & CAN_MID_MIDE) == CAN_MID_MIDE)
  { //extended id
    rxFramePtr->id = ul_id & 0x1FFFFFFFu;
    rxFramePtr->extended = true;
  } else { //standard ID
    rxFramePtr->id = (ul_id  >> CAN_MID_MIDvA_Pos) & 0x7ffu;
    rxFramePtr->extended = false;
  }

  rxFramePtr->fid    = _m_pCan->CAN_MB[mailbox].CAN_MFID;
  rxFramePtr->length = (ul_status & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos;
  rxFramePtr->time   = (ul_status & CAN_MSR_MTIMESTAMP_Msk);
  rxFramePtr->rtr    = (_m_pCan->CAN_MB[mailbox].CAN_MSR & CAN_MSR_MRTR) ? 1 : 0 ;
  ul_datal        = _m_pCan->CAN_MB[mailbox].CAN_MDL;
  ul_datah        = _m_pCan->CAN_MB[mailbox].CAN_MDH;

  rxFramePtr->data.high = ul_datah;
  rxFramePtr->data.low = ul_datal;
  /* Read the mailbox status again to check whether the software needs to re-read mailbox data register. */
  ul_status = _m_pCan->CAN_MB[mailbox].CAN_MSR;
  if (ul_status & CAN_MSR_MMI)  ul_retval |= CAN_MAILBOX_RX_NEED_RD_AGAIN ;
  else                          ul_retval |= CAN_MAILBOX_TRANSFER_OK ;
  /* Enable next receive process. */
  mailbox_send_transfer_cmd(mailbox);
  return ul_retval;
}


// Set the mode of the given mailbox
// mailbox = Which mailbox to set (0-7)
// mode = The mode to set mailbox to
// Note Modes: 0 = Disabled, 1 = RX, 2 = RX with overwrite, 3 = TX, 4 = consumer 5 = producer
void DueCan::mailbox_set_mode(uint8_t mailbox, uint8_t mode)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;
  if (mode > 5)  mode = 0 ;  //set disabled on invalid mode
  _m_pCan->CAN_MB[mailbox].CAN_MMR =
    (_m_pCan->CAN_MB[mailbox].CAN_MMR & ~CAN_MMR_MOT_Msk) | (mode << CAN_MMR_MOT_Pos);
}


// Set the transmission priority for given mailbox
// mailbox = The mailbox to use
// pri = The priority to set (0-15 in descending priority)
void DueCan::mailbox_set_priority(uint8_t mailbox, uint8_t pri)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;
  _m_pCan->CAN_MB[mailbox].CAN_MMR = (_m_pCan->CAN_MB[mailbox].CAN_MMR & ~CAN_MMR_PRIOR_Msk) | (pri << CAN_MMR_PRIOR_Pos);
}


// Set mask for RX on the given mailbox
// mailbox = The mailbox to use
// mask = The mask to set
// ext = Whether this should be an extended mask or not
void DueCan::mailbox_set_accept_mask(uint8_t mailbox, uint32_t mask, bool ext)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;

  if (ext) {
    _m_pCan->CAN_MB[mailbox].CAN_MAM  = mask | CAN_MAM_MIDE;
    _m_pCan->CAN_MB[mailbox].CAN_MID |= CAN_MAM_MIDE;

  } else {
    _m_pCan->CAN_MB[mailbox].CAN_MAM  = CAN_MAM_MIDvA(mask);
    _m_pCan->CAN_MB[mailbox].CAN_MID &= ~CAN_MAM_MIDE;
  }
}


// Sets the ID portion of the given mailbox
// mailbox = The mailbox to set (0-7)
// id = The ID to set (11 or 29 bit)
// extended = Boolean indicating if this ID should be designated as extended
void DueCan::mailbox_set_id(uint8_t mailbox, uint32_t id, bool extended)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;
  if (extended) {
    _m_pCan->CAN_MB[mailbox].CAN_MID = id | CAN_MID_MIDE;
  }
  else {
    _m_pCan->CAN_MB[mailbox].CAN_MID = CAN_MID_MIDvA(id);
  }
}


// Set value of one byte of data for mailbox
// mailbox = Which mailbox (0-7)
// bytepos = Which byte to set (0-7)
// val = The byte value to set
void DueCan::mailbox_set_databyte(uint8_t mailbox, uint8_t bytepos, uint8_t val)
{
  uint8_t  shift;    // how many bits to shift
  uint32_t working;  // working copy of the relevant data int

  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;
  if (bytepos > 7)  bytepos = 7 ;
  shift = 8 * (bytepos & 0x03); //how many bits to shift up into position
  if (bytepos < 4)
  { //low data block
    working  = _m_pCan->CAN_MB[mailbox].CAN_MDL & ~(0xFF << shift); //mask out where we have to be
    working |= (val << shift);
    _m_pCan->CAN_MB[mailbox].CAN_MDL = working;
  } else { //high data block
    working  = _m_pCan->CAN_MB[mailbox].CAN_MDH & ~(0xFF << shift); //mask out where we have to be
    working |= (val << shift);
    _m_pCan->CAN_MB[mailbox].CAN_MDH = working;
  }
}


// Set RTR for given mailbox
// mailbox = Which mailbox? (0-7)
// rtr = value of RTR {0,1}
void  DueCan::mailbox_set_rtr (uint8_t mailbox,  uint8_t rtr)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1;

  if (rtr)  _m_pCan->CAN_MB[mailbox].CAN_MSR |=  CAN_MSR_MRTR;
  else      _m_pCan->CAN_MB[mailbox].CAN_MSR &= ~CAN_MSR_MRTR;
}

// Set # of data bytes for given mailbox
// mailbox = Which mailbox? (0-7)
// dlen = The number of data bytes to use (0-8)
void DueCan::mailbox_set_datalen(uint8_t mailbox, uint8_t dlen)
{
  if (mailbox >= D_DueCanNumberOfMailboxes)  mailbox = D_DueCanNumberOfMailboxes - 1 ;
  if (dlen > 8)  dlen = 8 ;
  _m_pCan->CAN_MB[mailbox].CAN_MCR =
    (_m_pCan->CAN_MB[mailbox].CAN_MCR & ~CAN_MCR_MDLC_Msk) | CAN_MCR_MDLC(dlen);
}


// =======================================================================
// =                                                                     =
// =                      RECEIVING                                      =
// =                                                                     =
// =======================================================================


// Retrieve a frame from the RX buffer
// buffer Reference to the frame structure to fill out
// Returns 0 if no frames waiting to be received from the ring buffer, 1 if a frame was returned
bool DueCan::pullFrameFromRxRingBuffer(dueCanFrame_t *rxFramePtr)
{
  irqLock();
  bool result = removeFromRingBuffer(&_rxRing, rxFramePtr);
  irqRelease();
  return result;
}


uint16_t DueCan::rxRingNumberOfMessages() // Returns the number of messages in the ring buffer
{
  irqLock();
  uint16_t val = ringBufferCount(&_rxRing);
  irqRelease();
  return val;
}


int DueCan::rxRingStatus() // Returns 0:No Rx messages in ring, 1:1 or more messages, -1:Ring is full (or size==0)
{
  int status;
  irqLock();
  uint16_t val = ringBufferCount(&_rxRing);
  irqRelease();
  if (val >= _rxRing.size) // Robust when _sizeRxBuffer = _rxRing.size = 0, and then always returns -1:Ring is full
    status = -1;
  else if (val == 0)
    status = 0;
  else
    status = 1;
  return status;
}


bool DueCan::rxRingHasAtLeastOneMessageInIt() // Returns true if there are frames waiting in buffer, otherwise false. If ring.size==0, returns false.
{
  irqLock();
  bool result = !isRingBufferEmpty(&_rxRing); // isRingBufferEmpty Will always return true (buffer Empty) for ring.size==0 so long as ring.tail and ring.head are stuck at 0.
  irqRelease();
  return result;
}



// =======================================================================
// =                                                                     =
// =                      ISRs and Interrupt configuration               =
// =                                                                     =
// =======================================================================


// Handle all interrupt reasons
// Call this from CAN0_Handler() in the main application context.
void DueCan::interruptHandler()
{
  uint32_t  ul_status = _m_pCan->CAN_SR ;  // Get interrupt Status Register
  
  uint32_t  interruptsEnabled = _m_pCan->CAN_IMR;
  // I find that sometimes, mailbox interrupts for tx mailboxes end up here even when the relevant mailbox interrupts have recently been disabled.
  // This is (in my case) mailbox 7 only, but to be consistent treat all boxes 0-7 the same.
  // So now we also trap below for the interrupts actually being enabled.

  if ((ul_status & CAN_SR_MB0) && (interruptsEnabled & 1<<0)) mailbox_int_handler(0 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB1) && (interruptsEnabled & 1<<1)) mailbox_int_handler(1 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB2) && (interruptsEnabled & 1<<2)) mailbox_int_handler(2 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB3) && (interruptsEnabled & 1<<3)) mailbox_int_handler(3 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB4) && (interruptsEnabled & 1<<4)) mailbox_int_handler(4 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB5) && (interruptsEnabled & 1<<5)) mailbox_int_handler(5 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB6) && (interruptsEnabled & 1<<6)) mailbox_int_handler(6 /*, ul_status */) ;
  if ((ul_status & CAN_SR_MB7) && (interruptsEnabled & 1<<7)) mailbox_int_handler(7 /*, ul_status */) ;

  if (ul_status & CAN_SR_ERRA)    _numBusErrors++ ;  // error active
  if (ul_status & CAN_SR_WARN)    {} ;              // warning limit
  if (ul_status & CAN_SR_ERRP)    {} ;              // error passive
  if (ul_status & CAN_SR_BOFF)    _numBusErrors++ ;  // bus off
  if (ul_status & CAN_SR_SLEEP)   {} ;              // controller in sleep mode
  if (ul_status & CAN_SR_WAKEUP)  {} ;              // controller woke up
  if (ul_status & CAN_SR_TOVF)    {} ;              // timer overflow
  if (ul_status & CAN_SR_TSTP)    {} ;              // timestamp - start or end of frame
  if (ul_status & CAN_SR_CERR)    {} ;              // CRC error in mailbox
  if (ul_status & CAN_SR_SERR)    _numBusErrors++ ;  // stuffing error in mailbox
  if (ul_status & CAN_SR_AERR)    _numBusErrors++ ;  // ack error
  if (ul_status & CAN_SR_FERR)    _numBusErrors++ ;  // form error
  if (ul_status & CAN_SR_BERR)    _numBusErrors++ ;  // bit error
}


// Get the IER (interrupt mask) for the specified mailbox index.
// mailbox - the index of the mailbox to get the IER for
// Returns the IER of the specified mailbox
uint32_t  DueCan::getMailboxIer (int8_t mailbox) // Returns the IER of the specified mailbox
{
  switch (mailbox) {
    case 0  :  return CAN_IER_MB0;
    case 1  :  return CAN_IER_MB1;
    case 2  :  return CAN_IER_MB2;
    case 3  :  return CAN_IER_MB3;
    case 4  :  return CAN_IER_MB4;
    case 5  :  return CAN_IER_MB5;
    case 6  :  return CAN_IER_MB6;
    case 7  :  return CAN_IER_MB7;
    default :  return 0;
  }
  return 0;
}


// Handle a mailbox interrupt event
// mb = which mailbox generated this event
// ul_status = The status register of the canbus hardware
void DueCan::mailbox_int_handler(uint8_t mb /*,  uint32_t ul_status*/ )
{
  dueCanFrame_t        tempFrame;
  dueCanRingbuffer_t   *ringPtr;

  // if (mb >= getNumMailBoxes())  mb = getNumMailBoxes() - 1 ; // AJR 20200922 commented out and replaced
  if (mb < getNumMailBoxes())
  { // if (mb < getNumMailBoxes())  AJR 20200922
    _D_DueCanDebugNoLn(F("DueCan::Interrupt on mailbox "));
    _D_DueCanDebug(mb);
    if (_m_pCan->CAN_MB[mb].CAN_MSR & CAN_MSR_MRDY) { //mailbox signals it is ready
      switch (((_m_pCan->CAN_MB[mb].CAN_MMR >> 24) & 7)) { //what sort of mailbox is it?
        case 1: //receive
        case 2: //receive w/ overwrite
        case 4: //consumer - technically still a receive buffer
          mailbox_read(mb, &tempFrame);
          _numRxFrames++;
          //First, try to send a callback. If no callback registered then buffer the frame.
          if (_isrCallbackPointers[mb].functionPointer) // Has any valid pointer value been set?
          {
            // THIS IS WHAT ACTUALLY HAPPENS. IT GOES BACK DIRECTLY TO CanBus::serviceRxInterruptCallbacksFromDueCan
            // which is configured for each Tx mailbox by CanBus::configureRxInterruptCallbacksFromDueCan
            // which callsDueCan::connectIsrCallback()
            _D_DueCanDebug(F("DueCan::Received a message and sending it to the callback function in dueCan.mailbox_int_handler() ..."));
            _isrCallbackPointers[mb].functionPointer(_isrCallbackPointers[mb].objectInstancePointer, &tempFrame); // e.g. canBus.staticServiceRxInterruptCallbacksFromDueCan(void *objectInstancePtr, CAN_FRAME *incomingFramePtr)
          }
          else
          {
            // SO ACTUALLY, WE NEVER GET HERE AND THE GLOBAL TX RINGBUFFER IS NEVER USED!
            addToRingBuffer(&_rxRing, &tempFrame); // if no callback was active then queue the frame in the Rx ring buffer
            _D_DueCanDebug(F("DueCan::Received a message and placed it into the ringbuffer in dueCan.mailbox_int_handler()."));
          }
          break;

        case 3: //transmit
          _D_DueCanDebug(F("DueCan::Tx interrupt in dueCan.mailbox_int_handler()."));
          _D_DueCanDebugNoLn(F("CAN_IMR (Interrupts enabled? Bit 0 MB0 up to bit 7 MB7) : ")); // Bit 0 MB0 up to bit 7 MB7
          _D_DueCanDebug(_m_pCan->CAN_IMR);
          _D_DueCanDebugNoLn(F("CAN_SR  (Event?              Bit 0 MB0 up to bit 7 MB7) : ")); // Bit 0 MB0 up to bit 7 MB7
          _D_DueCanDebug(ul_status);
          ringPtr = usesGlobalTxRing(mb) ? &_txRing : &_txRings[mb];
          if (removeFromRingBuffer(ringPtr, &tempFrame)) //if there is a frame in the queue to send
          {
            _D_DueCanDebug(F("DueCan::Pulled a Tx message from the ringbuffer and sending to Tx mailbox "));
            writeTxRegisters(&tempFrame, mb);
          }
          else
          {
            _D_DueCanDebug(F("DueCan::No more buffered Tx messages to send. Disabling the Tx interrupt."));
            disable_interrupt(0x01 << mb);
          }
          break;

        case 5: // producer - technically still a transmit buffer
          _D_DueCanDebug(F("DueCan::Producer mailbox. No code for this case!"));
          break;
        default:
          _D_DueCanDebug(F("DueCan::Unknown mailbox type. No code for this case!"));
          break;

      } // End: Switch on what sort of mailbox is it?
    } // End: mailbox signals it is ready
  } // End: if (mb < getNumMailBoxes())  AJR 20200922
}


// Interrupt dispatchers - Never directly call these
// Note. These two functions needed because interrupt handlers cannot be part of a class
// AJR 20181217 Moved to the main program

/*
  void CAN0_Handler(void)
  {
	Can0.interruptHandler();
  }
  void CAN1_Handler(void)
  {
	Can1.interruptHandler();
  }
*/

// Instantiate the two canbus adapters
// AJR 20181217 Moved to the main program
//DueCan Can0(CAN0, CAN0_EN, 1, sizeTxBuffer, sizeRxBuffer);
//DueCan Can1(CAN1, CAN1_EN, 1, sizeTxBuffer, sizeRxBuffer);

#endif // #ifdef BOARD_DUE
