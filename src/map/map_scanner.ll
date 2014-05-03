%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "map/Map_Driver.hpp"
# include "map_parser.hpp"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
static yy::location loc;
%}
%option noyywrap nounput batch debug noinput

delim [ \t]
ws {delim}+
letter [A-Za-z]
digit [0-9]
id ({letter}|{digit}|_|\-)*

number \-?{digit}+
fnumber \-?{digit}+\.{digit}+

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

{ws}   loc.step ();
[\n]+  loc.lines (yyleng); loc.step ();
\/\/.* loc.step ();

\( {return yy::map_parser::make_LPAREN(loc);}
\) {return yy::map_parser::make_RPAREN(loc);}
\{ {return yy::map_parser::make_LBRACE(loc);}
\} {return yy::map_parser::make_RBRACE(loc);}

{number}   {return yy::map_parser::make_FNUMBER(atof(yytext),loc);}
{fnumber}   {return yy::map_parser::make_FNUMBER(atof(yytext),loc);}

\"(\\.|[^\\\"])*\"  {return yy::map_parser::make_STRINGL(std::string(yytext),loc);}

NULL {return yy::map_parser::make_TEXTURE(std::string(""),loc);}
{id} {return yy::map_parser::make_TEXTURE(std::string(yytext),loc);}
{id}(\/?{id})* {return yy::map_parser::make_TEXTURE(std::string(""),loc);}

.          driver.error (loc, "invalid character");
<<EOF>>    return yy::map_parser::make_END(loc);



%%

void Map_Driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      error ("cannot open " + file + ": " + strerror(errno));
      exit (EXIT_FAILURE);
    }
}



void Map_Driver::scan_end ()
{
  fclose (yyin);
}

