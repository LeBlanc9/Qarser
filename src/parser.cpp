#include "parser.h"
#include <iostream>


namespace qarser {
    
    // -- Public :
    Parser::Parser(const std::string& source) 
        : lexer(source) {
        advance();
    }

    std::unique_ptr<Program> Parser::parse() {
        auto program = std::make_unique<Program>();

        parse_version(program);

        while (!match(TokenType::EOF_TOKEN)) {
            program->statements.push_back(parse_statement());
        }

        return program;
    }


    // -- Private :
    void Parser::advance() {
        previous = current;
        current = lexer.next();
    }

    Token Parser::consume(TokenType type, const std::string& message) {
        if (current.type == type) {
            Token temp = current; 
            advance();
            return temp;
        }
        error(message);
    }


    bool Parser::match(TokenType type) {
        return type == current.type;
    }

    void Parser::error(const std::string& message) {
        throw ParsingError(current.line, current.column, message);
    }



    void Parser::parse_version(std::unique_ptr<Program>& program) {
        consume(TokenType::OPENQASM, "Need OPENQASM key word!");
        Token version = consume(TokenType::NUMBER, "Need Version number!");
        double version_num = std::stod(version.lexeme);
        if (version_num != 2.0) {
            throw std::runtime_error("Only support OPENQASM 2.0!");
        }
        program->version = version_num;
        consume(TokenType::SEMICOLON, "Need Semicolon!");
    } 


    std::unique_ptr<Statement> Parser::parse_statement() {
        if (match(TokenType::INCLUDE)) return parse_include();
        if (match(TokenType::QREG)) return parse_qreg();
        if (match(TokenType::CREG)) return parse_creg();
        if (match(TokenType::MEASURE)) return parse_measure();
        if (match(TokenType::BARRIER)) return parse_barrier();

        return parse_gate();
    }

    std::unique_ptr<Include> Parser::parse_include() {
        consume(TokenType::INCLUDE, "Need include key word!");
        Token filename = consume(TokenType::STRING, "Need filename!");
        consume(TokenType::SEMICOLON, "Need ';' !");

        return std::make_unique<Include>(filename.lexeme);
    }

    std::unique_ptr<QRegister> Parser::parse_qreg() {
        consume(TokenType::QREG, "Need qreg key word!");
        auto [name, size] = parse_array_declaration();
        consume(TokenType::SEMICOLON, "Need ';' !");
        return std::make_unique<QRegister>(QRegister{name, size});
    }

    std::unique_ptr<CRegister> Parser::parse_creg() {
        consume(TokenType::CREG, "Need creg key word!");
        auto [name, size] = parse_array_declaration();
        consume(TokenType::SEMICOLON, "Need ';' !");
        return std::make_unique<CRegister>(name, size);
    }

    // q[0];
    std::pair<std::string, int> Parser::parse_array_declaration() {
        Token name = consume(TokenType::IDENTIFIER, "Need register name!");
        consume(TokenType::LEFT_BRACKET, "Need '[' !");
        Token size = consume(TokenType::NUMBER, "Need register size!");
        consume(TokenType::RIGHT_BRACKET, "Need Right Bracket ']' !");

        return {name.lexeme, std::stoi(size.lexeme)};
    }

    std::string Parser::parse_qubit_ref() {
        Token reg = consume(TokenType::IDENTIFIER, "Need register name!");
        if (match(TokenType::LEFT_BRACKET)) {
            consume(TokenType::LEFT_BRACKET, "Need '[' !");
            Token index = consume(TokenType::NUMBER, "Need register size!");
            consume(TokenType::RIGHT_BRACKET, "Need ']'!");
            return reg.lexeme + "[" + index.lexeme + "]";
        }
        else {
            return reg.lexeme;
        }
    }

    std::vector<std::string> Parser::parse_qubit_ref_list() {
        std::vector<std::string> qubits;
        qubits.push_back(parse_qubit_ref());
        while (match(TokenType::COMMA)) {
            consume(TokenType::COMMA, "Need Comma!");
            qubits.push_back(parse_qubit_ref());
        }
        return qubits;
    }

    std::unique_ptr<Gate> Parser::parse_gate() {
        Token name = consume(TokenType::IDENTIFIER, "Need gate name!");
        std::vector<std::string> qubits = parse_qubit_ref_list();
        consume(TokenType::SEMICOLON, "Need ';'!");

        return std::make_unique<Gate>(name.lexeme, qubits);
    }

    std::unique_ptr<Measure> Parser::parse_measure() {
        consume(TokenType::MEASURE, "Need measure key word!");

        std::vector<std::string> qubits;
        qubits = parse_qubit_ref_list();

        Token arrow = consume(TokenType::ARROW, "Need right arrow '->' !");

        std::vector<std::string> cbits;
        cbits = parse_qubit_ref_list();

        consume(TokenType::SEMICOLON, "Need ';'!");

        return std::make_unique<Measure>(qubits, cbits);
    }


    std::unique_ptr<Barrier> Parser::parse_barrier() {
        consume(TokenType::BARRIER, "Need barrier key word!");
        std::vector<std::string> qubits = parse_qubit_ref_list();
        consume(TokenType::SEMICOLON, "Need ';'!");

        return std::make_unique<Barrier>(qubits);
    }

} // namespace qarser
