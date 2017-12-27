//George Cooper
//gcc219
//Header for lexer.
#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include "token.h"
using namespace std;

class Lexer
{

 public:
  Lexer(istream& input);
  ~Lexer();
  Token nextToken();

 private:
  istream& input;
  char nextChar();
  int line;
  int pos;
  string keywords[7];
  string specials[19];
};

#endif
