/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   //cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
   /*Expression *exp = parseExp(scanner);
   int value = exp->eval(state);
   cout << value << endl;
   delete exp;*/
   string token;
   TokenType type;
   if (!scanner.hasMoreTokens()) return;
   token = scanner.nextToken();
   type = scanner.getTokenType(token);
   if (type == WORD || type == NUMBER){
       if(type == WORD){
           if (token == "RUN") {
               if(scanner.hasMoreTokens()) {
                   cout<<"SYNTAX ERROR"<<endl;
                   return;
               }
               try{
                   program.Run(state);
               }catch(ErrorException &error){
                   if (error.getMessage() == "INVALID NUMBER") cout<<"INVALID NUMBER"<<endl;
                   if (error.getMessage() == "END") return;
                   if (error.getMessage() == "DIVIDE BY ZERO") cout<<"DIVIDE BY ZERO"<<endl;
                   if (error.getMessage() == "LINE NUMBER ERROR") cout<<"LINE NUMBER ERROR"<<endl;
                   else cout<<"VARIABLE NOT DEFINED"<<endl;
                   return;
               }
           }
           if (token == "LIST") {
               if (scanner.hasMoreTokens()) {
                   cout<<"SYNTAX ERROR"<<endl;
                   return;
               }
               program.List();
               return;
           }
           if (token == "CLEAR") {
               program.clear();
               state.clear();
               return;
           }
           if (token == "QUIT") {
               if (scanner.hasMoreTokens()) cout<<"SYNTAX ERROR"<<endl;
               exit(0);
           }
           if (token == "HELP") {
               if (scanner.hasMoreTokens()) cout<<"SYNTAX ERROR"<<endl;
               cout << "RUN : This command starts program execution beginning at the lowest-numbered line"<<endl;
               cout << "LIST : This command lists the steps in the program in numerical sequence."<<endl;
               cout << "CLEAR : This command deletes all program and variables."<<endl;
               cout << "QUIT : This command exits from the BASIC interpreter by calling exit(0)."<<endl;
           }
           if (token == "LET" || token == "PRINT" || token == "INPUT") {
               scanner.setInput(line);
               Statement *statement;
               if (!scanner.hasMoreTokens()) {
                   cout<<"SYNTAX ERROR"<<endl;
                   return;
               }
               try{
                   statement = parseStatement(scanner , line);
               }
               catch(...){
                   cout<<"SYNTAX ERROR"<<endl;
                   return;
               }
               try{
                   statement->execute(state);
               }
               catch (ErrorException &error) {
                   if (error.getMessage() == "DIVIDE BY ZERO") cout<<"DIVIDE BY ZERO"<<endl;
                   else cout<<"VARIABLE NOT DEFINED"<<endl;
                   delete statement;
                   return;
               }
               delete statement;
           }
       }
       if (type == NUMBER){
           int linenumber = stringToInteger(token);
           if (!scanner.hasMoreTokens()) {
               program.removeSourceLine(linenumber);
               return;
           }
           try{
               Statement *statement = parseStatement(scanner , line);
               program.addSourceLine(linenumber , line);
               program.setParsedStatement(linenumber , statement);
           }
           catch(...){
               cout<<"SYNTAX ERROR"<<endl;
               return;
           }
       }
   }
   else error("SYNTAX ERROR");
}
