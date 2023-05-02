# include <iostream>
# include <map>
# include "checker.h"
# include "scope.h"
# include "lexer.h"

using namespace std;

Scope* head = nullptr;

const string E1 = "redefinition of '%s'", 
             E2 = "conflicting types for '%s'", 
             E3 = "redeclaration of '%s'", 
             E4 = "'%s' undeclared";

map<string, Type> externs;

void openScope() {
    head = new Scope(head);
}

void closeScope() {
    head = head->enclosing();
}

void defineFunction(const string &name, const Type &type) {
    Scope* global = head;
    while (global->enclosing() != nullptr) {
        global = global->enclosing();
    }

    Symbol* symbol = global->find(name);
    if (symbol == nullptr) {

        if (externs.count(name) == 0) externs.insert(make_pair(name,type));
        else if (externs.at(name) != type) report(E2, name);
        
        global->insert(new Symbol(name, type));

    } 
    else {

        Type t = symbol->type();
        if (t.isFunction() && t.declarators()[0].parameters() != nullptr) report(E1, name);
        else if (t != type) report(E2, name);

        else global->insert(new Symbol(name, type));
    }
}

void declareSymbol(const string &name, const Type &type) {
    
    Symbol* symbol = head->find(name);

    if (symbol == nullptr) {
        if (type.isFunction() || head->enclosing() == nullptr) {

            if (externs.count(name) == 0) externs.insert(make_pair(name,type));
            else if (externs.at(name) != type) report(E2, name);

            head->insert(new Symbol(name, type));
        }
    } else {
        Type t = symbol->type();
        if (!(t.isFunction() && head->enclosing() == nullptr)) { // not (declaraing functino in global scope)
            report(E3, name);
        } else {
            if (externs.count(name) == 0) externs.insert(make_pair(name,t));
            else if (externs.at(name) != t) report(E2, name);
        }
    }
}

void checkIdentifier(const string &name) {
    if (head->lookup(name) == nullptr) report(E4, name); 
}