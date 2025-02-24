#pragma once
#include <stdexcept>
#include "ast_visitor.h"
#include "symbol.h"

namespace qarser {

class SemanticError : public std::runtime_error {
public:
    SemanticError(int line ,const std::string& message) 
        : std::runtime_error(
            "Semantic Error at line " + 
            std::to_string(line) + 
            ": "+ message) {}
};


class SemanticAnalyzer : public AstVisitor {
private:
    bool use_qelib2 = false;
    SymbolTable symbol_table;

public:
    SemanticAnalyzer() = default;
    ~SemanticAnalyzer() = default;

public:
    void visit(Program& program) override {
        for (const auto& statement : program.statements) {
            statement->accept(*this);
        }
    }

    void visit(Include& include) override {
        const std::string& filename = include.filename;
        if (filename == "qelib2.inc") {
            use_qelib2 = true;
        }
    }

    void visit(QRegister& qreg) override {
        if (qreg.size <= 0) {
            throw SemanticError(qreg.line, "QRegister " + qreg.name + " must have a size greater than 0");
        }
        symbol_table.insert(
            qreg.name, 
            std::make_unique<RegisterSymbol>(
                SymbolType::QREG,
                qreg.name,
                qreg.size
            )
        );
    }

    void visit(CRegister& creg) override {
        if (creg.size <= 0) {
            throw SemanticError(creg.line, "CRegister " + creg.name + " must have a size greater than 0");
        }
        symbol_table.insert(
            creg.name, 
            std::make_unique<RegisterSymbol>(
                SymbolType::CREG,
                creg.name,
                creg.size
            )
        ); 
    }

    void visit(Gate& gate) override {
        if (symbol_table.exists(gate.name)) {
            throw SemanticError(gate.line, "Gate " + gate.name + " already defined");
        }
    }



};







}; // namespace qarser