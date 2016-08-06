// include the library code:
#include <string.h>

#define QUEUE_SIZE 100

// Config
unsigned int inPin    = 7;
unsigned int debugPin = 6;
unsigned int ledPin   = 5;

// State Tracking
unsigned int  _state;
bool          _posAligned;
bool          _posClock;
bool          _bitSync;
unsigned long _bitHist;
unsigned int  _bitCount;
unsigned long _bitShiftA;
unsigned long _bitShiftB;
unsigned long _rxWordA;
unsigned long _rxWordB;
unsigned int  _stateCount;
unsigned long _ledTime;

// Queue variables
unsigned int _queueRead;
unsigned int _queueWrite;
unsigned int _queueWidth[QUEUE_SIZE];
unsigned int _queueValue[QUEUE_SIZE];

// Function prototypes
bool rfRxPulse();
bool rfRxBit(bool value);

bool queuePush  ( unsigned int  width, unsigned int  value );
bool queuePop   ( unsigned int *width, unsigned int *value );

// Initialize
void setup() {

   // Set and init input
   pinMode(inPin,INPUT);
   //digitalWrite(inPin,HIGH); // Enable pullup

   // Init variables
   _state       = false;
   _posAligned  = false;
   _posClock    = false;
   _bitSync     = false;
   _bitHist     = 0;
   _bitCount    = 0;
   _bitShiftA   = 0;
   _bitShiftB   = 0;
   _rxWordA     = 0;
   _rxWordB     = 0;
   _stateCount  = 0;
   _queueRead   = 0;
   _queueWrite  = 0;

   // Setup sync debug
   pinMode(debugPin,OUTPUT);
   digitalWrite(debugPin,LOW);

   // Setup green LED
   pinMode(ledPin,OUTPUT);
   digitalWrite(ledPin,LOW);
   _ledTime = 0;

   // Setup serial
   Serial.begin(9600);

   // Print hello world
   Serial.println("Debug: Waiting for RF Signals");

   // Setup 100Khz Timer
   noInterrupts();

   //set timer2 interrupt at 100kHz
   TCCR2A = 0; // set entire TCCR2A register to 0
   TCCR2B = 0; // same for TCCR2B
   TCNT2  = 0; // initialize counter value to 0

   // set compare match register for 100khz increments
   //OCR2A = 19;  // = ((16*10^6) / (8*100000))-1 // 100Khz
   OCR2A = 39;  // = ((16*10^6) / (8*50000))-1  // 50Khz
   //OCR2A = 199; // = ((16*10^6) / (8*10000))-1  // 10Khz

   // turn on CTC mode
   TCCR2A |= (1 << WGM21);

   // Set CS21 bit for 8 prescaler
   TCCR2B |= (1 << CS21);  

   // enable timer compare interrupt
   TIMSK2 |= (1 << OCIE2A);

   interrupts();

}


// Main loop
void loop() {

   // Time
   unsigned long time = millis();

   // Poll the input pin
   if ( rfRxPulse() ) {
      digitalWrite(ledPin,LOW);
      _ledTime = time;
      Serial.print("Data: 0x");
      Serial.print(_rxWordA, HEX);
      Serial.print(" 0x");
      Serial.print(_rxWordB, HEX);
      Serial.println("");
   }

   if ( (time - _ledTime) > 5000 ) digitalWrite(ledPin,HIGH);

}


// 100Khz
ISR(TIMER2_COMPA_vect) {
   unsigned int newState;

   //_stateCount += 10; // 100Khz / 10uS
   _stateCount += 20;   // 50Khz / 20uS 
   //_stateCount += 100;   // 10Khz / 100uS

   // Get state and time
   newState = (digitalRead(inPin) == HIGH)?1:0;

   // Going High, was low
   if ( (_state == 0) && (newState == 1) ) {
      queuePush(_stateCount,0);
      _state      = 1;
      _stateCount = 0;
   }

   // Going low, was high
   else if ( (_state == 1) && (newState == 0) ) {
      queuePush(_stateCount,1);
      _state      = 0;
      _stateCount = 0;
   }
}


