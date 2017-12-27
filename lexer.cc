//George Cooper
//CSE 109
//gcc219
//Lexer class, this has the functions that get the next character in the file
//and that return the individual tokens to the calling program.
//It also contains an array of keywords and special characters.
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include "lexer.h"

Lexer::Lexer(istream& in):input(in)
{
  line = 0;
  pos = 0;
  
  keywords[0] = "if";
  keywords[1] = "else";
  keywords[2] = "while";
  keywords[3] = "function";
  keywords[4] = "var";
  keywords[5] = "printf";
  keywords[6] = "return";

  specials[0] =   "+";
  specials[1] =   "-";
  specials[2] =   "*";
  specials[3] =   "/";
  specials[4] =   "=";
  specials[5] =   "==";
  specials[6] =   "!=";
  specials[7] =   "<";
  specials[8] =   "<=";
  specials[9] =   ">";
  specials[10] =   ">=";
  specials[11] =   "&&";
  specials[12] =   "||";
  specials[13] =   "(";
  specials[14] =   ")";
  specials[15] =   "{";
  specials[16] =   "}";
  specials[17] =   ",";
  specials[18] =   ";";
}

Lexer::~Lexer() {}

char Lexer::nextChar()
{
  char next = input.get();
  if (next == EOF)
    {
      return '#';
    }
  else if (next == '\n')
    {
      line++;
      pos = 0;
      return ' ';
    }
  else
    {
      pos++;
      return next;
    }
}

Token Lexer::nextToken()
{
  int startPos = 0;
  int startLine = 0;
  int type = 0;
  string tokenContents = "";
  //Remove white space
  while (input.peek() == ' ' || input.peek() == '\n' || input.peek() == '\t')
    {
      nextChar();
    }
  //Check if first is letter
  if (isalpha(input.peek()))
    {
      tokenContents += nextChar();
      startPos = pos;
      startLine = line;
      while (isalnum(input.peek()))
	{
	  tokenContents += nextChar();
	}
      type = 1;
      for (int i = 0; i < 7; i++)
	{
	  if (tokenContents.compare(keywords[i]) == 0)
	  {
	    type =  i + 23;
	  }
	}
      
    }
  //Check if first is a digit
  else if (isdigit(input.peek()))
    {
      tokenContents += nextChar();
      startPos = pos;
      startLine = line;
      while (isdigit(input.peek()))
	{
	  tokenContents += nextChar();
	}
      type = 2;
    }
  //Check if first is a double quote
  else if (input.peek() == '"')
    {
      tokenContents += nextChar();
      startPos = pos;
      startLine = line;
      while (input.peek() != '"')
	{
	  tokenContents += nextChar();
	}
      tokenContents += nextChar();
      type = 3;
    }
  //Check if first is a special Char
  else if (ispunct(input.peek()))
    {
      tokenContents += nextChar();
      startPos = pos;
      startLine = line;
      while (ispunct(input.peek()) && tokenContents != "(" && tokenContents != ")" && tokenContents != "{" && tokenContents != "}" && tokenContents != ";")
	{
	  tokenContents += nextChar();
	}
      
      for (int i = 0; i < 19; i++)
        {
          if (tokenContents.compare(specials[i]) == 0)
	    {
	      type =  i + 4;
	    }
        }

    }
  //Check if first is EOF
  else if (input.peek() == EOF)
    {
      tokenContents += nextChar();
      startPos = pos;
      startLine = line;
      type = 30;
    }
  //Returns error if none of the above.
  if (type == 0)
    {
      tokenContents += nextChar();
      startPos = pos;
      startLine = line;
      type = 31;
    }
  Token myToken(type, tokenContents, startLine, startPos);
  return myToken;
} 
