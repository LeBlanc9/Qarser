#pragma once
#include <iostream>
#include "ast.h"

namespace qarser {

class AstPrinter: public AstVisitor {
public:
    int indent = 1;

private:
    void print_indent() {
        for (int i = 0; i < indent; i++) {
            std::cout << "  ";
        }
    }

public:
    void visit(Program& program) override {
        std::cout << "Program(version=" << program.version << ")\n";
        for (const auto& statement : program.statements) {
            print_indent();
            statement->accept(*this);
        }
    }

    void visit(Include& include) override {
        std::cout << "Include(filename=" << include.filename << ")\n";
    }

    void visit(QRegister& qreg) override {
        std::cout << "QReg(name=" << qreg.name 
            << ", size=" << qreg.size << ")\n";
    }

    void visit(CRegister& creg) override {
        std::cout << "CReg(name=" << creg.name 
            << ", size=" << creg.size << ")\n";
    }
    void visit(Gate& gate) override {
        std::cout << "Gate(name=" << gate.name;
        std::cout << ", qubits=";
        for (const auto& qubit : gate.qubits) {
            std::cout << qubit;
            // if (&qubit != &gate.qubits.back()) {
            //     std::cout << ", ";
            // }
        }
        std::cout << ")\n";
    }

    void visit(Measure& measure) override {
        std::cout << "Measure(qubit=";
        for (const auto& qubit : measure.qubits) {
            std::cout << qubit;
        }
        std::cout << ", creg=";
        for (const auto& qubit : measure.qubits) {
            std::cout << qubit;
        }
        std::cout << ")\n";
    }

    void visit(Barrier& barrier) override {
        std::cout << "Barrier(qubits=";
        for (const auto& qubit : barrier.qubits) {
            std::cout << qubit;
        }
        std::cout << ")\n"; 
    }

    void visit(Reset& reset) override {

    }


};




};