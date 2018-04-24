#include "HTTP2ConnectionSettings.hpp"

#include <iostream>

HTTP2ConnectionSettings::HTTP2ConnectionSettings(char *payload, unsigned int length) {
  setSettings(payload, length);
}

bool HTTP2ConnectionSettings::setSettings(char *payload, unsigned int length) {
  for(unsigned int i = 0, iD, value; i < length; i += 6, iD = 0, value = 0) {
    iD += payload[i] << 8;
    iD += payload[i + 1];

    for(unsigned int j = 2, b = 24; j <= 5; j++, b -= 8) {
      value += payload[i + j] << b;
    }

    switch(iD) {
    case TypeIDs.HEADER_TABLE_SIZE:
      Header_Table_Size = value;
      break;
    case TypeIDs.ENABLE_PUSH:
      if(value > 1) {
        return false;
      }
      Enable_Push = value;
      break;
    case TypeIDs.MAX_CONCURRENT_STREAMS:
      Max_Concurrent_Streams = value;
      break;
    case TypeIDs.INITIAL_WINDOW_SIZE:
      if(value > 2147483647) {
        return false;
      }
      Initial_Window_Size = value;
      break;
    case TypeIDs.MAX_FRAME_SIZE:
      if(value < 16384 || value > 16777215) {
        return false;
      }
      Max_Frame_Size = value;
      break;
    case TypeIDs.MAX_HEADER_LIST_SIZE:
      Max_Header_List_Size = value;
      break;
    default:
      return false;
      break;
    }
  }
  return true;
}

unsigned int HTTP2ConnectionSettings::buildPayload(char *payload) {
  
  unsigned int values[HTTP2ConnectionSettings::totalTypes] = {
      Header_Table_Size,
      Enable_Push,
      Max_Concurrent_Streams,
      Initial_Window_Size,
      Max_Frame_Size,
      Max_Header_List_Size
  };
  
  for (unsigned int n = 0; n < HTTP2ConnectionSettings::totalTypes; n++) {  
    payload[n*6] = ((n+1) >> 8) & 0xFF;
    payload[n*6 + 1] = (n+1) & 0xFF;
    payload[n*6 + 2] = (values[n] >> 24) & 0xFF;
    payload[n*6 + 3] = (values[n] >> 16) & 0xFF;
    payload[n*6 + 4] = (values[n] >> 8) & 0xFF;
    payload[n*6 + 5] = (values[n]) & 0xFF;
  }
  
  return HTTP2ConnectionSettings::totalTypes*6;
}

