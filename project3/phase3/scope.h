# ifndef SCOPE_H
# define SCOPE_H
# include "symbol.h"
# include <vector>
# include <string>

typedef std::vector<Symbol*> Symbols;

class Scope {

    typedef std::string string;

    Scope *_enclosing;
    Symbols _symbols;
    public:
        Scope(Scope *enclosing = nullptr): _enclosing(enclosing) {}
        Scope* enclosing() const {return _enclosing;}
        const Symbols & symbols() const {return _symbols;}

        void insert(Symbol* symbol);
        void remove(const string &name);
        Symbol* find(const string &name) const;
        Symbol* lookup(const string &name) const;
};

# endif