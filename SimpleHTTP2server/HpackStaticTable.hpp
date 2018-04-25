#pragma once

#include <string>

/**
 * The static table values that are recognized by HTTP2.
 **/
struct StaticTableLine {
  uint8_t index;
  std::string header_name, header_value;
  
  StaticTableLine() {}
  StaticTableLine(uint8_t index, std::string header_name, std::string header_value) : index(index), header_name(header_name), header_value(header_value) {}
};

struct StaticTable {static const StaticTableLine static_table[61];};