/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 ,gmail.com
  Tile NMEA calculation implementation. It is used to make calculations for de communication unit and
  the visualisation unit
*/
#include <calc/mfd_calculation.h>
#include <lvgl.h>
#include <string.h>

// *** Conditional Debug & Test Info to Serial Monitor
// *** by commenting out the line(s) below the debugger and or test statements will
// *** be ommitted from the code
// #define DEBUG 1
#define TEST 1

/***********************************************************************************
   Global variables go here
*/
#ifdef TEST

/**
 * Test data set if nog real data is available
 */
String NmeaStream[] = {
    "$GPRMC,095218.000,A,5251.5621,N,00540.8482,E,4.25,201.77,120420,,,D*6D",
    "$IIVWR,151,R,02.4,N,,,,",
    "$IIMTW,12.2,C",
    "!$SDDPT,002.3,*78",
    "$IIDBK,A,0014.4,f,,,,",
    "$IIVLW,1149.1,N,001.07,N",
    "$GPGLL,5251.3091,N,00541.8037,E,151314.000,A,D*5B",
    "$PSTOB,13.0,v",
    "$IIVWR,151,R,02.3,N,,,,",
    "$IIVHW,,,000,M,01.57,N,,"};

#endif
// Variable used to pass nmea tag values like SOG etc. back and forth between objects, max 15 char's long
char cvalue[FIELD_BUFFER] = {0};

/***
 * function check if a string is a number
 */
bool isNumeric(char *value)
{
  bool result = true;
  int i = 0;
  while (value[i] != '\0' && result && i < FIELD_BUFFER)
  {
    result = (isdigit(value[i]) || value[i] == '.' || value[i] == '-');
    i++;
  }
  return result;
}

/**
 * Helper function to generate NMEA data streams for testing if
 * no NMEA data is available from talkers
 */
int softIndex = 0;
long softTimerNow = millis();

void NMEA_runSoftGenerator()
{
  if (millis() - softTimerNow > 25)
  {
    softTimerNow = millis();
    if (softIndex < 10)
    {

      // lv_log("sending msg %d data=%s\n", softIndex, NmeaStream[softIndex].c_str());
      processNMEAData(NmeaStream[softIndex++].c_str());
    }

    softIndex %= 10;
  }
}

/**
 *  function to processes the receivedChars buffer and filters sentence MWV,RM and VWR,
 * and more, which contain the SOG,COG, AWS and AWA parameters, when new data has arrived
 */
void processNMEAData(const char *buff)
{

  // lv_log("received string: %s length= %d\n", buff, strlen(buff));

  int field = 0; // the field number in the NMEA string,

  double tmpVal = 0.0;
  double tmpVar = 0.0;

  char *ptr;

  char *nmea_str;    // used to proces the tokens (seperated by ',' )
  char bfr[83];      // a copy of the referenced nmead buffer to prevent pointe problems
  strcpy(bfr, buff); // make the copy!

  field = 0;                   // ignore sentence tag, the first field as the tag
  nmea_str = strstr(bfr, ","); // find the tokens in the NMEA string
  // lv_log("nmea buff = %s\n", nmea_str);
  char *token = strtok(nmea_str, ","); // the token is actually holding the value of the fields
  // lv_log("token = %s\n", token);

  // Get all the field from the NMEAstring seperated by ','
  while (token != NULL)
  {

    // lv_log("received string: %s ci =%d  li=%d\n", buff, ci, li);
    if ((ptr = strstr(buff, "MWV")) != NULL ||
        (ptr = strstr(buff, "RMC")) != NULL ||
        (ptr = strstr(buff, "CTS")) != NULL ||
        (ptr = strstr(buff, "TOB")) != NULL ||
        (ptr = strstr(buff, "VWR")) != NULL ||
        (ptr = strstr(buff, "BAT")) != NULL ||
        (ptr = strstr(buff, "HDG")) != NULL ||
        (ptr = strstr(buff, "MTW")) != NULL ||
        (ptr = strstr(buff, "DPT")) != NULL)
    {
      // prepare for further manupulation and to prevent pointer problems
      // make a copy of the token
      strcpy(cvalue, token);
      // lv_log("value = %s\n", cvalue);
      //  Set to next field
      field++;

      // Prepare AWS and AWA values, from MWV or VWR tags
      if ((strstr(buff, "MWV") != NULL && strstr(buff, ",R,") != NULL) ||
          strstr(buff, "VWR") != NULL)
      {
        if (field == 1)
        {

          set_data_store(AWA, cvalue);
        }
        if (field == 2)
        {
          // Correct for +(SB) or -(Port) depending on value
          if (cvalue[0] == 'L' || cvalue[0] == 'T')
          {

            memmove(cvalue + 1, cvalue, FIELD_BUFFER - 2);
            cvalue[0] = '-';
          }
          set_data_store(DIR, cvalue);
        }
        if (field == 3)
        {
          set_data_store(AWS, cvalue);
        }
      }
      // Prepare for SOF and COG values from RMC tags
      if (strstr(buff, "RMC") != NULL)
      {
        if (field == 7)
        {
          set_data_store(SOG, cvalue);
        }
        if (field == 8)
        {
          set_data_store(COG, cvalue);
        }
      }
      // Prepare for DPT values, can be comming DBK,DBT or DPT tags
      if (strstr(buff, "DBK") != NULL)
      {
        if (field == 2)
        {
          set_data_store(DPT, cvalue);
        }
        if (field == 3 && cvalue[0] == 'f')
        {
          double dpt = atof(cvalue);
          dpt *= FTM;
          sprintf(cvalue, "%.1f", dpt);
          set_data_store(DPT, cvalue);
        }
      }
      else if (strstr(buff, "DBT") != NULL)
      {
        if (field == 3)
        {
          set_data_store(DPT, cvalue);
        }
      }
      else if (strstr(buff, "DPT") != NULL)
      {
        if (field == 1)
        {
          set_data_store(DPT, cvalue);
        }
      }
      // Prepare for BAT values, either from TOB or BAT tags
      if (strstr(buff, "TOB") != NULL)
      {
        if (field == 1)
        {
          set_data_store(BAT, cvalue);
        }
      }
      else if (strstr(buff, "BAT") != NULL)
      {
        if (field == 2)
        {
          set_data_store(BAT, cvalue);
        }
      }
      // Prepare for HDG values, converting from magnetic to true values
      if (strstr(buff, "HDG") != NULL)
      {
        if (field = 1)
        {
          // Magnetinc Sensor reading
          tmpVal = atof(cvalue);
        }
        if (field = 2)
        {
          // Magnetic Deviation
          tmpVar = atof(cvalue);
        }
        if (field = 3)
        {
          if (cvalue[0] = 'E')
          {
            // For Easterly deviation add to magnetic sensor reading
            // For Magnetic Heading
            tmpVal += tmpVar;
          }
          else
          {
            // For Westerly deviation subtract from magnetic sensor reading
            // for Mahnetic Heading
            tmpVal -= tmpVar;
          }
        }
        if (field = 4)
        {
          // Magnetic Variation
          tmpVar = atof(cvalue);
        }
        if (field = 5)
        {
          if (cvalue[0] = 'E')
          {
            // For Easterly variation add to magnetic heading
            // For True Heading
            tmpVal += tmpVar;
          }
          else
          {
            // For Westerly variation subtract from magnetic heading
            // for True Heading
            tmpVal -= tmpVar;
          }
        }
        // Storing True Heading!!
        sprintf(cvalue, "%.1f", tmpVal);
        set_data_store(HDG, cvalue);
      }
      // get next token
    }
    token = strtok(NULL, ",");
  }
}
