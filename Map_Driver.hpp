/*
kianagy++
Copyright (C) 2014 José Prous

This file is part of kianagy++.

kianagy++ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

kianagy++ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with kianagy++.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAP_DRIVER_HPP
# define MAP_DRIVER_HPP
# include <string>
# include <map>
#include <vector>
#include "Map.hpp"
# include "map_parser.hpp"
// Tell Flex the lexer's prototype ...
# define YY_DECL \
  yy::map_parser::symbol_type yylex (Map_Driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;
// Conducting the whole scanning and parsing of Calc++.
class Map_Driver
{
public:

    Map_Driver ();
    virtual ~Map_Driver ();

    std::map<std::string, int> variables;

    int result;
    Map map;
    // Handling the scanner.
    void scan_begin ();
    void scan_end ();
    bool trace_scanning;
    // Run the parser on file F.
    // Return 0 on success.
    int parse (const std::string& f);
    // The name of the file being parsed.
    // Used later to pass the file name to the location tracker.
    std::string file;
    // Whether parser traces should be generated.
    bool trace_parsing;
    // Error handling.
    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);
};
#endif
