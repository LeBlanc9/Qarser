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
        if (!symbol_table.exists(gate.name)) {
            throw SemanticError(gate.line, "Gate " + gate.name + " not defined");
        }        

        int num_qubits = 0;
        for (const auto& qubit : gate.qubits) {
            num_qubits += checkRegisterRef(qubit, gate.line);
        }
        if (num_qubits != symbol_table.getAs<GateSymbol>(gate.name)->num_qubits) {
            throw SemanticError(gate.line, 
                "Gate " + gate.name + " expects " + 
                std::to_string(symbol_table.getAs<GateSymbol>(gate.name)->num_qubits) + 
                " qubits, but got " + std::to_string(num_qubits));
        }


        symbol_table.insert(
            gate.name,
            std::make_unique<GateSymbol>(
                gate.name,
                0,
                num_qubits
            )
        );
    }

private:
    int checkRegisterRef(const RegisterRef& ref, int line) {
        int num_qubits = 0;

        if (!symbol_table.exists(ref.name)) {
            throw SemanticError(line, "Register " + ref.name + " not defined");
        }

        const auto& reg_symbol = symbol_table.getAs<RegisterSymbol>(ref.name);
        if (ref.isRefWholeRegister()) {
            return reg_symbol->size;
        }
        else if (ref.index < reg_symbol->size){
            return 1;
        }
        else {
            throw SemanticError(line, 
                "Index " + std::to_string(ref.index) + 
                " out of range for register '" + ref.name + 
                "' of size " + std::to_string(reg_symbol->size));
        }
    }

};







}; // namespace qarser