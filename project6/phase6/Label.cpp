# include <cstdlib>
# include <iostream>
# include "Label.h"

using namespace std;

Label::Label() {
    _number = _counter ++;
}

unsigned Label::number() const {
    return _number;
}

ostream &operator <<(ostream &ostr, const Label &label) {
    return ostr << ".L" << label.number();
}

