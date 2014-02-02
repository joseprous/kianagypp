#include "Map_Driver.hpp"
#include "map_parser.hpp"

Map_Driver::Map_Driver ()
  : trace_scanning (false), trace_parsing (false)
{
  variables["one"] = 1;
  variables["two"] = 2;
}

Map_Driver::~Map_Driver ()
{
}

int Map_Driver::parse (const std::string &f)
{
  file = f;
  scan_begin ();
  yy::map_parser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  scan_end ();
  return res;
}

void Map_Driver::error (const yy::location& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
}

void Map_Driver::error (const std::string& m)
{
  std::cerr << m << std::endl;
}
