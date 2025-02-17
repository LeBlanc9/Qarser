#pragma once
#include <string>
#include "token.h"

namespace qarser {


class QasmLexer {
public:
    QasmLexer(const std::string& source);
    Token next();

private:
    std::string source;
    int position = 0;
    int line = 1;
    int column = 1;

    char advance();
    char peek() const;
    bool is_at_end() const;
    Token make_token(TokenType type);
    void skip_whitespace();

public:
    static const std::unordered_map<std::string, TokenType> keywords;

};

 
}; // namespace qarser
