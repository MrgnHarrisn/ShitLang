#pragma once

#include <string>
#include <sstream>
#include <any>

enum TokenType
{
	INTEGER,
	PLUS,
	MINUS,
	MULT,
	DIVIDE,
	EXPONENT,
	LPAREN,
	RPAREN,
	FLOAT,
	LET,
	VARIABLE,
	ASSIGN,
	UNARY_OP,
	LESS_THAN,
	GREATER_THAN,
	PRINT,
	EoF,
};

/* Exclusively for debugging */
inline std::string type_to_str(TokenType type)
{
	switch (type)
	{
	case INTEGER:
		return "INTEGER";
	case FLOAT:
		return "FLOAT";
	case PLUS:
		return "PLUS";
	case EoF:
		return "EoF";
	case MINUS:
		return "MINUS";
	case MULT:
		return "MULT";
	case DIVIDE:
		return "DIVIDE";
	case EXPONENT:
		return "EXPONENT";
	case LPAREN:
		return "LPAREN";
	case RPAREN:
		return "RPAREN";
	case LET:
		return "LET";
	case LESS_THAN:
		return "LESS_THAN";
	case GREATER_THAN:
		return "GREATER_THAN";
	case VARIABLE:
		return "VARIABLE";
	case ASSIGN:
		return "ASSIGN";
	case UNARY_OP:
		return "UNARY_OP";
	case PRINT:
		return "PRINT";
	default:
		return "NONE";
	}
}

class Token
{
public:

	Token() {};

	Token(TokenType type, std::any value) : m_value(value), m_type(type) {};
	std::any get_value() {
		return m_value;
	}
	TokenType get_type() {
		return m_type;
	}

	void set_type(TokenType type) {
		m_type = type;
	}
	void set_value(std::any value) {
		m_value = value;
	}

	std::string any_to_str(const std::any& a) const {
		std::ostringstream os;
		
		if (a.type() == typeid(int)) {
			os << std::any_cast<int>(a);
		}
		else if (a.type() == typeid(float)) {
			os << std::any_cast<float>(a);
		}
		else if (a.type() == typeid(double)) {
			os << std::any_cast<double>(a);
		}
		else if (a.type() == typeid(std::string)) {
			os << std::any_cast<std::string>(a);
		}
		else if (!a.has_value()) {
			os << "null";
		}
		else if (a.type() == typeid(char)) {
			os << "char";
		}
		else {
			os << "Unsupported type";
		}

		return os.str();
	}

	std::string to_str() const {
		std::ostringstream os;
		os << type_to_str(m_type) << " | " << any_to_str(m_value);
		return os.str();
	}
private:

	std::any m_value;
	TokenType m_type;
};