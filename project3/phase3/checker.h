# ifndef CHECKER_h
# define CHECKER_h
# include <iostream>
# include "type.h"

using namespace std;

void openScope() {
    cout << "open scope" << endl;
}

void closeScope() {
    cout << "close scope" << endl;
}

void defineFunction(const string &name, const Type &type) {
    cout << "declare " << name << " as " << type << endl;
}

void declareSymbol(const string &name, const Type &type) {
    cout << "declare " << name << " as " << type << endl;
}

void checkIdentifier(const string &name);

# endif