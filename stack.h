/*J Femister
  CSE 109 - 010
  Fall 2015
*/

#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

/* 
   A simple implementation of a stack for ints.
*/

// This is a comment
class SymbolTable;
class Stack {
public:
  // Constructors
  Stack(int n);
  Stack(); // Default constructor
  Stack(const Stack& s); // Copy Constructor - deep copy

  // Destructor
  ~Stack();

  // Regular Member Functions
  void push(SymbolTable scope);
  SymbolTable pop();
  SymbolTable peek() const;
  int gettos();

  // Inline Member Function
  bool isEmpty() const { return tos == EMPTY; } // inline member function

  // Overloaded Operator Friend Functions
  // friend ostream& operator<<(ostream& out, const Stack& s);

  // Overloaded Operator Member Functions
  const SymbolTable& operator[](int i) const; // rvalue

  SymbolTable& operator [](int i); // lvalue
  
private:
  SymbolTable *stack;
  int size;
  int tos;
  static const int EMPTY = -1;
  void checkIndex(int i) const;
};

#endif

