#pragma once

namespace qarser {

class Program;
class Include;
class QRegister;
class CRegister;
class Gate;
class Measure;
class Reset;
class Barrier;
class GateDef;

class NumberExpr;
class IdentifierExpr;
class UnaryExpr;
class BinaryExpr;



class AstVisitor {
public: 
    virtual void visit(Program& program) = 0;
    virtual void visit(Include& include) = 0;
    virtual void visit(QRegister& qreg) = 0;
    virtual void visit(CRegister& creg) = 0;
    virtual void visit(Gate& gate) = 0;
    virtual void visit(Measure& measure) = 0;
    virtual void visit(Reset& reset) = 0;
    virtual void visit(Barrier& barrier) = 0;
    virtual void visit(GateDef& gate_def) = 0;

    virtual void visit(NumberExpr& expr) = 0;
    virtual void visit(IdentifierExpr& expr) = 0;
    virtual void visit(UnaryExpr& expr) = 0;
    virtual void visit(BinaryExpr& expr) = 0;

    virtual ~AstVisitor() = default;
};


}; // namespace qarser