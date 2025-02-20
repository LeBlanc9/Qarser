#pragma once
#include "lexer.h"
#include "ast.h"

namespace qarser {

class Parser {
public:
    Parser(const std::string& source);

    Program* parse();

private:
    QasmLexer lexer;
    Token current;
    Token previous;

};


}; // namespace qarser