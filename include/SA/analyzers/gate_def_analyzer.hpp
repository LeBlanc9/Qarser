#pragma once
#include "analyzer.hpp"
#include "SA/context/gate_def_context.hpp"


namespace qarser {

    class GateDefBodyAnalyzer : public BaseAnalyzer {
    private:
        GateScope& gate_scope;

    public:
        GateDefBodyAnalyzer(AnalysisContext& context, GateScope& gate_scope)
            : BaseAnalyzer(context), gate_scope(gate_scope) {}

        void visit(Gate& gate) {
            auto* gate_symbol = context.get_symbols().lookup_gate(gate.name);
            if (!gate_symbol) {
                context.add_error(gate.line, "Undefined gate '" + gate.name + "'");
                return;
            }

            if (gate.params.size() != gate_symbol->num_params) {
                context.add_error(gate.line, 
                    "Gate '" + gate.name + "' expects " + 
                    std::to_string(gate_symbol->num_params) + " parameters, got " +
                    std::to_string(gate.params.size()));
                return;
            }

        }

    };





    class GateDefAnalyzer : public BaseAnalyzer {
    private:
        std::unique_ptr<GateDefBodyAnalyzer> body_analyzer = nullptr;
        std::unique_ptr<GateScope> gate_scope = nullptr;

    public:
        GateDefAnalyzer(AnalysisContext& context) 
            : BaseAnalyzer(context) {}


        void visit(GateDef& gate_def) override {
            if (!context.get_symbols().add_gate(gate_def.name, gate_def.params.size(), gate_def.qubits.size())) {
                context.add_error(gate_def.line, "Redefinition of gate '" + gate_def.name + "'");
                return;
            }


            gate_scope = std::make_unique<GateScope>();
            body_analyzer = std::make_unique<GateDefBodyAnalyzer>(context, *gate_scope);

            for (const auto& qubit : gate_def.qubits) {
                if (!gate_scope->add_qubit(qubit.name)) {
                    context.add_error(gate_def.line, "Name '" + qubit.name + "' already used in gate definition");
                }
            }

            for (const auto& param : gate_def.params) {
                if (!gate_scope->add_param(param)) {
                    context.add_error(gate_def.line, "Name '" + param + "' already used in gate definition");
                }
            }


            for (const auto& stmt : gate_def.body) {
                stmt->accept(*body_analyzer);
            }
        }

    };








}; // namespace qarser