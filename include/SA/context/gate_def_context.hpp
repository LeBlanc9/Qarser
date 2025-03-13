#pragma once
#include <unordered_map>
#include <string>



namespace qarser {

    class GateScopeSymbol {
    public:
        std::string name;
        
        explicit GateScopeSymbol(const std::string& name) : name(name) {}
        virtual ~GateScopeSymbol() = default;
    };


    class ParamSymbol : public GateScopeSymbol {
    public:
        explicit ParamSymbol(const std::string& name) 
            : GateScopeSymbol(name) {}
    };

    class QubitArgSymbol : public GateScopeSymbol {
    public:
        explicit QubitArgSymbol(const std::string& name) 
            : GateScopeSymbol(name) {}
    };



    class GateScope {
    private:
        std::unordered_map<std::string, ParamSymbol> params;
        std::unordered_map<std::string, QubitArgSymbol> qargs;

    public:
        bool exists(const std::string& name) const {
            return  params.find(name) != params.end() || 
                    qargs.find(name) != qargs.end();
        }

        bool add_param(const std::string& name) {
            if (exists(name)) {
                return false;
            }
            return params.emplace(name, ParamSymbol(name)).second;
        }

        bool add_qubit(const std::string& name) {
            if (exists(name)) {
                return false;
            }
            return qargs.emplace(name, QubitArgSymbol(name)).second;
        }

        const ParamSymbol* lookup_param(const std::string& name) const {
            auto it = params.find(name);
            return it != params.end() ? &it->second : nullptr;
        }

        const QubitArgSymbol* lookup_qubit(const std::string& name) const {
            auto it = qargs.find(name);
            return it != qargs.end() ? &it->second : nullptr;
        }
    };




    class GateBodyAnalyzer : public BaseVisitor {
    private:
        const GateScope& scope;
        const SymbolTable& symbol_table;  // 添加符号表引用
        ErrorCollector& errors;
    
    public:
        GateBodyAnalyzer(
            const GateScope& scope, 
            const SymbolTable& symbol_table,
            ErrorCollector& errors
        ) : scope(scope), 
            symbol_table(symbol_table), 
            errors(errors) {}
    
        void visit(Gate& gate) override {
            // 1. check gate name
            const GateSymbol* gate_symbol = symbol_table.lookup_gate(gate.name);
            if (!gate_symbol) {
                errors.add_error(gate.line, "Undefined gate '" + gate.name + "' in gate body");
                return;
            }
    
            // 2. check params
            if (gate.params.size() != gate_symbol->num_params) {
                errors.add_error(gate.line,
                    "Gate '" + gate.name + "' expects " + 
                    std::to_string(gate_symbol->num_params) + " parameters, got " +
                    std::to_string(gate.params.size()));
                return;
            }
    
            for (const auto& param : gate.params) {
            }
    
            // 3. check qubit
            if (gate.qubits.size() != gate_symbol->num_qubits) {
                errors.add_error(gate.line,
                    "Gate '" + gate.name + "' expects " + 
                    std::to_string(gate_symbol->num_qubits) + " qubits, got " +
                    std::to_string(gate.qubits.size()));
                return;
            }
    
            for (const auto& qubit : gate.qubits) {
                if (!scope.lookup_qubit(qubit.name)) {
                    errors.add_error(gate.line, "Undefined qubit '" + qubit.name + "' in gate body");
                }
            }
        }
    };


}