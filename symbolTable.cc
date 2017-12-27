//George Cooper


//gcc219


//cse109


//Symbol Table file
#include "symbolTable.h"
#include "stack.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;
Stack SymbolTable::scopeStack = Stack();
int SymbolTable::uniqueNumber = 0;
SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

SymbolTable SymbolTable::enterScope() {
  SymbolTable toAdd = SymbolTable();
  scopeStack.push(toAdd);
  return scopeStack.peek();
}

SymbolTable SymbolTable::exitScope() {
  return scopeStack.pop();
}

int SymbolTable::addSymbol(string sym) {
  ostringstream uniqueSym;
  uniqueSym << sym <<  "$" <<  uniqueNumber;
  uniqueNumber++;
  int i;
  for (i = 0; i < 1000; i++)
    {
      if (table[i][0].empty())
	{
	  table[i][0] = sym;
	  table[i][1] = uniqueSym.str();
	  return 1;
	}
    }
  return 0;
}

string SymbolTable::getUniqueSymbol(string sym) {
  Stack currentStack = Stack(scopeStack);
  SymbolTable currentTable = currentStack.peek();
  while (currentStack.gettos() >= 0)
    {
       for (int i = uniqueNumber; i >= 0; i--)
	{
	  if (table[i][0].compare(sym) == 0)
	    {
	      return table[i][1];
	    }
	}
      currentTable = currentStack.pop();
    }
  return "";
}
