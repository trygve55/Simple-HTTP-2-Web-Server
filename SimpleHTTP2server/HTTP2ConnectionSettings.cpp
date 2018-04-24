#include "HTTP2ConnectionSettings.hpp"
  
HTTP2ConnectionSettings::HTTP2ConnectionSettings(char *payload, unsigned int length) {
  changeSettings(payload, length);
}

bool HTTP2ConnectionSettings::changeSettings(char *payload, unsigned int length) {
  for (unsigned int i = 0, iD, value; i < length; i += 6, iD = 0, value = 0) {
    iD += payload[i] << 8;
    iD += payload[i + 1];
    
    for (unsigned int j = 2, b = 24; j <= 5; j++, b -= 8) {value += payload[i + j] << b;}
    
    switch (iD) {
    case TypeIDs.HEADER_TABLE_SIZE:
      Header_Table_Size = value;
      break;
    case TypeIDs.ENABLE_PUSH:
      if (value > 1) {return false;}
      Enable_Push = value;
      break;
    case TypeIDs.MAX_CONCURRENT_STREAMS:
      Max_Concurrent_Streams = value;
      break;
    case TypeIDs.INITIAL_WINDOW_SIZE:
      if (value > 2147483647) {return false;}
      Initial_Window_Size = value;
      break;
    case TypeIDs.MAX_FRAME_SIZE:
      if (value < 16384 || value > 16777215) {return false;}
      Max_Header_List_Size = value;
      break;
    case TypeIDs.MAX_HEADER_LIST_SIZE:
      Max_FRAME_Size = value;
      break;
    default:
      return false;
      break;
    }
  }
  return true;
}