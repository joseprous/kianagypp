%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {map_parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include "Map.hpp"   
    class Map_Driver;

    template <typename T>
        void add_append_vector(std::vector<T> &a, T v, std::vector<T> &b)
    {
        a.reserve(a.size() + 1 + b.size());
        a.push_back(v);
        std::move(b.begin(), b.end(), std::back_inserter(a));
    }
}
// The parsing context.
%param { Map_Driver& driver }
%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};
%define parse.trace
%define parse.error verbose
%code
{
# include "Map_Driver.hpp"
}
%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  LPAREN  "("
  RPAREN  ")"
  LBRACE  "{"
  RBRACE  "}"
;
%token <double> NUMBER "number"
%token <double> FNUMBER "fnumber"
%token <std::string> TEXTURE "texture"
%token <std::string> STRINGL "stringl"

%token <std::string> IDENTIFIER "identifier"

%type <Map> map
%type <std::vector<entity> > entlist
%type <entity> entity
%type <std::vector<header> > headers
%type <std::vector<rawbrush> > brushes 
%type <std::vector<rawplane> > planes 
%type <glm::dvec3> point
%type <std::vector<double> > numlist

%printer {  } <*>;
%%
%start map;

map: entlist {driver.map = Map {$1};}
   ;

entlist: entity entlist {add_append_vector($$,$1,$2);}
   |                    {}
   ;

entity: LBRACE headers brushes RBRACE {$$= entity{$2,$3};}
      ;

headers: STRINGL STRINGL headers {add_append_vector($$, header{$1,$2},$3);}
      |                          {}
      ;

brushes: LBRACE planes RBRACE brushes {add_append_vector($$,rawbrush{$2},$4);}
       |                        {}
       ;

planes: point point point TEXTURE numlist planes {add_append_vector($$,rawplane{$1,$2,$3,$4,$5},$6);}
        |                                        {}
	;

point: LPAREN FNUMBER FNUMBER FNUMBER RPAREN {$$=glm::vec3($2,$3,$4);}
     ;

numlist: NUMBER numlist {add_append_vector($$,$1,$2);}
| FNUMBER numlist {add_append_vector($$,$1,$2);}
     |                 {}
     ;

%%

void yy::map_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
