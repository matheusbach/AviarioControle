
#include "esp_wifi.h"
#include "Arduino.h"
#include "headers.h"
#include "string"

String dateTimePaddingLeft(int datetimeUnpadded)
{
  String str;

  if (datetimeUnpadded < 10)
  {
    str.concat("0" + (String)datetimeUnpadded);
  }
  else
  {
    str = (String)datetimeUnpadded;
  }

  return str;
}

std::string returnLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    return "Fail";
  }
  String retorno;
  retorno.concat(dateTimePaddingLeft(timeinfo.tm_mday) + "/" + dateTimePaddingLeft(timeinfo.tm_mon) + " " + dateTimePaddingLeft(timeinfo.tm_hour) + ":" + dateTimePaddingLeft(timeinfo.tm_min));
  return retorno.c_str();
}

char *itob(int val, char *buf, size_t n)
{
  char *bp = buf + n;
  do
  {
    *--bp = (val % 10) + '0';
    val = val / 10;
  } while (bp != buf);
  return (buf);
}

const char* outputState(int gpio)
{
  if (digitalRead(gpio))
  {
    return "checked";
  }
  else
  {
    return "";
  }
}