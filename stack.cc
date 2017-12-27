#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "stack.h"
#include "symbolTable.h"

using namespace std;

Stack::Stack(int n):size(n),tos(Stack::EMPTY) {
  stack = new SymbolTable[size];
}

Stack::Stack():size(10),tos(Stack::EMPTY) {
    stack = new SymbolTable[size];
}

Stack::Stack(const Stack& s)  { // Copy constructor
  size = s.size;
  tos = s.tos;
  stack = new SymbolTable[size];
  for (int i = 0; i < size; ++i) {
    stack[i] = s.stack[i];
  }
}


Stack::~Stack() {
  if (stack != NULL) {
      delete [] stack;
  }
}

void Stack::push(SymbolTable val) {
  if (tos >= size - 1) {
    cerr << "Stack overflow\n";
    exit(1);
  } else {
    stack[++tos] = val;
  }
}

SymbolTable Stack::pop() {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos--];
  }
}

SymbolTable Stack::peek() const {
  if (tos < 0) {
    cerr << "Peek on empty stack!\n";
    exit(1);
  } else {
    return stack[tos];
  }
}

int Stack::gettos() {
  return tos;
}
//ostream& operator <<(ostream& out, const Stack& s){ //This is essentially a new version of the insertion operator function.
//  out << "[ "; //operator<<(out, "[ ") //This doesn't call the function we are working on because of the second parameter being different
//  for (int i=0; i <= s.tos; i++) {
//    out << s.stack[i] << " ";
//  }
//  out << "]";  
//  return out; //Returning same ostream we passed in as a parameter.
//}

void Stack::checkIndex(int i) const {
  if (i < 0 || i > tos) {
        cerr << "Index out of bounds\n";
    exit(1);
  }  
}

SymbolTable& Stack::operator [](int i){//Overloading the array access operator.
  cout << "lvalue []\n";
  checkIndex(i);
  return stack[i];
}//Allows stack to operate like an array s1[0] returns first element of the stack.

const SymbolTable& Stack::operator [](int i) const {
  cout << "rvalue []\n";  
  checkIndex(i);  
  return stack[i];
}
//& means this is a reference
//* means this is a pointer
