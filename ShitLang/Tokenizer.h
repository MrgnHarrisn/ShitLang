#pragma once

#include <vector>
#include <iostream>
#include <variant>
#include <cmath>
#include <map>

#include "Token.h"

class Tokenizer
{
public:
	Tokenizer(const std::string& text);
	Tokenizer(const std::string& text, std::map<std::string, double>* map);

	std::vector<Token> tokenize();
	void run(std::vector<Token> tokens);
	std::map<std::string, double>* get_variables() {
		return variables;
	}
	
private:

	void error(const std::string& message);

	bool is_whole_number(double number, double epsilon = 1e-8) {
		return floor(number) == number;
		/*return std::fabs(number - std::round(number)) < epsilon;*/
	}

	// void tokenize_operators();
	
	std::variant<int, double> get_number(bool is_neg);

	std::string get_word();
	void handle_word(const std::string& word);
	void create_variable();
	void skip_spaces();

	std::map<std::string, double>* variables;
	std::vector<Token> tokens;
	std::string text = "";

	size_t position = 0;
	char current_char;
	Token current_token;
};