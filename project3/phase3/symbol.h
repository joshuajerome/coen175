#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include "type.h"

class Symbol {

    typedef std::string string;

    string _name;
    Type* _type;
    public:
        Symbol(string name, Type* type): _name(name), _type(type){}
        string name() {return _name;}
        Type* type() {return _type;}
};

#endif