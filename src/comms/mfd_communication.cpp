#include <Arduino.h>
/*
  Project:  Yazz_NMEAtor_ESP32.cpp, Copyright 2020, Roy Wassili
  Contact:  waps61 @gmail.com
/*
    This is the NMEA comminication unit for receiving and sending NMEA data
    Include the necessary libraries
*/
#include <calc/mfd_calculation.h>
#include <comms/mfd_communication.h>

/*
   Definitions go here
*/
// *** Conditional Debug & Test Info to Serial Monitor
// *** by commenting out the line(s) below the debugger and or test statements will
// *** be ommitted from the code
// #define DEBUG 1
#define TEST 1

/*
   If there is some special treatment needed for some NMEA sentences then
   add the their definitions to the NMEA_SPECIALTY definition
   The pre-compiler concatenates string literals by using "" in between
*/

// Declare buffers for NMEA string and display parameters
char nmeaBuffer[NMEA_BUFFER_SIZE + 1] = {0};
char __CTS[FIELD_BUFFER] = {0};
char __COG[FIELD_BUFFER] = {0};
char __SOG[FIELD_BUFFER] = {0};
char __DPT[FIELD_BUFFER] = {0};
char __AWA[FIELD_BUFFER] = {0};
char __AWS[FIELD_BUFFER] = {0};
char __TWA[FIELD_BUFFER] = {0};
char __TWS[FIELD_BUFFER] = {0};
char __HDG[FIELD_BUFFER] = {0};
char __LOG[FIELD_BUFFER] = {0};
char __TRP[FIELD_BUFFER] = {0};
char __VMG[FIELD_BUFFER] = {0};
char __CMG[FIELD_BUFFER] = {0};
char __STW[FIELD_BUFFER] = {0};
char __BAT[FIELD_BUFFER] = {0};
char __DIR[FIELD_BUFFER] = {0};
char oldVal[255] = {0}; // holds previos _BITVALUE to check if we need to send

enum NMEAReceiveStatus
{
  INVALID,
  VALID,
  RECEIVING,
  CHECKSUMMING,
  TERMINATING,
  NMEA_READY
};
byte nmeaStatus = INVALID;
byte nmeaIndex = 0;
bool nmeaDataReady = false;
bool newData = false;
unsigned long tmr1 = 0;

//SoftwareSerial nmeaSerialOut; // // signal need to be inverted for RS-232

// ----- software timer
unsigned long Timer2 = 1000000; // 500000L;                         // 500mS loop ... used when sending data to to Processing
unsigned long Stop2 = 0;

bool on = true;
byte pin = 22;

char cvalue[FIELD_BUFFER] = {0};
uint16_t tmpVal = 0;

unsigned int ci = 0; // current index of ',' to iterate through received NMEA data
unsigned int li = 0; // last index of ',' in NMEA buffer
uint16_t cp = 0;     // current index pointer in an array
int field = 0;
bool updateDisplay = false;

const byte numChars = NMEA_BUFFER_SIZE;
char receivedChars[numChars];

//bool newData = false;
//unsigned long tmr1 = 0;

/*
 * Setting for Serial interrupt
 */
//*** flag data on the listener port is ready
volatile bool listenerDataReady = false;

//*** ISR to set listerDataReady flag
void listenerReady()
{
  listenerDataReady = true;
}

/*
debugWrite() <--provides basic debug info from other tasks
takes a String as input parameter
*/
void debugWrite(String debugMsg)
{
#ifdef DEBUG
  if (debugMsg.length() > 1)
    Serial.println(debugMsg);
  else
    Serial.print(debugMsg);
#endif
}

/*
   Class definitions go here
*/

/*
  Initialize the NMEA Talker port and baudrate
  on RX/TX port 2 to the multiplexer
*/
void initializeTalker()
{
  //nmeaSerialOut.begin(TALKER_RATE, SWSERIAL_8N1, 22, TALKER_PORT, true);
#ifdef DEBUG
  debugWrite("Talker initialized...");
#endif
}

/*** Converts and adjusts the incomming values to usable values for the HMI display
 * and concatenates these values in one string so it can be send in one command to the
 * Nextion HMI in timed intervals of 50ms.
 * This is due the fact that a timer in the HMI checks on new data and refreshes the
 * display. So no need to send more data than you can chew!
 * A refresh of 20x per second is more then sufficient.
 * The string is formatted like:
 * <Sentence ID1>=<Value1>#....<Sentence IDn>=<Value_n>#
 * Sentence ID = 3 chars i.e. SOG, COG etc
 * Value can be an integer or float with 1 decimal and max 5 char long incl. delimiter
 * i.e. SOG=6.4#COG=213.2#BAT=12.5#AWA=37#AWS=15.7#
 * The order is not applicable, so can be random
 */

