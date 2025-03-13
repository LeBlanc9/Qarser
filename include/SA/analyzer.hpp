#pragma once
#include <vector>
#include <memory>
#include "AST/visitor.hpp"
#include "SA/context/analysis_context.hpp"

#include "analyzers/declaration_analyzer.hpp"
#include "analyzers/gate_usage_analyzer.hpp"
#include "analyzers/gate_def_analyzer.hpp"



namespace qarser {

    class SemanticAnalyzer {
    private:
        AnalysisContext context;
        

        std::unique_ptr<DeclarationAnalyzer> declaration_analyzer;
        std::unique_ptr<GateAnalyzer> gate_analyzer;
        std::unique_ptr<GateDefAnalyzer> gate_def_analyzer;

    public:
        SemanticAnalyzer() {
            init_builtins();
            init_analyzers();
        }


        void analyze(Program& program) {
            for (const auto& stmt : program.statements) {
                switch (stmt->kind()) {
                    case Statement::Kind::QREG:
                        stmt->accept(*declaration_analyzer);
                        break;
                    case Statement::Kind::CREG:
                        stmt->accept(*declaration_analyzer);
                        break;
                    case Statement::Kind::GATE:
                        stmt->accept(*gate_analyzer);
                        break;
                    case Statement::Kind::GATE_DEF:
                        stmt->accept(*gate_def_analyzer);
                        break;
                   default:
                        break;
                }
            }
            context.get_errors().report();
        }

        
    private:
        void init_builtins() {
            context.get_symbols().add_gate("U", 3, 1);
            context.get_symbols().add_gate("CX", 0, 2);
        }

        void init_analyzers() {
            declaration_analyzer = std::make_unique<DeclarationAnalyzer>(context);
            gate_analyzer = std::make_unique<GateAnalyzer>(context);
            gate_def_analyzer = std::make_unique<GateDefAnalyzer>(context);
        }

    };


        

}; // namespace qarser