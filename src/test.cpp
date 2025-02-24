#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "ast_print.h"
#include "semantic_analyer.h"

std::string debug_qasm2 = R"(
    OPENQASM 2.0;
    include "qelib1.inc";
    qreg q[6];
    creg meas[6];
    h q[0];
    h q[1];
    h q[2];
    barrier q[0],q[1],q[2],q[3],q[4],q[5];
    cx q[2],q[4];
    x q[3];
    cx q[2],q[3];
    ccx q[0],q[1],q[3];
    x q[0];
    x q[1];
    ccx q[0],q[1],q[3];
    x q[0];
    x q[1];
    x q[3];
    barrier q[0],q[1],q[2],q[3],q[4],q[5];
    h q[0];
    h q[1];
    h q[2];
    barrier q[0],q[1],q[2],q[3],q[4],q[5];
    measure q[0] -> meas[0];
    measure q[1] -> meas[1];
    measure q[2] -> meas[2];
    measure q[3] -> meas[3];
    measure q[4] -> meas[4];
    measure q[5] -> meas[5];
)";

std::string debug_qasm = R"(
    OPENQASM 2.0;
    include "qelib1.inc";
    qreg q[6];
    creg meas[6];
    ccx q[0],q[1],q[3];
)";


void test_lexer() {
    qarser::QasmLexer lexer(debug_qasm);
    while (!lexer.is_at_end()) {
        lexer.next().print();
    }
}

void test_parser() {
    qarser::Parser parser(debug_qasm2);
    auto ast = parser.parse();

    qarser::AstPrinter printer;
    ast->accept(printer);
}


int main() {
    test_parser();
    // test_lexer();
    return 0;
}
