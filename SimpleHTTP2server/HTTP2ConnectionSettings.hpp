/**
 * The settings supported by HTTP2, any other setting must give a connection error.
 * https://tools.ietf.org/html/rfc7540
 **/
class HTTP2ConnectionSettings {
public:
  static const struct Types {
    static const unsigned short
        HEADER_TABLE_SIZE = 0,
        ENABLE_PUSH = 2,
        MAX_CONCURRENT_STREAMS = 3,
        INITIAL_WINDOW_SIZE = 4,
        MAX_FRAME_SIZE = 5,
        MAX_HEADER_LIST_SIZE = 6;
  } TypeIDs;
  
  static const unsigned short totalTypes = 6;

  bool Enable_Push = 0;
  unsigned int Header_Table_Size = 4096,
      Max_Concurrent_Streams = 100,
      Initial_Window_Size = 65535,
      Max_Frame_Size = 16384,
      Max_Header_List_Size = 0;

  HTTP2ConnectionSettings() {}

  HTTP2ConnectionSettings(char *payload, unsigned int length);

  bool setSettings(char *payload, unsigned int length);
  
  unsigned int buildPayload(char *payload);
};