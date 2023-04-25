# include "type.h"
# include "tokens.h"
# include <cassert>
# include <string>
# include <ostream>

using namespace std;

Declarator::Declarator(int kind, unsigned length, Types *types):_kind(kind), _length(length), _parameters(types) {
    assert(kind == POINTER || kind == FUNCTION || kind == ARRAY || kind == -1);
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
    if(_kind != that._kind) {
        return false;
    } else if(_kind == ARRAY) {
        return _length == that._length;
    }
    if(_kind == FUNCTION) {
        if(!_parameters || !that._parameters){
            return true;
        }
        return _parameters ==that._parameters;
    }
    return true;
}

bool Type::isFunction() const {
    assert(_decls.size() > 0);
    return _decls[_decls.size() - 1].kind() == FUNCTION;
}

bool Type::isArray() const {
    assert(_decls.size() > 0);
    return _decls[_decls.size() - 1].kind() == ARRAY;
}

bool Type::isPointer() const {
    assert(_decls.size() > 0);
    return _decls[_decls.size() - 1].kind() == POINTER;
}

ostream & operator<<(std::ostream & ostr, const Declarator &decl) {
    ostr << decl.kind();
    return ostr;
}

ostream & operator<<(std::ostream & ostr, const Type &type) {
    ostr << "([";
    int len = type.declarators().size();
    for (int i = 0; i < len; i++) {
        if (type.isArray()) {
            ostr << "array";
        } else if (type.isFunction()) {
            ostr << "function";
        } else if (type.isPointer()) {
            ostr << "pointer";
        } else {
            ostr << "";
        }

        if (i == len - 1) {
            ostr << "], ";
        } else {
            ostr << ", ";
        }
    }
    ostr << (type.specifier() == INT? "int" : "char") << ")" << endl;
    return ostr;
}