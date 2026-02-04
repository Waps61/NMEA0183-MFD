/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Tile NMEA calculation implementation. It is used to make calculations for de communication unit and 
  the visualisation unit
*/ 
#include <calc/mfd_calculation.h>

/*** function check if a string is a number
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


/***********************************************************************************
   Global variables go here
*/

