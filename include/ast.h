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
    std::vector<Statement*> statements;
};

class Include : public Statement {
public:
    std::string filename;
};

// Register Declaration
class QRegister : public Statement {
public:
    std::string name;
    int size;
};

class CRegister : public Statement {
public:
    std::string name;
    int size;
};

class Gate : public Statement {
public:
    std::string name;
    std::vector<std::string> qubit;
};

class Measure : public Statement {
public:
    std::string qubit;
    std::string cbit;
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