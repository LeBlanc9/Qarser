#pragma once
#include <vector>
    
namespace qarser {
    

    class SemanticError {
    public:
        int line;
        std::string message;

    public:
        SemanticError(int line ,const std::string& message) 
            : line(line), message(message) {}
    };


    class ErrorCollector {
    private:
        std::vector<SemanticError> errors;
        
    public:
        ErrorCollector() = default;       

        void add_error(int line, const std::string& message) {
            errors.emplace_back(line, message);
        }

        bool has_errors() {
            return !errors.empty();
        }

        const std::vector<SemanticError>& get_errors() const {
            return errors;
        }

    };


}; // namespace qarser