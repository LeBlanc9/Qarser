#pragma once
#include <vector>
#include <memory>
#include "AST/visitor.hpp"
#include "SA/symbol/symbol.hpp"
#include "SA/error/error.hpp"
#include "SA/checkers/checker.hpp"


namespace qarser {

    class SemanticAnalyzer : public AstVisitor {
    private:
        SymbolTable symbol_table;
        ErrorCollector errors;

    public:
        SemanticAnalyzer() {
            init_builtins();
        }


        void analyze(Program& program) {
            for (const auto& stmt : program.statements) {
                stmt->accept(*this);
                if (has_errors()) 
                    return;
            }
        }
    
        void visit(QRegister& qreg) override {
            checkRegisterDeclaration(qreg);
        }

        void visit(CRegister& creg) override {
            checkRegisterDeclaration(creg);
        }

        void visit(GateDef& gate_def) override {
        }

        void visit(Gate& gate) override {
        }

    
        
    
    private:
        void init_builtins() {
            symbol_table.insert_unchecked("U", std::make_unique<GateSymbol>("U", 3, 1));
            symbol_table.insert_unchecked("CX", std::make_unique<GateSymbol>("CX", 0, 2));
        }

        bool has_errors() { 
            return errors.has_errors(); 
        }

        
        void checkRegisterDeclaration(const Register& reg) {
            if (reg.size <= 0) {
                errors.add_error(reg.line, 
                    "Register size must be positive");
                return;
            }
    
            if (symbol_table.exists(reg.name)) {
                errors.add_error(reg.line, "Register '" + reg.name + "' already declared");
                return;
            }
    
            symbol_table.insert(reg.name,
                std::make_unique<RegisterSymbol>(
                    reg.type(),
                    reg.name,
                    reg.size
                )
            );
        }

    };
        

}; // namespace qarser