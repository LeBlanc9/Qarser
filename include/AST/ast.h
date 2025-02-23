#pragma once
#include <vector>
#include <string>
#include "ast_visitor.h"


namespace qarser {


class AstNode {
public:
    virtual ~AstNode() = default;
    virtual void accept(AstVisitor& visitor) = 0;
};


class Statement : public AstNode {
public: 
    virtual ~Statement() = default;
    virtual void accept(AstVisitor& visitor) = 0;
};


class Program : public AstNode {
public:
    double version;
    std::vector<std::unique_ptr<Statement>> statements;

public:  
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Include : public Statement {
public:
    std::string filename;
public:
    Include(const std::string& filename) 
        : filename(filename) {}

    void accept(AstVisitor& visitor) override { 
        visitor.visit(*this);
    }
};

// Register Declaration
class QRegister : public Statement {
public:
    std::string name;
    int size;
public:
    QRegister(const std::string& name, int size) 
        : name(name), size(size) {}
    
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class CRegister : public Statement {
public:
    std::string name;
    int size;
public:
    CRegister(const std::string& name, int size) 
        : name(name), size(size) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Gate : public Statement {
public:
    std::string name;
    std::vector<std::string> qubits;
public:
    Gate(std::string name, const std::vector<std::string>& qubits)
        : name(name), qubits(qubits) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Measure : public Statement {
public:
    std::vector<std::string> qubits;
    std::vector<std::string> cbits;
public:
    Measure(const std::vector<std::string>& qubits, const std::vector<std::string>& cbits) 
        : qubits(qubits), cbits(cbits) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};


class Barrier : public Statement {
public:
    std::vector<std::string> qubits;
public:
    Barrier(const std::vector<std::string>& qubits) 
        : qubits(qubits) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Reset : public Statement {
public:
    std::string qubit;

public:
    Reset(const std::string& qubit) 
        : qubit(qubit) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};



// TODO
// class If : public Statement {
// public:
//     std::string creg;
//     std::string value;
//     std::unique_ptr<Statement> statement;
// };


// class CustomGateDeclare : public Statement {
// public:
// };


}; // namespace qarser