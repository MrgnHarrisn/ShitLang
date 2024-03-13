#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Tokenizer.h"
#include "Parser.h"
#include "Node.h"

#define disp(msg) std::cout << msg << std::endl;

void interpret(const std::string& input, std::map<std::string, double>& variables) {
    Tokenizer toker(input, &variables);
    std::vector<Token> tokens = toker.tokenize();

    for (Token t : tokens) {
        disp(t.to_str());
    }

    if (tokens.empty()) {
        // std::cout << "No expression to evaluate or syntax error." << std::endl;
        return;
    }

    try {
        Parser parser(tokens, &variables);
        Node* root = parser.parse();

        if (root != nullptr) {
            double result = root->evaluate();
        }
        else {
            std::cout << "No expression to evaluate." << std::endl;
        }

        delete root;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    std::map<std::string, double> variables;

    if (argc > 1) {
        // File mode
        std::string filename = argv[1];
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return 1;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                interpret(line, variables);
            }
        }
    }
    else {
        // Interactive mode
        std::string data;
        while (true) {
            std::cout << ">>> ";
            std::getline(std::cin, data);

            if (data == "exit") {
                break;
            }

            interpret(data, variables);
        }
    }

    return 0;
}
