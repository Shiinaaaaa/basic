/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() =default;

Program::~Program() {
    for(auto i : program_map){
        delete program_map[i].statement;
        program_map[i].statement = nullptr;
    }
   // Replace this stub with your own code
}

void Program::clear() {
    for(auto  i : program_map){
        delete program_map[i].statement;
        program_map[i].statement = nullptr;
    }
    program_map.clear();
   // Replace this stub with your own code
}

void Program::addSourceLine(int lineNumber, string line) {
    if(program_map.containsKey(lineNumber)){
        program_map[lineNumber].inf = line;
        program_map[lineNumber].statement = nullptr;
    }
    else{
        Line l;
        l.inf = line;
        l.statement = nullptr;
        program_map.put(lineNumber , l);
    }
   // Replace this stub with your own code
}

void Program::removeSourceLine(int lineNumber) {
    if(!program_map.containsKey(lineNumber)) return;
    delete program_map[lineNumber].statement;
    program_map.remove(lineNumber);
   // Replace this stub with your own code
}

string Program::getSourceLine(int lineNumber) {
    if(!program_map.containsKey(lineNumber)) return "";
    return program_map[lineNumber].inf;
    // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if(!program_map.containsKey(lineNumber)) error("SYNTAX ERROR");
    program_map[lineNumber].statement = stmt;
   // Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber) {
    if(!program_map.containsKey(lineNumber)) error("SYNTAX ERROR");
    return program_map[lineNumber].statement;
    // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
    if(program_map.isEmpty()) return -1;
    for(auto i : program_map){
        return i;
    }
    return 0;
    // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
    for(auto i : program_map){
        if(i > lineNumber) return i;
    }
    return -1;
    // Replace this stub with your own code
}

void Program::List(){
    for(auto i : program_map){
        cout<<program_map[i].inf<<endl;
    }
}

void Program::Run(EvalState &state){
    auto it = program_map.begin();
    while(it != program_map.end()){
        try{
            program_map[*it].statement->execute(state);
            it++;
        } catch (ErrorException &e) {
            TokenScanner t;
            if(t.getTokenType(e.getMessage()) == NUMBER){
                int number = stringToInteger(e.getMessage());
                if(program_map.containsKey(number)) {
                    auto i = program_map.begin();
                    while(i != program_map.end()){
                        if(*i == number) break;
                        else ++i;
                    }
                    it = i;
                    continue;
                }
                else error("LINE NUMBER ERROR");
            }
            else error(e.getMessage());
        }
    }
    /*for(auto i : program_map){
        try{
            program_map[i].statement->execute(state);
        }catch(ErrorException &e){
            TokenScanner t;
            if(t.getTokenType(e.getMessage()) == NUMBER){
                int number = stringToInteger(e.getMessage());
                if(program_map.containsKey(number)) {
                    i = number;
                }
                else error("LINE NUMBER ERROR");
            }
            else error(e.getMessage());
        }
    }*/
}