# include "scope.h"

using namespace std;

void Scope::insert(Symbol* symbol) {
    _symbols.push_back(symbol);
}

void Scope::remove(const string &name) {
    for (int i = 0; i < int(_symbols.size()); i++) {
        if (_symbols[i]->name() == name) {
            _symbols.erase(_symbols.begin() + i);
            return;
        }
    }
}

Symbol* Scope::find(const string &name) const {
    for (auto s: _symbols)
        if (s->name() == name)
            return s;
    return nullptr;
}

Symbol* Scope::lookup(const string &name) const {
    Symbol* var = find(name);
    if (var != nullptr) return var; 
    if (_enclosing == nullptr) return nullptr;
    return _enclosing->lookup(name);
}
