# include "scope.h"

using namespace std;

Symbol* Scope::find(const string &name) const{
    for (auto s: _symbols)
        if (s->name() == name)
            return s;
    return nullptr;
}