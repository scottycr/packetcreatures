// Ignore parsing for now, get networking to work.
#include <iostream>
#include <sstream>

#include "creature.h"
#include "dataparser.h"

using namespace std;
using namespace creature;
using namespace dataparser;

Parser::Parser(
    string _textFile,
    string _binFile, 
    fileflag_t _flag
) : flag(_flag)
{
    textFile = filePath + _textFile;
    binFile = filePath + _binFile;
}

bool Parser::read() {
    if (flag == TEXT) {
        fs.open(textFile);
        stringstream sstr;
        sstr << fs.rdbuf();
        fs.close();

        
    } else if (flag == BIN) {
        
    }
    return false;
}

bool Parser::dump() {
    if (flag == TEXT) {

    } else if (flag == BIN) {
        
    }
    return false;
}
