# include <iostream>
# include "checker.h"

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

void checkIdentifier(const string &name) {}