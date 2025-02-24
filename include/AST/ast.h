#pragma once
#include <vector>
#include <string>
#include "ast_visitor.h"


namespace qarser {


class RegisterRef {
private:
    std::string name;
    int index;

public:
    RegisterRef(const std::string& name) 
        : name(name), index(-1) {}

    RegisterRef(const std::string& name, int index) 
        : name(name), index(index) {}

    bool isWholeRegister() const {
        return index == -1;
    }    

    std::string toString() const {
        if (isWholeRegister()) {
            return name;
        }
        return name + "[" + std::to_string(index) + "]";
    }
};



class AstNode {
public:
    int line;

    AstNode(int line = 0) : line(line) {}
    virtual ~AstNode() = default;
    virtual void accept(AstVisitor& visitor) = 0;
};


class Statement : public AstNode {
public: 
    Statement(int line = 0) : AstNode(line) {}
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
    QRegister(int line, const std::string& name, int size) 
        : Statement(line), name(name), size(size) {}
    
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class CRegister : public Statement {
public:
    std::string name;
    int size;
public:
    CRegister(int line, const std::string& name, int size) 
        : Statement(line), name(name), size(size) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};


class Gate : public Statement {
public:
    std::string name;
    std::vector<RegisterRef> qubits;
public:
    Gate(int line, std::string name, const std::vector<RegisterRef>& qubits)
        : Statement(line), name(name), qubits(qubits) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Measure : public Statement {
public:
    std::vector<RegisterRef> qubits;
    std::vector<RegisterRef> cbits;
public:
    Measure(int line, const std::vector<RegisterRef>& qubits, const std::vector<RegisterRef>& cbits) 
        : Statement(line), qubits(qubits), cbits(cbits) {}

    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    }
};


class Barrier : public Statement {
public:
    std::vector<RegisterRef> qubits;
public:
    Barrier(int line, const std::vector<RegisterRef>& qubits) 
        : Statement(line), qubits(qubits) {}

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