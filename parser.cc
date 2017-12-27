//George Cooper

//gcc219                                                                                                                                                                                          
//cse109    

//Parser file
#include "parser.h"
#include <cstring>
#include <iostream>

using namespace std;

string varDefsArray[1000];

Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), scope(), lindex(1), tindex(1) {
  token = lexer.nextToken();
  scope =  scope.enterScope();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token.getLexeme() << " at line " << token.getLine() << " position "  << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
}



Parser::TreeNode* Parser::factor() {
  TreeNode* ret;
  TreeNode* right;
  string identOrFuncLexeme;
  string identLexeme;
  if (token.getType() == Token::LPAREN)
    {
      token = lexer.nextToken();
      ret = expression();
      check(Token::RPAREN, "Expecting )");
    }
  else if (token.getType() == Token::INTLIT)
    {
      ret = new TreeNode(PUSHL, token.getLexeme());
    }
  else if (token.getType() == Token::IDENT)
    {
      identOrFuncLexeme = token.getLexeme();
      token = lexer.nextToken();
      if (token.getType() == Token::LPAREN)
	{
	  if (token.getType() != Token::RPAREN)
            {
              token = lexer.nextToken();
              ret = expression();
	      while (token.getType() != Token::RPAREN)
		{
		  token = lexer.nextToken();
		  right = expression();
		  ret = new TreeNode(SEQ, ret, right);
		}
	      right = new TreeNode(CALL, identOrFuncLexeme);
	      ret = new TreeNode(SEQ, ret, right);
	    }
	  else
	    {
	      ret = new TreeNode(CALL, identOrFuncLexeme);
	    }
	}
      else
	{
	  identLexeme = scope.getUniqueSymbol(identOrFuncLexeme);
	  ret = new TreeNode(PUSHV, identLexeme);
	  return ret;
	}   
    }
  else
    {
      ret = new TreeNode(SEQ);
    }
  token = lexer.nextToken();
  return ret;
}

Parser::TreeNode* Parser::term() {
  TreeNode* right;
  TreeNode* ret;
  Operation op;
  ret = factor();
  while (token.getType() == Token::TIMES || token.getType() == Token::DIVIDE)
    {
      if (token.getType() == Token::TIMES)
	{
	  op = MULT; 
	}
      else     
	{
	  op = DIV;
	}
      token = lexer.nextToken();
      right = factor();
      ret = new TreeNode(op, ret, right);
    }
  return ret;

}

Parser::TreeNode* Parser::expression() {
  TreeNode* right;
  TreeNode* ret;
  Operation op;
  ret = term();
  while (token.getType() == Token::PLUS || token.getType() == Token::MINUS)
    {
      if (token.getType() == Token::PLUS)
	{
	  op = ADD;
	}
      else
	{
	  op = SUB;
	}
      token = lexer.nextToken();
      right = term();
      ret = new TreeNode(op, ret, right);
      }
  return ret;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* ret;
  TreeNode* right;
  Operation op;
  ret = expression();
  switch (token.getType())
    {
    case Token::EQ:
      {
	op = ISEQ;
	break;
      }
    case Token::LT:
      { 
	op = ISLT;
	break;
      }
    case Token::LE:
      { 
	op = ISLE;
	break;
      }
    case Token::GT:
      { 
	op = ISGT;
	break;
      }
    case Token::GE:
      { 
	op = ISGE;
	break;
      }
    case Token::NE:
      { 
	op = ISNE;
	break;
      }
    default:
      {
	return ret;
      }
    }
  token = lexer.nextToken();
  right = expression();
  ret =  new TreeNode(op, ret, right);
  return ret;
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* ret;
  TreeNode* right;
  Operation op;

  ret = relationalExpression();
  while (token.getType() == Token::AND || token.getType() == Token::OR)
    {
      if (token.getType() == Token::AND)
        {
          op = AND;
        }
      else
        {
          op = OR;
        }
      token = lexer.nextToken();
      right = relationalExpression();
      ret = new TreeNode(op, ret, right);
    }
  return ret;

}



Parser::TreeNode* Parser::returnStatement() {
  TreeNode* ret;
  TreeNode* right;
  check(Token::RETURN, "Expecting Return");
  token = lexer.nextToken();
  ret = logicalExpression();
  right = new TreeNode(RET);
  ret =  new TreeNode(SEQ, ret, right);
  check(Token::SEMICOLON, "Expecting Semicolon");
  token = lexer.nextToken();
  return ret;
  }