/** reads the softseroal port pin 10 and ckeks for valid nmea data starting with
 * character '$' only (~ and ! can be skipped as start charcter)
 */
void recvNMEAData()
{
  // static bool recvInProgress = false;
  // static byte ndx = 0;
  // char startMarker = '$';
  // char endMarker = '\n';
  // char rc;
  // while (nmeaSerial.available() > 0 && newData == false)
  // {
  //   rc = nmeaSerial.read();
  //   if (recvInProgress == true)
  //   {
  //     if (rc != endMarker)
  //     {
  //       receivedChars[ndx] = rc;
  //       ndx++;
  //       if (ndx >= numChars)
  //       {
  //         ndx = numChars - 1;
  //       }
  //     }
  //     else
  //     {
  //       receivedChars[ndx] = '\0'; // terminate the string

  //       recvInProgress = false;
  //       ndx = 0;
  //       newData = true;
  //     }
  //   }

  //   else if (rc == startMarker)
  //   {
  //     receivedChars[ndx] = rc;
  //     ndx++;
  //     recvInProgress = true;
  //   }
  // }
}

/* only processes the receivedChars buffer and filters sentence MWV,RM and VWR,
 * which contain the SOG,COG, AWS and AWA parameters, when new data has arrived
 */
void processNMEAData()
{

  String sentence = "";
  int ci, li, cp = 0;
  if (newData == true)
  {

    sentence = String(receivedChars);

    // cvalue='\0';
    tmpVal = 0;

    ci = sentence.indexOf(',', 0);
    li = sentence.indexOf(',', ci + 1);
    cp = 0;

    if (sentence.indexOf("MWV", 0) > 0 ||
        sentence.indexOf("RMC", 0) > 0 ||
        sentence.indexOf("DBK", 0) > 0 ||
        sentence.indexOf("TOB", 0) > 0 ||
        sentence.indexOf("VWR", 0) > 0 ||
        sentence.indexOf("BAT", 0) > 0 ||
        sentence.indexOf("DBT", 0) > 0 ||
        sentence.indexOf("DPT", 0) > 0)
    {
      field = 0; // ignore sentence tag
      while (li < sentence.length() && li < numChars)
      {

        cp = 0;
        while (ci + 1 < li)
        {
          cvalue[cp++] = sentence[ci + 1];
          ci++;
        }
        cvalue[cp] = '\0';
        field++;
        // only check for apparent or relative wind directions and speed
        if ((sentence.indexOf("MWV") > 0 && sentence.indexOf(",R,") > 0) ||
            sentence.indexOf("VWR") > 0)
        {
          if (field == 1)
          {
            memcpy(__AWA, cvalue, FIELD_BUFFER - 1);
          }
          if (field == 2)
          {
            memcpy(__DIR, cvalue, FIELD_BUFFER - 1);
            if (__DIR[0] == 'L' || __DIR[0] == 'T')
            {
              memmove(__AWA + 1, __AWA, FIELD_BUFFER - 2);
              __AWA[0] = '-';
            }
          }
          if (field == 3)
          {
            memcpy(__AWS, cvalue, FIELD_BUFFER - 1);
          }
        }

        if (sentence.indexOf("RMC") > 0)
        {
          if (field == 7)
          {
            memcpy(__SOG, cvalue, FIELD_BUFFER - 1);
          }
          if (field == 8)
          {
            memcpy(__COG, cvalue, FIELD_BUFFER - 1);
          }
        }
        if (sentence.indexOf("DBK") > 0)
        {
          if (field == 2)
          {
            memcpy(__DPT, cvalue, FIELD_BUFFER - 1);
          }
          if (field == 3 && cvalue[0] == 'f')
          {
            double dpt = atof(__DPT);
            dpt *= FTM;
            sprintf(__DPT, "%.1f", dpt);
          }
        }
        else if (sentence.indexOf("DBT") > 0)
        {
          if (field == 3)
          {
            memcpy(__DPT, cvalue, FIELD_BUFFER - 1);
          }
        }
        else if (sentence.indexOf("DPT") > 0)
        {
          if (field == 1)
          {
            memcpy(__DPT, cvalue, FIELD_BUFFER - 1);
          }
        }
        if (sentence.indexOf("TOB") > 0)
        {
          if (field == 1)
          {
            memcpy(__BAT, cvalue, FIELD_BUFFER - 1);
          }
        }
        else if (sentence.indexOf("BAT") > 0)
        {
          if (field == 2)
          {
            memcpy(__BAT, cvalue, FIELD_BUFFER - 1);
          }
        }
        ci = li;
        li = sentence.indexOf(',', ci + 1);
        if (li < 0 || li > numChars)
          li = sentence.length();
      }
    }
  }
}

/**********************************************************************************
  Purpose:  Helper class reading NMEA data from the serial port as a part of the multiplexer application
            - Reading NMEA0183 v1.5 data without a checksum,
*/

