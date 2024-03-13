#pragma once

#include <stdexcept>
#include <iostream>
class Node {
public:
    virtual ~Node() = default;
    virtual double evaluate() const = 0; // Method to evaluate the node's value
};

class NumberNode : public Node {
    double value;

public:
    explicit NumberNode(double value) : value(value) {}
    double evaluate() const override { return value; }
};

class NoOpNode : public Node {
public:
    double evaluate() const override {
        return 0; // Or potentially throw an exception if this should never be evaluated
    }
};

class UnaryOperationNode : public Node {
    Node* operand;
    char operation; // For simplicity, this example uses a char to represent the operation.

public:
    UnaryOperationNode(Node* operand, char operation)
        : operand(operand), operation(operation) {}

    double evaluate() const override {
        switch (operation) {
        case '-': return -operand->evaluate();
            // Add cases for other unary operations as necessary.
        default: throw std::invalid_argument("Unsupported unary operation");
        }
    }

    ~UnaryOperationNode() {
        delete operand;
    }
};

class PrintNode : public Node {
    Node* expression;

public:
    explicit PrintNode(Node* expression) : expression(expression) {}

    double evaluate() const override {
        double value = expression->evaluate();
        std::cout << value << std::endl; // Print the value to stdout
        return value; // You might return the printed value or simply return 0 to indicate success.
    }

    ~PrintNode() {
        delete expression;
    }
};

// Logic
class RelationalOperationNode : public Node {
    Node* left;
    Node* right;
    char operation;

public:
    RelationalOperationNode(Node* left, Node* right, char operation)
        : left(left), right(right), operation(operation) {}

    double evaluate() const override {
        // Implement evaluation logic for relational operators
        // For example:
        switch (operation) {
        case '<': return left->evaluate() < right->evaluate(); // True is 1, false is 0
        case '>': return left->evaluate() > right->evaluate();
        case '.': return left->evaluate() <= right->evaluate(); // Why tf does this work
        case ',': return left->evaluate() >= right->evaluate();
        case '=': return left->evaluate() == right->evaluate();
        default: throw std::invalid_argument("Unsupported relational operation");
        } // it bothers me so much that this works
    }

    ~RelationalOperationNode() {
        delete left;
        delete right;
    }
};

// MATH
class BinaryOperationNode : public Node {
    Node* left;
    Node* right;
    char operation;

public:
    BinaryOperationNode(Node* left, Node* right, char operation)
        : left(left), right(right), operation(operation) {}

    double evaluate() const override {
        switch (operation) {
        case '+': return left->evaluate() + right->evaluate();
        case '-': return left->evaluate() - right->evaluate();
        case '*': return left->evaluate() * right->evaluate();
        case '/': return left->evaluate() / right->evaluate();
        case '^': return std::pow(left->evaluate(), right->evaluate());
        default: throw std::invalid_argument("Unsupported operation");
        }
    }


    ~BinaryOperationNode() {
        delete left;
        delete right;
    }
};
