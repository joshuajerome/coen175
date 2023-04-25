#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include <ostream>

enum {ARRAY, FUNCTION, POINTER};
typedef std::vector<class Type> Types;

class Declarator{
    int _kind;
    unsigned _length;
    Types *_parameters;
    public:
        Declarator(int kind, unsigned length = 0, Types *_parameters = nullptr);
        int kind() const {return _kind;}
        unsigned length() const;
        Types *parameters() const;

        bool operator==(const Declarator &that) const;
        bool operator!=(const Declarator &that) const;
};

std::ostream & operator<<(std::ostream & ostr, const Declarator &decl);
typedef std::vector<class Declarator> Declarators;

class Type {
    int _specifier;
    Declarators _decls;
    public:
        Type(int specifier): _specifier(specifier){}
        Type(int specifier, const Declarators &decls): _specifier(specifier), _decls(decls){}
        int specifier() const {return _specifier;}
        const Declarators &declarators() const {return _decls;}

    bool operator==(const Type &that) const;
    bool operator!=(const Type &that) const;

    bool isFunction() const;
    bool isArray() const;
    bool isPointer() const;


};
std::ostream & operator<<(std::ostream & ostr, const Type &type);
#endif
