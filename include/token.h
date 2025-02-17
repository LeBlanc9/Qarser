#pragma once
#include <string>
#include <unordered_map>

namespace qarser {


enum class TokenType {
    OPENQASM,       // OPENQASM 2.0
    INCLUDE,        // include 

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
    IDENTIFIER,    // 标识符
    NUMBER,        // 数字
    
    // Symbol
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    SEMICOLON,     // ;
    ARROW,         // ->
    COMMA,         // ,
    
    // Special
    EOF_TOKEN,     // 文件结束
    ERROR
};


struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};







};