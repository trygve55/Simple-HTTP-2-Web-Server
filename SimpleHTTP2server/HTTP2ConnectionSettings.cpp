#include "HTTP2ConnectionSettings.hpp"

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

void HTTP2ConnectionSettings::buildPayload(char *payload, unsigned int length) {
  int values[6] = {
      Header_Table_Size,
      Enable_Push,
      Max_Concurrent_Streams,
      Initial_Window_Size,
      Max_Frame_Size,
      Max_Header_List_Size
  };
  for (unsigned int n = 0; n < 6; n++) {// Correct?
    for(unsigned int i = 0, iD = 1; i < length; i += 6, ++iD) {
      payload[i] = (iD >> 8) & 0xFF;
      payload[i + 1] = iD & 0xFF;
  
      for(unsigned int j = 2, b = 24; j <= 5; j++, b -= 8) {payload[i + j] = (values[n] >> b) & 0xFF;}
    }
  }
}

