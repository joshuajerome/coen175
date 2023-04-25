# include "type.h"
# include <cassert>

using namespace std;

Declarator::Declarator(int kind, unsigned length = 0, Types *types): _kind(kind), _length(length), _parameters(types) {
    assert(kind == POINTER || kind == FUNCTION || kind ==ARRAY);
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

// ostream & operator<<(std::ostream & ostr, const Declarator &decl) {}

bool Type::isFunction() const {
    assert(_decls.size() > 0);
    return _decls[0].kind() == FUNCTION;
}

bool Type::isArray() const {
    assert(_decls.size() > 0);
    return _decls[0].kind() == ARRAY;
}

bool Type::isPointer() const {
    assert(_decls.size() > 0);
    return _decls[0].kind() == POINTER;
}