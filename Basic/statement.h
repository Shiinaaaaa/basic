/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class REM: public Statement{
public:
    REM();
    ~REM() override;
    void execute(EvalState & state) override;
};

class LET: public Statement{
public:
    explicit LET(Expression *exp);
    ~LET() override;
    void execute(EvalState & state) override;
private:
    Expression *Exp;
};

class PRINT: public Statement{
public:
    explicit PRINT(Expression *exp);
    ~PRINT() override;
    void execute(EvalState & state) override;
private:
    Expression *Exp;
};

class INPUT: public Statement{
public:
    explicit INPUT(string &n);
    ~INPUT() override;
    void execute(EvalState & state) override;
private:
    string var;
};

class END: public Statement{
public:
    END();
    ~END() override;
    void execute(EvalState & state) override;
};

class GOTO: public Statement{
public:
    explicit GOTO(int n);
    ~GOTO() override;
    void execute(EvalState & state) override;
private:
    int linenumber;
};

class THEN: public Statement{
public:
    explicit THEN(int n);
    ~THEN() override;
    void execute(EvalState & state) override;
private:
    int linenumber;
};

class IF: public Statement{
public:
    IF(Expression *expl, Expression *expr, string &cmp, THEN *then);
    ~IF() override;
    void execute(EvalState & state) override;
private:
    Expression *Expl , *Expr;
    string Cmp;
    THEN *Then;
};


#endif
