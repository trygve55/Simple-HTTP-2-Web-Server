/**
 * The settings supported by HTTP2, any other setting must give a connection error.
 * https://tools.ietf.org/html/rfc7540
 **/
struct Settings {
  bool ENABLE_PUSH = 0;
  int HEADER_TABLE_SIZE = 4096,
      MAX_CONCURRENT_STREAMS = 100,
      INITIAL_WINDOW_SIZE = 65535,
      MAX_HEADER_LIST_SIZE = 16384,
      MAX_FRAME_SIZE = 0;
  
  Settings(char *payload) {
    
  }
};

/*
* SETTINGS_HEADER_TABLE_SIZE (0x1)
* SETTINGS_ENABLE_PUSH (0x2)
* SETTINGS_MAX_CONCURRENT_STREAMS (0x3)
* SETTINGS_INITIAL_WINDOW_SIZE (0x4)
* SETTINGS_MAX_FRAME_SIZE (0x5)
* SETTINGS_MAX_HEADER_LIST_SIZE (0x6)
*/