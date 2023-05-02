#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include "type.h"

class Symbol {

    typedef std::string string;
    string _name;
    Type _type;
    
    public:
        Symbol(string name, Type type): _name(name), _type(type){}
        const string &name() const {return _name;}
        const Type &type() const {return _type;}
};

#endif