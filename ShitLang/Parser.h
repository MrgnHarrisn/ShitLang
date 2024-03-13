#pragma once

#include "Token.h"
#include "Node.h"
#include <vector>

#include <map>

class Parser {
    std::vector<Token> tokens;
    size_t position = 0;

public:
    explicit Parser(const std::vector<Token>& tokens, std::map<std::string, double>* var_map) : tokens(tokens), variables(var_map) {}

    Node* parse() {
        Node* result = nullptr;
        while (position < tokens.size()) {
            result = parseStatement();
        }
        return result; // Be cautious with memory management here
    }


    void set_variables(std::map<std::string, double>* var_map);

private:
    Token& currentToken() {
        return tokens[position];
    }

    void eatToken(TokenType type) {
        if (currentToken().get_type() == type) {
            position++;
        }
        else {
            throw std::runtime_error("Unexpected token");
        }
    }

    Node* parseStatement() {
        if (currentToken().get_type() == LET) {
            return parseVariableDeclaration();
        }
        else if (currentToken().get_type() == PRINT) {
            eatToken(PRINT);
            Node* expr = parseExpression();
            return new PrintNode(expr);
        }
        else {
            return parseExpression(); // For cases that are not variable declarations
        }
    }

    Node* parseVariableDeclaration() {
        eatToken(LET); // Consume the 'LET' token
        std::string varName = std::any_cast<std::string>(currentToken().get_value());
        eatToken(VARIABLE); // Consume the variable name token

        eatToken(ASSIGN); // Consume the '=' token

        // Now expect an expression for the variable value
        Node* value = parseExpression();

        // Assuming you have a method to update your variables map
        if (variables->find(varName) == variables->end()) {
            (*variables)[varName] = value->evaluate(); // Evaluate the expression and store the result
        }
        else {
            throw std::runtime_error("Variable redeclaration: " + varName);
        }

        delete value; // Clean up the expression node
        return new NoOpNode(); // Or any other way you signify a non-evaluative result
    }


    Node* parseExpression() {
        Node* node = parseTerm(); // Start with the highest precedence operations

        while (position < tokens.size() &&
                (currentToken().get_type() == PLUS || currentToken().get_type() == MINUS ||
                currentToken().get_type() == GREATER_THAN || currentToken().get_type() == LESS_THAN) || 
                currentToken().get_type() == GREATER_THAN_EQ || currentToken().get_type() == LESS_THAN_EQ ||
                currentToken().get_type() == EQEQ || currentToken().get_type() == AND || currentToken().get_type() == OR) {
            TokenType opType = currentToken().get_type();
            eatToken(opType);

            // Depending on the operator, we either parse another term (for +, -) or move directly to a relational operation (for <, >)
            Node* right = parseTerm(); // Continue with arithmetic operations

            // Now differentiate between arithmetic and relational operations
            if (opType == PLUS || opType == MINUS) {
                char op = opType == PLUS ? '+' : '-';
                node = new BinaryOperationNode(node, right, op); // Existing arithmetic node
            }
            else if (opType == GREATER_THAN || opType == LESS_THAN || opType == GREATER_THAN_EQ || opType == LESS_THAN_EQ || opType == EQEQ || opType == AND || opType == OR) {
                // char op = opType == GREATER_THAN ? '>' : '<';
                char op;
                switch (opType) {
                case GREATER_THAN:      op = '>'; break;
                case LESS_THAN:         op = '<'; break;
                case GREATER_THAN_EQ:   op = ','; break;
                case LESS_THAN_EQ:      op = '.'; break;
                case EQEQ:              op = '='; break;
                case AND:               op = '&'; break;
                case OR:                op = '|'; break;
                }
                node = new RelationalOperationNode(node, right, op); // New relational node
            }
        }

        return node;
    }

    Node* parseTerm() {
        Node* node = parseFactor();
        while (position < tokens.size() &&
            (currentToken().get_type() == MULT || currentToken().get_type() == DIVIDE || currentToken().get_type() == EXPONENT)) {
            TokenType opType = currentToken().get_type();
            eatToken(opType); // Now we consume the token correctly before creating the node
            Node* right = parseFactor(); // Parse the right-hand side of the operation
            if (opType == EXPONENT) {
                node = new BinaryOperationNode(node, right, '^'); // Handle exponentiation
            }
            else {
                char op = opType == MULT ? '*' : '/';
                node = new BinaryOperationNode(node, right, op);
            }
        }
        return node;
    }


    Node* parseFactor() {
        if (currentToken().get_type() == INTEGER || currentToken().get_type() == FLOAT) {
            double value = std::any_cast<double>(currentToken().get_value());
            eatToken(currentToken().get_type());
            return new NumberNode(value);
        }
        else if (currentToken().get_type() == VARIABLE) {
            std::string varName = std::any_cast<std::string>(currentToken().get_value());
            if (variables && variables->find(varName) != variables->end()) {
                double value = (*variables)[varName];
                eatToken(VARIABLE);
                return new NumberNode(value);
            }
            else {
                throw std::runtime_error("Undefined variable: " + varName);
            }
        }
        else if (currentToken().get_type() == LPAREN) {
            eatToken(LPAREN);
            Node* node = parseExpression();
            eatToken(RPAREN);
            return node;
        }
        else {
            std::cout << currentToken().to_str() << std::endl;
            throw std::runtime_error("Unexpected token in factor");
        }
    }

    // variables
    std::map<std::string, double>* variables = nullptr;

};