/*
Cleasr the inputbuffer by reading until empty, since Serial.flush does not this anymore
*/
void clearNMEAInputBuffer()
{

  while (Serial1.available() > 0)
  {
    Serial1.read();
  }
}

/*
 * Initializes UART 2 for incomming NMEA0183 data from the Robertson network
 */
void initializeListener()
{

  Serial1.begin(LISTENER_RATE, SERIAL_8N1, LISTENER_RX, LISTENER_TX, true);
  // clearNMEAInputBuffer();
#ifdef DEBUG
  debugWrite("Listener initialized...");
#endif
}

// /*
//   Decode the incomming character and test if it is valid NMEA data.
//   If true than put it the NMEA buffer and call NMEAParser object
//   to process incomming and complete MNEA sentence
// */
// void decodeNMEAInput(char cIn)
// {
//   switch (cIn)
//   {
//   case '~':
//     // reserved by NMEA
//   case '!':
//     // for AIS info
//   case '$':
//     // for general NMEA info
//     nmeaStatus = RECEIVING;
//     nmeaIndex = 0;
//     break;
//   case '*':
//     if (nmeaStatus == RECEIVING)
//     {
//       nmeaStatus = CHECKSUMMING;
//     }
//     break;
//   case '\n':
//   case '\r':
//     // in old v1.5 version, NMEA Data may not be checksummed!
//     if (nmeaStatus == RECEIVING || nmeaStatus == CHECKSUMMING)
//     {
//       nmeaDataReady = true;
//       nmeaStatus = TERMINATING;
//     }
//     else
//       nmeaStatus = INVALID;

//     break;
//   }
//   switch (nmeaStatus)
//   {
//   case INVALID:
//     // do nothing
//     nmeaIndex = 0;
//     nmeaDataReady = false;
//     break;
//   case RECEIVING:
//   case CHECKSUMMING:
//     nmeaBuffer[nmeaIndex] = cIn;
//     nmeaIndex++;
//     break;
//   case TERMINATING:

//     nmeaStatus = INVALID;
//     if (nmeaDataReady)
//     {
//       nmeaDataReady = false;

//       // Clear the remaining buffer content with '\0'
//       for (int y = nmeaIndex + 1; y < NMEA_BUFFER_SIZE + 1; y++)
//       {
//         nmeaBuffer[y] = '\0';
//       }
// #ifdef DEBUG
//       debugWrite(nmeaBuffer);
// #endif
//       NmeaParser.parseNMEASentence(nmeaBuffer);

//       // clear the NMEAbuffer with 0
//       memset(nmeaBuffer, 0, NMEA_BUFFER_SIZE + 1);
//       nmeaIndex = 0;
//     }

//     break;
//   }
// }

// /*
//  * Start listeneing for incomming NNMEA sentences
//  */
// void NMEA_tartListening()
// {
// #ifdef DEBUG
//   debugWrite("Listening....");
// #endif

//   while (Serial1.available() > 0 && nmeaStatus != TERMINATING)
//   {
//     decodeNMEAInput(Serial1.read());
//   }
// }

#ifdef TEST

String NmeaStream[10] = {
    "$IIVWR,151,R,02.4,N,,,,",
    "$IIMTW,12.2,C",
    "!AIVDM,1,1,,A,13aL<mhP000J9:PN?<jf4?vLP88B,0*2B",
    "$IIDBK,A,0014.4,f,,,,",
    "$IIVLW,1149.1,N,001.07,N",
    "$GPGLL,5251.3091,N,00541.8037,E,151314.000,A,D*5B",
    "$GPRMC,095218.000,A,5251.5621,N,00540.8482,E,4.25,201.77,120420,,,D*6D",
    "$PSTOB,13.0,v",
    "$IIVWR,151,R,02.3,N,,,,",
    "$IIVHW,,,000,M,01.57,N,,"};

int softIndex = 0;
long softTimerOld = 0;
long softTimerNow;

void NMEA_runSoftGenerator()
{
  softTimerNow = millis();

  if (softIndex < 10)
  {

    //NmeaParser.parseNMEASentence(NmeaStream[softIndex++]);

    delay(200);
  }
  else
    softIndex = 0;
}

#endif

void mfd_setup_communication()
{
  // put your setup code here, to run once:

  initializeListener();
  initializeTalker();

  //nmeaSerialOut.begin(38400, SWSERIAL_8N1, 22, TALKER_PORT, true);
}



// void loop()
// {
//   // put your main code here, to run repeatedly:

// #ifdef TEST
//   NMEA_runSoftGenerator();
// #endif

//   NMEA_startListening();

// #
//   NMEA_startTalking();

  
//   recvNMEAData();
//   if (newData)
//   {
//     processNMEAData();
    
//   }
// #ifdef WRITE_ENABLED
//   relayData();
// #endif
// }
