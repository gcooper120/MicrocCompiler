//George Cooper
//gcc219
//cse109
//Token class, includes get functions and constructors.
#include <cstdlib>
#include <stdio.h>
#include <string>
#include "token.h"

Token::Token(){}
Token::Token(int t, string l, int li, int p):type(t),lexeme(l),line(li),pos(p) {}

Token::~Token() {}

int Token::getType()
{
  return type;
}

string Token::getLexeme()
{
  return lexeme;
}

int Token::getLine()
{
  return line;
}

int Token::getPos()
{
  return pos;
}
