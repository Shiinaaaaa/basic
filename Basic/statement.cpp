/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() = default;
Statement::~Statement() = default;

REM::REM()= default;
REM::~REM()= default;
void REM::execute(EvalState &state) {}

LET::LET(Expression *exp){
    Exp = exp;
}
LET::~LET(){
    delete Exp;
}
void LET::execute(EvalState &state) {
    Exp->eval(state);
}

PRINT::PRINT(Expression *exp){
    Exp = exp;
}
PRINT::~PRINT(){
    delete Exp;
}
void PRINT::execute(EvalState &state) {
    cout << Exp->eval(state)<<endl;
}

INPUT::INPUT(string &n){
    var = n;
}
INPUT::~INPUT()= default;
void INPUT::execute(EvalState &state) {
    cout<<" ? ";
    TokenScanner scanner;
    string token;
    TokenType type;
    scanner.ignoreComments();
    while(true){
        scanner.setInput(getLine());
        token = scanner.nextToken();
        bool flag;
        if(token == "-") {
            token = scanner.nextToken();
            flag = true;//特判负数
        }
        type = scanner.getTokenType(token);
        if(type != NUMBER) {
            cout<<"INVALID NUMBER"<<endl;
            cout<<" ? ";
            continue;
        }
        if(scanner.hasMoreTokens()){
            cout<<"INVALID NUMBER"<<endl;
            cout<<" ? ";
            continue;
        }
        int value;
        try{
            if(!flag) value = stringToInteger(token);
            else value = -stringToInteger(token);
        }
        catch (...) {
            cout<<"INVALID NUMBER"<<endl;
            cout<<" ? ";
            continue;
        }
        state.setValue(var , value);
        break;
    }
}

END::END()=default;
END::~END()=default;
void END::execute(EvalState &state) {
    error("END");
}

GOTO::GOTO(int n){
    linenumber = n;
}
GOTO::~GOTO()=default;
void GOTO::execute(EvalState &state) {
    error(integerToString(linenumber));
}

THEN::THEN(int n){
    linenumber = n;
}
THEN::~THEN()=default;
void THEN::execute(EvalState &state) {
    error(integerToString(linenumber));
}

IF::IF(Expression *expl, Expression *expr, string &cmp, THEN *then):Expr(expr),Expl(expl),Cmp(cmp),Then(then) {}
IF::~IF(){
    delete Expl;
    delete Expr;
    delete Then;
}
void IF::execute(EvalState &state) {
    int left = Expl->eval(state);
    int right = Expr->eval(state);
    if(Cmp == "="){
        if(left == right) Then->execute(state);
        else return;
    }
    if(Cmp == "<"){
        if(left < right) Then->execute(state);
        else return;
    }
    if(Cmp == ">"){
        if(left > right) Then->execute(state);
        else return;
    }
}