Parser::TreeNode* Parser::printfStatement() {
  TreeNode* paramList = NULL;
  int nparams = 0;
  check(Token::PRINTF, "Expecting printf");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  check(Token::STRINGLIT, "Expecting string literal");
  string formatString = token.getLexeme();
  token = lexer.nextToken();
  if (token.getType() == Token::COMMA) {
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while (token.getType() == Token::COMMA) {
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
 TreeNode* printStatement =
   new TreeNode(SEQ, paramList, new TreeNode(PRINTF, itos(nparams) + formatString));
 return printStatement;
}


Parser::TreeNode* Parser::whileStatement() {
  TreeNode* ret;
  TreeNode* right;
  string L1 = makeLabel();
  string L2 = makeLabel();
  check(Token::WHILE, "Expecting 'While'");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting '('");
  token = lexer.nextToken();
  ret = new TreeNode(LABEL, L1);
  right = logicalExpression();
  ret = new TreeNode(SEQ, ret, right);
  right = new TreeNode(JUMPF, L2);
  ret = new TreeNode(SEQ, ret, right);
  check(Token::RPAREN, "Expecting ')'");
  token = lexer.nextToken();
  scope.enterScope();
  right = block();
  scope.exitScope();
  ret = new TreeNode(SEQ, ret, right);
  right = new TreeNode(JUMP, L1);
  ret = new TreeNode(SEQ, ret, right);
  right = new TreeNode(LABEL, L2);
  ret = new TreeNode(SEQ, ret, right);
  return ret;
}

Parser::TreeNode* Parser::ifStatement() {
  TreeNode* ret;
  TreeNode* right;
  string Label1 = makeLabel();
  string Label2 = makeLabel();
  
  check(Token::IF, "Expecting 'If'");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting '(");
  token = lexer.nextToken();
  ret = logicalExpression();
  check(Token::RPAREN, "Expecting ')");
  token = lexer.nextToken();
  right = new TreeNode(JUMPF, Label1);
  ret = new TreeNode(SEQ, ret, right);
  scope.enterScope();
  right = block();
  scope.exitScope();
  ret = new TreeNode(SEQ, ret, right);
  right = new TreeNode(JUMP, Label2);
  ret = new TreeNode(SEQ, ret, right);
  right = new TreeNode(LABEL, Label1);
  ret = new TreeNode(SEQ, ret, right);
  if (token.getType() == Token::ELSE)
    {
      token = lexer.nextToken();
      scope.enterScope();
      right = block();
      scope.exitScope();
      ret = new TreeNode(SEQ, ret, right);
    }
  right = new TreeNode(LABEL, Label2);
  ret = new TreeNode(SEQ, ret, right);
  return ret;
}

Parser::TreeNode* Parser::assignStatement() {
  TreeNode* ret;
  string toStore;
  string lexemeInput;
  TreeNode* right; 
  check(Token::IDENT, "Expecting Identifier");
  lexemeInput = token.getLexeme();
  toStore = scope.getUniqueSymbol(lexemeInput);
  if (toStore.compare("") == 0)
    {
      error("Variable not identified");
    }
  token = lexer.nextToken();
  check(Token::ASSIGN, "Expecting =");
  token = lexer.nextToken();
  right = new TreeNode(STORE, toStore);
  ret = logicalExpression();
  ret = new TreeNode(SEQ, ret, right);
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  return ret;

}

void  Parser::vardefStatement() {
  string varIdent;
  check(Token::VAR, "Expecting Var");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting Identifier");
  varIdent = token.getLexeme();
  scope.addSymbol(varIdent);
  token = lexer.nextToken();
  while (token.getType() == Token::COMMA)
    {
      token = lexer.nextToken();
      varIdent = token.getLexeme();
      scope.addSymbol(varIdent);
      token = lexer.nextToken();
    }
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  }




Parser::TreeNode* Parser::statement() {
  TreeNode* ret;
  switch (token.getType())
    {
    case Token::IDENT:
      {
	ret = assignStatement();
	break;
      }
    case Token::WHILE:
      {
	ret = whileStatement();
	break;
      }
    case Token::IF:
      {
	ret = ifStatement();
	break;
      }
    case Token::VAR:
      {
	vardefStatement();
	ret = new TreeNode(SEQ);
	break;
      }
    case Token::RETURN:
      {
	ret = returnStatement();
	break;
      }
    case Token::PRINTF:
      {
	ret = printfStatement();
	break;
      }
    default:
      {
	error("Expecting 'if', 'while', 'var', return' or  an identifier");
      }
    }
      return ret;
}




Parser::TreeNode* Parser::block() {
  TreeNode* ret;
  TreeNode* right;
  int type;
  check(Token::LBRACE, "Expecting '{'");
  token = lexer.nextToken();
  type = token.getType();
  if (type == Token::IDENT || type == Token::WHILE || type == Token::IF || type == Token::VAR || type == Token::RETURN || type == Token::PRINTF)
    {
      ret = statement();
      type = token.getType();
    }
  while (type == Token::IDENT || type == Token::WHILE || type == Token::IF || type == Token::VAR || type == Token::RETURN || type == Token::PRINTF)
    {
      right = statement();
      ret = new TreeNode(SEQ, ret, right);
      type = token.getType();
    }
  check(Token::RBRACE, "Expecting '}'");
  token = lexer.nextToken();
  return ret;
}

Parser::TreeNode* Parser::function() {
  TreeNode* ret;
  TreeNode* right;
  string funcName;
  string paramName;
  string uniquePName;
  check(Token::FUNCTION, "Expecting \"Function\"");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting Identifier");
  funcName = token.getLexeme();
  ret = new TreeNode(FUNC, funcName);
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting '('");
  token = lexer.nextToken();
  scope.enterScope();
  while (token.getType() != Token::RPAREN)
    {
      paramName = token.getLexeme();
      scope.addSymbol(paramName);
      uniquePName = scope.getUniqueSymbol(paramName);
      right = new TreeNode(PARAM, uniquePName);
      ret = new TreeNode(SEQ, ret, right);
      token = lexer.nextToken();
      if (token.getType() == Token::COMMA)
	{
	  token = lexer.nextToken();
	}
    }
  check(Token::RPAREN, "Expecting ')'");
  token = lexer.nextToken();
  right = block();
  scope.exitScope();
  ret = new TreeNode(SEQ, ret, right);
  return ret;
}




Parser::TreeNode* Parser::compilationunit() {
  TreeNode* ret;
  TreeNode* right; 
  ret = function();
  while (token.getType() == Token::FUNCTION)
    {
      right = function();
      ret = new TreeNode(SEQ, ret, right);
    }
  return ret;
}

void Parser::emit (string message) {
  cout << message << endl;
}
void Parser::handleAddSubAndOr (Operation oper) {
  emit (" pop rbx");
  emit (" pop rax");
  switch (oper)
    {
    case ADD:
      {
	emit (" add rax,rbx");
	break;
      }
    case SUB:
      { 
	emit (" sub rax,rbx");
	break;
      }
    case AND:
      { 
	emit (" and rax,rbx");
	break;
      }
    case OR:
      { 
	emit (" or rax,rbx");
	break;
      }
    default:
      {
	break;
      }
    }
      emit (" push rax");
}

void Parser::handleEquality (Operation oper) {
  string j1 = makeLabel();
  string j2 = makeLabel();
  emit (" pop rbx");
  emit (" pop rax");
  emit (" cmp rax,rbx");
  
  switch (oper)
    {
    case ISEQ:
      {
        emit (" je " + j1);
        break;
      }
    case ISNE:
      {
        emit (" jne " + j1);
        break;
      }
    case ISLT:
      {
        emit (" jl " + j1);
        break;
      }
    case ISLE:
      {
        emit (" jle " + j1);
        break;
      }
    case ISGT:
      {
        emit (" jg " + j1);
        break;
      }
    case ISGE:
      {
        emit (" jge " + j1);
        break;
      }
    default:
      {
        break;
      }
    }
      emit (" mov rax,0");
      emit (" jmp " + j2);
      emit (j1 + ":");
      emit (" mov rax, 1");
      emit (j2 + ":");
      emit (" push rax");
}
string currentFunc;
int nfmts = 0;
string fmts[100];
void Parser::geninst (TreeNode *Node) {
  if (Node == NULL)
    {
      return;
    }
  geninst(Node->leftChild);
  geninst(Node->rightChild);
  Operation currentOp = Node->op;
  switch (currentOp)
    {
    case (ADD):
    case (SUB):
    case (AND):
    case (OR):
      {
	handleAddSubAndOr(currentOp);
	break;
      }
    case (MULT):
      {
	emit(" pop rbx");
	emit(" pop rax");
	emit(" imul rbx");
	emit(" push rax");
	break;
      }
    case (DIV):
      {
	emit(" mov rdx,0");
	emit(" pop rbx");
	emit(" pop rax");
	emit(" idiv rbx");
	emit(" push rax");
	break;
      }
    case (LABEL):
      {
	emit(Node->val + ":");
	break;
      }
    case (ISEQ):
    case (ISNE):
    case (ISLT):
    case (ISLE):
    case (ISGT):
    case (ISGE):
      {
	handleEquality(currentOp);
	break;
      }
    case (JUMP):
      {
	emit(" jmp " + Node->val);
	break;
      }
    case (JUMPF):
      {
	emit(" pop rax");
	emit(" cmp rax, 0");
	emit(" je " + Node->val);
	break;
      }
    case (JUMPT):
      {
	emit(" pop rax");
	emit(" cmp rax, 0");
	emit(" jne " + Node->val);
	break;
      }
    case (PUSHV):
      {
	emit(" push qword[" + Node->val + "]");
	break;
      }
    case (PUSHL):
      {
	emit(" mov rax," + Node->val);
	emit(" push rax");
	break;
      }
    case (STORE):
    case (PARAM):
      {
	emit(" pop qword[" + Node->val + "]");
	break;
      }
    case (PRINTF):
      {
	string fmt = Node->val;
	int nparams = fmt.at(0) - '0';
	fmt = "`" + fmt.substr(2,(fmt.size() - 3)) + "`";
      	/*For some reason fmt.substr(1) was returning a string with 2 sets of double quotes.  This made
	  the printf print something like this "The cat ran" rather than just The cat ran.
	  I'm unsure why this happens, but this is the change I needed to make to make my output match
	  the expected output. It must be an issue with how I am processing String literals in my lexer,
	  but I have reviewed that and can't find the issue there either. */
	fmts[nfmts++] = fmt;
	emit(" mov rdi,fmt" + itos(nfmts));
	if (nparams == 5) {
	  emit(" pop r9");
	  --nparams;
	}
	if (nparams == 4) {
	  emit(" pop r8");
	  --nparams;
	}
	if (nparams == 3) {
	  emit(" pop rcx");
	  --nparams;
	}
	if (nparams == 2) {
	  emit(" pop rdx");
	  --nparams;
	}
	if (nparams == 1) {
	  emit(" pop rsi");
	}
	emit(" mov rax,0");
	emit(" push rbp");
	emit(" call printf");
	emit(" pop rbp");
	break;
      }
    case (CALL):
      {
	emit (" call " + Node->val);
	emit (" push rax");
	break;
      }
    case (FUNC):
      {
	currentFunc = Node->val;
	emit(currentFunc + ":");
	if (currentFunc != "main")
	  emit(" pop r15");
	break;
      }
    case (RET):
      {
	emit(" pop rax");
	if (currentFunc != "main")
	  emit(" push r15");
	emit(" ret");
	break;
      }
    default:
      {
	break;
      }
      
    }
      
}
void Parser::vardefs(TreeNode *Node) {
  if (Node == NULL)
    {
      return;
    }
  vardefs(Node->leftChild);
  vardefs(Node->rightChild);
  Operation currentOp = Node->op;
  string varName;
  if (currentOp == PUSHV || currentOp == STORE)
    {
      varName = Node->val;
      for (int i = 0; i < 1000; i++)
	{
	  if (varDefsArray[i] == varName)
	    {
	      break;
	    }
	  if (varDefsArray[i] == "")
	    {
	      varDefsArray[i] = varName;
	      break;
	    }
	}
    }
}
void Parser::genasm(TreeNode *Node) {
  cout  << "  global main" << endl;
  cout  << "  extern printf" << endl;
  cout << endl << "  segment .bss" << endl;
  vardefs(Node);
  for (int i = 0; i < 1000; i++)
    {
      if (varDefsArray[i] == "")
	{
	  break;
	}
      else
	{
	  emit("  " + varDefsArray[i] + " resq 1");
	}
    }

  cout << endl << "  section .text" << endl;
  geninst(Node);
  cout << endl << "  section .data" << endl;
  for (int i=0; i < nfmts; ++i) {
    cout << "  fmt" << i+1 << ": db " << fmts[i] << ", 0" << endl;

  }
}
