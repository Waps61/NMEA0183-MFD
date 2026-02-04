#ifndef MFD_COMMUNICATION_H
#define MFD_COMMUNICATION_H

#include <HardwareSerial.h>
//#include <SPI.h>
//#include <SD.h>
#include <NMEA0183_data.h>

//*** Since the signal from the RS422-TTL converter is inverted
//*** a digital input is used as a software serial port because
//*** it can invert te signal back to its orignal pulse set
//#include <SoftwareSerial.h>



extern boolean isNumeric(char *value);
extern void mfd_setup_communication();
extern void NMEA_runSoftGenerator();
extern byte NMEA_startTalking();
extern void NMEA_startListening();

#endif // MFD_COMMUNICATION_H
