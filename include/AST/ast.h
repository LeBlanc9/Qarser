#pragma once
#include <vector>
#include <string>
#include "ast_visitor.h"


namespace qarser {




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


;

// TODO
// class If : public Statement {
// public:
//     std::string creg;
//     std::string value;
//     std::unique_ptr<Statement> statement;
// };



}; // namespace qarser