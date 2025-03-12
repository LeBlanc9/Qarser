#pragma once
#include <memory>

namespace qarser {

    enum class SymbolType {
        QREG,
        CREG,
        GATE,
    };

    inline std::string symbolTypeToString(SymbolType type) {
        switch (type) {
            case SymbolType::QREG: return "QReg";
            case SymbolType::CREG: return "CReg";
            case SymbolType::GATE: return "Gate";
        }
        return "Unknown";
    }


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
        int num_params;
        int num_qubits;

    public:
        GateSymbol(const std::string& name, int num_params, int num_qubits) 
            :   Symbol(SymbolType::GATE, name), 
                num_params(num_params),
                num_qubits(num_qubits) {} 
    };



    class SymbolTable {
    private:
        std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols;

    public:
        SymbolTable() = default;

        void insert_unchecked(const std::string& name, std::unique_ptr<Symbol> symbol) {
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


        // 查找符号
        const Symbol* lookup(const std::string& name) const {
            auto it = symbols.find(name);
            return it != symbols.end() ? it->second.get() : nullptr;
        }


        template<typename T>
        T* getAs(const std::string& name) {
            return dynamic_cast<T*>(symbols[name].get());
        }

    };

}; // namespace qarser