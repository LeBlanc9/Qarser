#pragma once
#include <vector>
#include <string>


namespace qarser {


class AstNode {
public:
    virtual ~AstNode() = default;
};


class Statement : public AstNode {
public: 
    virtual ~Statement() = default;
};


class Program : public AstNode {
public:
    double version;
    std::vector<std::unique_ptr<Statement>> statements;
};

class Include : public Statement {
public:
    std::string filename;
public:
    Include(const std::string& filename) 
        : filename(filename) {}
};

// Register Declaration
class QRegister : public Statement {
public:
    std::string name;
    int size;
public:
    QRegister(const std::string& name, int size) 
        : name(name), size(size) {}
};

class CRegister : public Statement {
public:
    std::string name;
    int size;
public:
    CRegister(const std::string& name, int size) 
        : name(name), size(size) {}
};

class Gate : public Statement {
public:
    std::string name;
    std::vector<std::string> qubit;
public:
    Gate(std::string name, const std::vector<std::string>& qubit) 
        : name(name), qubit(qubit) {}
};

class Measure : public Statement {
public:
    std::vector<std::string> qubits;
    std::vector<std::string> cbits;
public:
    Measure(const std::vector<std::string>& qubits, const std::vector<std::string>& cbits) 
        : qubits(qubits), cbits(cbits) {}
};


class Barrier : public Statement {
public:
    std::vector<std::string> qubits;
};

class Reset : public Statement {
public:
    std::string qubit;
};

class If : public Statement {
public:
    std::string creg;
    std::string value;
    std::unique_ptr<Statement> statement;
};


class CustomGateDeclare : public Statement {
public:
    // TODO
};


}; // namespace qarser