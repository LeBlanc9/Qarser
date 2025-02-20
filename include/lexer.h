#pragma once
#include <string>
#include "token.h"

namespace qarser {


class QasmLexer {
public:
    QasmLexer(const std::string& source);
    bool is_at_end() const;
    Token next();

private:
    std::string source;
    int position = 0;
    int line = 1;
    int column = 1;

    char advance();
    char peek() const;
    Token make_token(TokenType type);
    void skip_whitespace();
    void skip_line();

public:
    static const std::unordered_map<std::string, TokenType> keywords;

};

 
}; // namespace qarser
