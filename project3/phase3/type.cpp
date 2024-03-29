# include "type.h"
# include "tokens.h"
# include <cassert>
# include <string>
# include <ostream>

using namespace std;

Declarator::Declarator(int kind, unsigned length, Types *types):_kind(kind), _length(length), _parameters(types) {
    assert(kind == POINTER || kind == FUNCTION || kind == ARRAY);
}
unsigned Declarator::length() const{
    assert(_kind == ARRAY);
    return _length;
}

Types* Declarator::parameters() const{
    assert(_kind == FUNCTION);
    return _parameters;
}

bool Type::operator !=(const Type& that) const {
    return !operator==(that);
}
bool Type::operator==(const Type& that) const { 
    return _specifier == that._specifier && _decls == that._decls;
}

bool Declarator::operator!=(const Declarator &that) const{
    return !operator==(that);
}
bool Declarator::operator==(const Declarator &that)const{
    if (_kind != that._kind) {
        return false;
    } 
    if (_kind == POINTER) {
        return true;
    }
    if (_kind == ARRAY) {
        return _length == that._length;
    } 
    assert(_kind == FUNCTION);
    if (!_parameters || !that._parameters) {
        return true;
    }
    return *_parameters == *that._parameters;
}

bool Type::isFunction() const {
    if (_decls.size() <= 0) return false;
    return _decls[0].kind() == FUNCTION;
}

bool Type::isArray() const {
    if (_decls.size() <= 0) return false;
    return _decls[0].kind() == ARRAY;
}

bool Type::isPointer() const {
    if (_decls.size() <= 0) return false;
    return _decls[0].kind() == POINTER;
}

ostream & operator<<(std::ostream & ostr, const Declarator &decl) {
    switch (decl.kind()) {
        case ARRAY:
            ostr << "array of " << decl.length() << " ";
            break;
        case FUNCTION:
            ostr << "function returning ";
            break;
        case POINTER:
            ostr << "pointer to ";
            break;
    }
    return ostr;
}

ostream & operator<<(std::ostream & ostr, const Type &type) {
    int len = type.declarators().size();
    for (int i = 0; i < len; i++) {
        ostr << type.declarators()[i];
    }
    ostr << (type.specifier() == INT? "int" : "char");
    return ostr;
}