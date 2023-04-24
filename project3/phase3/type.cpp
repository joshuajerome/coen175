# include "type.h"
# include <cstdlib>

using namespace std;

Declarator(int kind, unsigned length, Types *types): _kind(kind), _length(length), _types(types) {
    assert(kind == POINTER || kind == FUNCTION || kind ==ARRAY);
}
unsigned Declarator::length() const{
    assert(kind == ARRAY);
    return length;
}

Types Declarator::*parameters() const{
    assert(kind == FUNCTION);
    return types;
}

bool Type::operator !=(const & Type that){
    return !operator==(that);
}
bool Type::operator==(const &Type that){
    return _specifier == that._specifier && _decls == that._decls;
}
bool Type::isArray(){
    return _decls.kind == ARRAY;
}
bool Type::isArray(){
    return _decls.kind == ARRAY;
}
bool Type::isArray(){
    return _decls.kind == ARRAY;
}