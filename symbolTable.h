//George Cooper


//gcc219


//cse109


//header file for SymbolTable
#ifndef SymbolTable_H
#define SymbolTable_H

#include <string>
#include <stdlib.h>
#include "stack.h"

using namespace std;

class SymbolTable
{
  public:

  SymbolTable();
  ~SymbolTable();
  SymbolTable enterScope();
  SymbolTable exitScope();
  int addSymbol(string sym);
  string getUniqueSymbol(string sym);
  string table[1000][2];
  static int uniqueNumber;
  static Stack scopeStack;
};

#endif
