/*
 * File:	Type.h
 *
 * Description:	This file contains the class definitions for types in
 *		Simple C.  A type consists of a specifier and a list of
 *		declarators.  A declarator is either a pointer declarator,
 *		an array declarator, or a function declarator.
 *
 *		By convention, a null parameter list represents an
 *		unspecified parameter list.  An empty parameter list is
 *		represented by an empty vector.
 *
 *		The enumeration for the kinds of declarators should
 *		probably be within the class, but we have constants for
 *		tokens floating around in the global namespace anyway.
 *
 *		To save space, we use a C++ forward list rather than a C++
 *		vector or a C++ list for the declarators.  That way, simple
 *		types without any declarators incur no wasted space, and
 *		types with only one or two declarators are still more
 *		efficient than using the other representations.
 *
 *		Subclassing is used to provide custom constructors that
 *		serve as factory functions.  However, all information is
 *		stored in the base class to avoid the problem to object
 *		slicing (since we'll be treating types as value types.)
 */

# ifndef TYPE_H
# define TYPE_H
# include <vector>
# include <forward_list>
# include <ostream>

enum { ARRAY, FUNCTION, POINTER };

typedef std::vector<class Type> Types;

class Declarator {
protected:
    int _kind;
    unsigned _length;
    Types *_parameters;

    Declarator(int kind, unsigned length = 0, Types *parameters = nullptr);

public:
    int kind() const;
    unsigned length() const;
    Types *parameters() const;

    bool operator ==(const Declarator &that) const;
    bool operator !=(const Declarator &that) const;
};

struct Pointer : public Declarator {
    Pointer() : Declarator(POINTER) {}
};

struct Array : public Declarator {
    Array(unsigned length) : Declarator(ARRAY, length) {}
};

struct Function : public Declarator {
    Function(Types *parameters) : Declarator(FUNCTION, 0, parameters) {}
};

std::ostream &operator <<(std::ostream &ostr, const Declarator &decl);


typedef std::forward_list<class Declarator> Declarators;

class Type {
    int _specifier;
    Declarators _decls;

public:
    Type(int specifier);
    Type(int specifier, const Declarators &declarators);

    int specifier() const;
    const Declarators &declarators() const;

    bool operator ==(const Type &that) const;
    bool operator !=(const Type &that) const;

    bool isArray() const;
    bool isPointer() const;
    bool isFunction() const;
    Types *parameters() const;

    Type promote() const;
    Type dereference() const;
    unsigned size() const;
};

std::ostream &operator <<(std::ostream &ostr, const Type &type);

# endif /* TYPE_H */
