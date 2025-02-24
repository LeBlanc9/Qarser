#pragma once
#include <memory>

namespace qarser {

enum class SymbolType {
    QREG,
    CREG,
    GATE,
    PARAMETER
};

class Symbol {
public:
    SymbolType type;
    std::string name;

    Symbol(SymbolType type, const std::string& name) 
        : type(type), name(name) {}

    virtual ~Symbol() = default;
};


class RegisterSymbol : public Symbol {
public:
    int size;

public:
    RegisterSymbol(SymbolType type, const std::string& name, int size) 
        : Symbol(type, name), size(size) {}
};


class GateSymbol : public Symbol {
public:
    int num_qubits;
    int num_params;

public:
    GateSymbol(const std::string& name, int num_qubits, int num_params) 
        :   Symbol(SymbolType::GATE, name), 
            num_qubits(num_qubits), 
            num_params(num_params) {}
};



class SymbolTable {
private:
    std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols;

public:

    void insertUnchecked(const std::string& name, std::unique_ptr<Symbol> symbol) {
        symbols[name] = std::move(symbol);
    }

    void insert(const std::string& name, std::unique_ptr<Symbol> symbol) {
        if (exists(name)) {
            throw std::runtime_error("Symbol '" + name + "' already defined");
        }
        symbols[name] = std::move(symbol);
    }


    bool exists(const std::string& name) {
        return symbols.find(name) != symbols.end();
    } 

};

};