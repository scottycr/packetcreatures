// Ignore parsing for now, get networking to work.
#ifndef DATAPARSER_H_
#define DATAPARSER_H_

#include <fstream>
#include <string>

#include "creature.h"

namespace dataparser {

enum fileflag_t {
    TEXT,
    BIN
};

// TODO:
// Have a CLI flag that allows user to override default path
std::string filePath = "../data/";

class Parser {
private:
    std::string textFile, binFile;
    std::fstream fs;
    fileflag_t flag;  
public:
    Parser(
        std::string _textFile, 
        std::string _binFile,
        fileflag_t _flag
    );
    void setFlag(fileflag_t newFlag) { flag = newFlag; }
    bool read();
    bool dump();
    ~Parser();
};

}

#endif 