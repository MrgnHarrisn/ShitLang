#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::string& text) {
    this->text = text;
    variables = new std::map<std::string, double>;
}

Tokenizer::Tokenizer(const std::string& text, std::map<std::string, double>* map) {
    this->text = text;
    variables = map;
}

std::vector<Token> Tokenizer::tokenize() {
    if (position > text.size()) {
        error("No tokens in program");
    }

    while (position < text.size()) {
        skip_spaces();
        current_char = text[position];

        if (isalpha(current_char)) { // Handle words
            std::string word = get_word();
            handle_word(word);
            position--;
        }
        else if (current_char == '-' && (tokens.empty() || !isdigit(text[position + 1]) || tokens.back().get_type() == LPAREN || tokens.back().get_type() == PLUS || tokens.back().get_type() == MINUS || tokens.back().get_type() == MULT || tokens.back().get_type() == DIVIDE)) {
            // Treat as subtraction operator if the '-' is not at the start or not followed by a digit
            tokens.push_back(Token(MINUS, '-'));
        }
        else if (current_char == '-' && isdigit(text[position + 1])) {
            // Treat as a negative number
            position++; // Advance position to correctly parse the negative number
            auto variant = get_number(true); // Assuming get_number correctly handles negative flag
            double new_val = std::visit([](auto&& value) -> double {
                return static_cast<double>(value);
                }, variant);
            tokens.push_back(Token(is_whole_number(new_val) ? INTEGER : FLOAT, new_val));
        }
        else if (isdigit(current_char)) { // Handle numbers
            auto variant = get_number(false);
            double new_val = std::visit([](auto&& value) -> double {
                return static_cast<double>(value);
                }, variant);
            tokens.push_back(Token(is_whole_number(new_val) ? INTEGER : FLOAT, new_val));
        }
        else if (current_char == '\'') {
            /* handle for a char */
        }
        else if (current_char == '^') {
            tokens.push_back(Token(EXPONENT, '^'));
        }
        else if (current_char == '+') {
            tokens.push_back(Token(PLUS, '+'));
        }
        else if (current_char == '/') {
            tokens.push_back(Token(DIVIDE, '/'));
        }
        else if (current_char == '*') {
            tokens.push_back(Token(MULT, '*'));
        }
        else if (current_char == '(') {
            tokens.push_back(Token(LPAREN, '('));
        }
        else if (current_char == ')') {
            tokens.push_back(Token(RPAREN, ')'));
        }
        else if (current_char == '=') {
            if (text[position + 1] == '=') {
                tokens.push_back(Token(EQEQ, '='));
            }
        }
        else if (current_char == '<') {
            if (text[position + 1] == '=') {
                tokens.push_back(Token(LESS_THAN_EQ, ',')); // is it flipping the numbers?
                // maybe swapping their order in the parser? no not the operations
                // the order of the numbers e.g:
                // 1 < 2
                // goes into the parser -> (comes out) -> 2 < 1
                // the first number could be getting placed on the right
                // the second getting placed on the first
                // whatever it works
                // I hate this code haven't added it yet
                position++;
            }
            else {
                tokens.push_back(Token(LESS_THAN, '<'));
            }
        }
        else if (current_char == '>') {
            if (text[position + 1] == '=') {
                tokens.push_back(Token(GREATER_THAN_EQ, '.'));
                position++;
            }
            else {
                tokens.push_back(Token(GREATER_THAN, '>'));
            } // it should
        }
        else if (current_char != ' ') {
            std::cout << "Current Character: " << current_char << std::endl;
            error("Unknown Token");
        }

        position++;
    }

    return tokens;
}


void Tokenizer::error(const std::string& message) {
    std::cerr << message << "\n";
    std::cerr << "Error occurred at position " << position << "\n";
}

std::variant<int, double> Tokenizer::get_number(bool is_neg) {
    bool is_floating = false;
    std::string num;

    while (position < text.size()) {
        if (isdigit(text[position])) {
            num += text[position];
        }
        else if (text[position] == '.') {
            if (!is_floating) {
                num += text[position];
                is_floating = true;
            }
            else {
                error("Invalid number");
            }
        }
        else {
            break;
        }

        position++;
    }

    position--; // Adjust position to not skip non-numeric characters

    if (is_floating) {
        return stod(num) * (is_neg ? -1 : 1);
    }
    else {
        return stoi(num) * (is_neg ? -1 : 1);
    }
}

std::string Tokenizer::get_word() {
    std::string output;
    while (position < text.size() && isalnum(text[position])) {
        output += text[position++];
    }
    return output;
}

void Tokenizer::skip_spaces() {
    while (position < text.size() && isspace(text[position])) {
        position++;
    }
}

void Tokenizer::handle_word(const std::string& word) {
    if (word == "let") {
        tokens.push_back(Token(LET, "LET"));
        skip_spaces();

        std::string var_name = get_word(); // Expect a variable name
        tokens.push_back(Token(VARIABLE, var_name));
        skip_spaces();
        if (position < text.size() && text[position] == '=') {
            position++; // Move past '='
            tokens.push_back(Token(ASSIGN, '='));
        }
        else {
            error("Expected '=' after variable name");
        }
    }
    else if (word == "print") {
        tokens.push_back(Token(PRINT, "print"));
    }
    else if (word == "if") {
        /* Handle if statements */
    }
    else {
        tokens.push_back(Token(VARIABLE, word)); // Handle it as a variable usage
    }
}

// Note: The following methods are placeholders for future implementation.
void Tokenizer::create_variable() {
    // Implement variable creation logic
}

void Tokenizer::run(std::vector<Token> tokens) {
    // Implement token execution logic
}
