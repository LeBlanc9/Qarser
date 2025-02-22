#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>

namespace qarser {


enum class TokenType {
    OPENQASM,       // OPENQASM 2.0
    INCLUDE,        // include 
    STRING,         // string

    QREG,          // qreg
    CREG,          // creg
    MEASURE,       // measure
    BARRIER,       // barrier
    RESET,         // reset

    // Gate
    U,             // U gate
    CX,            // CX gate
    GATE,          // gate

    // Flow Control
    IF,            // if
    
    // Math
    PI,            // pi
    SIN,           // sin
    COS,           // cos
    
    // Basic Type
    IDENTIFIER,    // Identifier
    NUMBER,        // number
    
    // Symbol
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    SEMICOLON,     // ;
    ARROW,         // ->
    COMMA,         // ,
    
    // Special
    EOF_TOKEN,     // file end
    ERROR
};


struct Token {
public:
    TokenType type;
    std::string lexeme;
    int line;
    int column;
public:
   void print() {
        std::cout << "Token{ type: " << std::setw(20) << std::left << to_string()
                  << "value: " << std::setw(20) << std::left << lexeme
                  << "line: " << std::setw(4) << std::left << line
                  << "column: " << std::setw(4) << std::left << column 
                  << "}" << std::endl;
    }

    std::string to_string() {
        switch (type) {
            case TokenType::OPENQASM: return "OPENQASM";
            case TokenType::INCLUDE: return "INCLUDE";
            case TokenType::QREG: return "QREG";
            case TokenType::CREG: return "CREG";
            case TokenType::MEASURE: return "MEASURE";
            case TokenType::BARRIER: return "BARRIER";
            case TokenType::RESET: return "RESET";
            case TokenType::U: return "U";
            case TokenType::CX: return "CX";
            case TokenType::GATE: return "GATE";
            case TokenType::IF: return "IF";
            case TokenType::PI: return "PI";
            case TokenType::SIN: return "SIN";
            case TokenType::COS: return "COS";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::NUMBER: return "NUMBER";
            case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
            case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
            case TokenType::LEFT_PAREN: return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenType::SEMICOLON: return "SEMICOLON";
            case TokenType::ARROW: return "ARROW";
            case TokenType::COMMA: return "COMMA";
            case TokenType::EOF_TOKEN: return "EOF";
            case TokenType::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
};



}; // namespace qarser