// RX Pulse
bool rfRxPulse() {
   bool         longPulse;
   bool         posPulse;
   bool         validPulse;
   unsigned int state;
   unsigned int width;

   // Get entry from queue
   if ( ! queuePop(&width,&state) ) return(false);

   // Default pulse
   validPulse = false;

   // Detect pulse type
   if (  state && (width < 300) ) { // On Short
      longPulse  = false;
      posPulse   = true;
      validPulse = true;
   }
   if (  state && (width >= 300) ) { // On Long
      longPulse  = true;
      posPulse   = true;
      validPulse = true;
   }
   if ( (!state) && (width < 440) ) { // Off Short
      longPulse  = false;
      posPulse   = false;
      validPulse = true;
   }
   if ( (!state) && (width >= 440) ) { // Off Long
      longPulse  = true;
      posPulse   = false;
      validPulse = true;
   }

   // We adjust the posClock value before evaluating the transtion.
   // The resulting value of posClock is what it was just before the transition.
   // Bits are evaluated when posClock is true. 
   if ( validPulse ) {

      // Bit count is zero and pulse is positive
      // we are now in the negative portion of clock period
      if ( (!_posAligned) && posPulse ) {
         _posClock   = true;
         _posAligned = true;
      }

      // Otherwise toggle clock polarity on short pulses
      else if ( ! longPulse ) _posClock = ! _posClock;
   }

   // Invalid pulse, clear bit count and clear pos clock
   else {

      if ( _bitSync ) {
         Serial.print("Debug: Invalid Pulse: Width ");
         Serial.print(width);
         Serial.print(" Count ");
         Serial.print(_bitCount);
         Serial.print(" Pos ");
         Serial.print(posPulse);
         Serial.print(" Long ");
         Serial.print(longPulse);
         Serial.println("");
      }

      _posAligned  = false;
      _posClock    = false;
      _bitSync     = false;
      _bitHist     = 0;

      digitalWrite(debugPin,LOW);
   }

   // If we are in a long transition and posClock is false 
   // then there is a problem, clear bit count
   if ( validPulse && longPulse && !_posClock ) {

      if ( _bitSync ) {
         Serial.print("Debug: Bad Transition: Width ");
         Serial.print(width);
         Serial.print(" Count ");
         Serial.print(_bitCount);
         Serial.print(" Pos ");
         Serial.print(posPulse);
         Serial.print(" Long ");
         Serial.print(longPulse);
         Serial.println("");
      }

      _posAligned  = false;
      _posClock    = false;
      _bitSync     = false;
      _bitHist     = 0;
      validPulse   = false;

      digitalWrite(debugPin,LOW);
   }

   // Valid transition
   if ( validPulse && _posClock ) return(rfRxBit(posPulse));
   else return(false);
}

// Receive a bit
bool rfRxBit(bool value) {

   // Shift bit history
   _bitHist =   _bitHist << 1;
   if ( value ) _bitHist |= 1;

   // Not sycned
   if ( ! _bitSync ) {
      _bitCount  = 0;
      _bitShiftA = 0;
      _bitShiftB = 0;

      // Sync plus partial preamble, 1001 0010 0101 0101
      if ( (_bitHist & 0xFFFF) == 0x9255 ) {
         Serial.println("Debug: Got Sync");
         _bitSync = true;

         digitalWrite(debugPin,HIGH);
      }
   }

   // Post sync bits
   else {

      // Next bit
      if ( _bitCount < 32 ) {
         _bitShiftA = _bitShiftA << 1;
         if ( value ) _bitShiftA |= 1;
      }
      else {
         _bitShiftB = _bitShiftB << 1;
         if ( value ) _bitShiftB |= 1;
      }
      _bitCount++;

      // Finished the word
      if ( _bitCount == 64 ) {
         _rxWordA  = _bitShiftA;
         _rxWordB  = _bitShiftB;
         _bitSync  = false;
         _bitHist  = 0;

         digitalWrite(debugPin,LOW);

         return(true);
      }
   }
   return(false);
}


bool queuePush ( unsigned int width, unsigned int value) {
   unsigned int next;

   next = (_queueWrite + 1 ) % QUEUE_SIZE;
   if ( next != _queueRead ) {
      _queueWidth[_queueWrite] = width;
      _queueValue[_queueWrite] = value;
      _queueWrite = next;
      return(true);
   } else return(false);
}

bool queuePop ( unsigned int * width, unsigned int * value ) {
   unsigned int next;

   if ( _queueRead == _queueWrite ) return(false);

   next = ( _queueRead + 1 ) % QUEUE_SIZE;

   *width = _queueWidth[_queueRead];
   *value = _queueValue[_queueRead];

   _queueRead = next;

   return(true);
}

