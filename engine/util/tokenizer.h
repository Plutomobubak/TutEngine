#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <cctype>

enum class TokenType {
    Keyword,
    Identifier,
    Number,
    String,
    Bool,
    Symbol,
    EndOfFile,
};

struct Token {
    TokenType type;
    std::string text;
};

class Tokenizer {
    const std::string& input;
    size_t pos = 0;

public:
    Tokenizer(const std::string& str) : input(str) {}

    char peek() const {
        return pos < input.size() ? input[pos] : '\0';
    }

    char get() {
        return pos < input.size() ? input[pos++] : '\0';
    }

    void skipWhitespace() {
        while (std::isspace(peek())) get();
    }

    bool isKeyword(const std::string& s) {
        return s == "scene" || s == "use" || s == "entity";
    }

    Token nextToken() {
        skipWhitespace();
        if (pos >= input.size()) return {TokenType::EndOfFile, ""};

        char c = peek();

        // Symbols: {}[]();=,:
        if (c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')' || 
            c == ';' || c == '=' || c == ',' || c == ':') {
            pos++;
            return {TokenType::Symbol, std::string(1, c)};
        }

        // Number (simple float/int)
        if (std::isdigit(c) || c == '-' || c == '+') {
            std::string num;
            bool dotSeen = false;
            while (std::isdigit(peek()) || peek() == '.' || peek() == '-' || peek() == '+') {
                if (peek() == '.') {
                    if (dotSeen) break;
                    dotSeen = true;
                }
                num += get();
            }
            return {TokenType::Number, num};
        }

        // String literal
        if (c == '"') {
            get(); // consume the opening quote
            std::string str;
            while (peek() != '"' && peek() != '\0') {
                str += get();
            }
            if (peek() == '"') {
                get(); // consume the closing quote
            } else {
                std::cerr << "Unterminated string literal\n";
            }
            return {TokenType::String, str};
        }

        // Identifier or keyword
        if (std::isalpha(c) || c == '_') {
            std::string id;
            while (std::isalnum(peek()) || peek() == '_') id += get();
            if (id == "true" || id == "false") return {TokenType::Bool, id};
            if (isKeyword(id))
                return {TokenType::Keyword, id};
            else
                return {TokenType::Identifier, id};
        }

        // Unexpected char
        std::cerr << "Unexpected char: '" << c << "'\n";
        pos++;
        return nextToken();
    }
};

#endif // TOKENIZER_H
