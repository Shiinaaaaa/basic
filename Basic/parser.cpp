/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      error("parseExp: Found extra token: " + scanner.nextToken());
   }
   return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses in expression");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}

Statement *parseStatement(TokenScanner &scanner , string line){
    string token;
    TokenType type;
    token = scanner.nextToken();
    type = scanner.getTokenType(token);
    if (type != WORD) error("SYNTAX ERROR");
    if (token == "REM") {
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new REM();
    }
    if (token == "LET") {
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        Expression *exp = parseExp(scanner);
        if (exp->getType() != COMPOUND) {
            delete exp;
            error("SYNTAX ERROR");
        }
        else {
            if (check(((IdentifierExp*)(((CompoundExp*)exp)->getLHS()))->getName())) error("SYNTAX ERROR");
            if ( ((CompoundExp*)exp)->getOp() != "=" ) {
                delete exp;
                error("SYNTAX ERROR");
            }
        }
        return new LET(exp);
    }
    if (token == "PRINT") {
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        Expression *exp = parseExp(scanner);
        return new PRINT(exp);
    }
    if (token == "INPUT") {
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        token = scanner.nextToken();
        type = scanner.getTokenType(token);
        if (type != WORD) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new INPUT(token);
    }
    if (token == "END") {
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new END();
    }
    if (token == "GOTO") {
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        token = scanner.nextToken();
        type = scanner.getTokenType(token);
        if (type != NUMBER) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        int linenumber = stringToInteger(token);
        return new GOTO(linenumber);
    }
    if (token == "IF") {
        Expression *left,*right;
        string cmp;
        THEN *then;
        if(!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        //token = scanner.nextToken();
        if(line.find('=') != string ::npos){
            cmp = "=";
            string l;//记录cpm左边的值
            scanner.setInput(line);
            token = scanner.nextToken(); token = scanner.nextToken();
            while(true){
                token = scanner.nextToken();
                if(token == "=") break;
                l += token;
            }
            right = readE(scanner);//right = parseExp(scanner);
            token = scanner.nextToken();
            if(token != "THEN") error("SYNTAX ERROR");
            if(!scanner.hasMoreTokens()) error("SYNTAX ERROR");
            token = scanner.nextToken();
            type = scanner.getTokenType(token);
            if(type != NUMBER) error("SYNTAX ERROR");
            then = new THEN(stringToInteger(token));
            scanner.setInput(l);
            left = readE(scanner);
            return new IF(left , right , cmp , then);
        }
        else{
            left = readE(scanner);
            token = scanner.nextToken();
            cmp = token;
            right = readE(scanner);
            token = scanner.nextToken();
            if(token != "THEN") {
                delete left; delete right;
                error("SYNTAX ERROR");
            }
            else {
                if(!scanner.hasMoreTokens()) {
                    delete left; delete right;
                    error("SYNTAX ERROR");
                }
                else {
                    token = scanner.nextToken();
                    type = scanner.getTokenType(token);
                    if(type != NUMBER) {
                        delete left; delete right;
                        error("SYNTAX ERROR");
                    }
                    else {
                        then = new THEN(stringToInteger(token));
                        return new IF(left , right , cmp , then);
                    }
                }
            }
        }
    }
    return nullptr;
}

bool check(const string &ss){
    if (ss == "REM" || ss == "INPUT" || ss == "LET" || ss == "PRINT" || ss == "END"
        || ss == "GOTO" || ss == "IF" || ss == "RUN" || ss == "LIST" || ss == "CLEAR"
        || ss == "QUIT" || ss == "HELP") return true;
    else return false;
}