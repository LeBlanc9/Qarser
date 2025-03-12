#pragma once
#include <vector>
#include <memory>
#include "AST/visitor.hpp"
#include "SA/error/error.hpp"
#include "SA/symbol/symbol.hpp"
#include "SA/symbol/gate_scope.hpp"


namespace qarser {

    class SemanticAnalyer : public BaseVisitor {
    private:
        SymbolTable symbol_table;
        ErrorCollector errors;

    public:
        SemanticAnalyer() {
            init_builtins();
        }


        void analyze(Program& program) {
            for (const auto& stmt : program.statements) {
                stmt->accept(*this);
                // if (has_errors()) 
                //     return;
            }

            if (has_errors()) 
                errors.print_errors();
            else 
                std::cout << "Semantic Analyze Success!" << std::endl;
        }

    
        void visit(QRegister& qreg) override {
            if (qreg.size <= 0) {
                errors.add_error(qreg.line, "Invalid quantum register size");
                return;
            }
            if (!symbol_table.add_qreg(qreg.name, qreg.size)) {
                errors.add_error(qreg.line, "Redefinition of quantum register '" + qreg.name + "'");
            }
        }

        void visit(CRegister& creg) override {
            if (creg.size <= 0) {
                errors.add_error(creg.line, "Invalid classical register size");
                return;
            }
            if (!symbol_table.add_creg(creg.name, creg.size)) {
                errors.add_error(creg.line, "Redefinition of classical register '" + creg.name + "'");
            }
        }

       void visit(GateDef& gate_def) override {
            if (!symbol_table.add_gate(gate_def.name, gate_def.params.size(), gate_def.qubits.size())) {
                errors.add_error(gate_def.line, "Redefinition of gate '" + gate_def.name + "'");
                return;
            }

            GateScope scope;
            for (const auto& qubit : gate_def.qubits) {
                if (!scope.add_qubit(qubit.name)) {
                    errors.add_error(gate_def.line, 
                        "Name '" + qubit.name + "' already used in gate definition");
                }
            }

            for (const auto& param : gate_def.params) {
                if (!scope.add_param(param)) {
                    errors.add_error(gate_def.line, 
                        "Name '" + param + "' already used in gate definition");
                }
            }

            GateBodyAnalyzer body_analyzer(scope, symbol_table, errors);
            for (const auto& stmt : gate_def.body) {
                stmt->accept(body_analyzer);
            }
        }


        void visit(Gate& gate) override {
            const GateSymbol* symbol = symbol_table.lookup_gate(gate.name);
            if (!symbol) {
                errors.add_error(gate.line, "Gate '" + gate.name + "' not declared");
                return;
            }

            // Check params count
            if (gate.params.size() != symbol->num_params) {
                errors.add_error(gate.line, 
                    "Gate '" + gate.name + "' expects " + 
                    std::to_string(symbol->num_params) + " parameters, got " +
                    std::to_string(gate.params.size()));
                return;
            }


            auto expanded_qubits = expand_register_refs(gate.qubits);

            // Check qubit count 
            if (expanded_qubits.size() != symbol->num_qubits) {
                errors.add_error(gate.line, 
                    "Gate '" + gate.name + "' expects " + 
                    std::to_string(symbol->num_qubits) + " qubits, got " +
                    std::to_string(expanded_qubits.size()));
                return;
            }

            // Check register refs legality
            for (const auto& ref : expanded_qubits) {
                if (ref.index >= symbol_table.get_register_size(ref.name)) {
                    errors.add_error(gate.line, "register '" + ref.name + "' index out of range");
                    return;
                }
            }
       }

    
    private:
        void init_builtins() {
            symbol_table.add_gate("U", 3, 1);
            symbol_table.add_gate("CX", 0, 2);
        }

        bool has_errors() { 
            return errors.has_errors(); 
        }


        // Eg.
        // QREG qa[3], qb[2]
        // {qa[0], qb, qa[2]} ->  {qa[0], qb[0], qb[1], qa[2]}
        // Importance: use after declare registers !
        std::vector<RegisterRef> expand_register_refs(const std::vector<RegisterRef>& ref) {
            std::vector<RegisterRef> expanded;
            for (const auto& r : ref) {
                if (r.index == -1) {
                    size_t size = symbol_table.get_register_size(r.name);
                    for (int i = 0; i < size; ++i) {
                        expanded.push_back(RegisterRef(r.name, i));
                    }
                } 
                else
                    expanded.push_back(r);
            }
            return expanded;
        }
       
    };
        

}; // namespace qarser