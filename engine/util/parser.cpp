#include <algorithm>
#include <iostream>
#include <filesystem>
#include <variant>
#include <sstream>
#include <fstream>
#include <cctype>
#include <optional>
#include "./parser.h"

std::unordered_map<std::string, PEntity> prefabs;

Parser::Parser(const std::string& str) : tokenizer(str) {
        next();
}
// Parser class member functions

void Parser::next() {
    curToken = tokenizer.nextToken();
}

bool Parser::expect(TokenType type, const std::string& text) {
    if (curToken.type == type && (text.empty() || curToken.text == text)) {
        return true;
    }
    std::cerr << "Expected " << (text.empty() ? "token type" : "token '" + text + "'") 
              << " but got '" << curToken.text << "'\n";
    return false;
}

void Parser::consume(TokenType type, const std::string& text) {
    if (!expect(type, text)) exit(1);
    next();
}

glm::vec3 Parser::parseVec3() {
    consume(TokenType::Symbol, "[");
    
    float x = std::stof(curToken.text);
    next();
    consume(TokenType::Symbol, ",");

    float y = std::stof(curToken.text);
    next();
    consume(TokenType::Symbol, ",");

    float z = std::stof(curToken.text);
    next();
    
    consume(TokenType::Symbol, "]");

    return glm::vec3(x, y, z);
}

PComponent Parser::parseComponent() {
    PComponent comp;

    if (!expect(TokenType::Identifier)) exit(1);
    comp.name = curToken.text;
    next();

    if (curToken.type == TokenType::Symbol && curToken.text == "(") {
        next();
        if (curToken.type == TokenType::Symbol && curToken.text == ")") {
            next(); // empty component
        } else {
            while (true) {
                if (!expect(TokenType::Identifier)) exit(1);
                std::string propName = curToken.text;
                next();
                consume(TokenType::Symbol, ":");

                Variant val;

                if (curToken.type == TokenType::Symbol && curToken.text == "[") {
                    val = parseVec3();
                } else if (curToken.type == TokenType::Number) {
                    if (curToken.text.find('.') != std::string::npos) {
                        val = std::stof(curToken.text);
                    } else {
                        val = std::stoi(curToken.text);
                    }
                    next();
                } else if (curToken.type == TokenType::String) {
                    val = curToken.text;
                    next();
                } else if (curToken.type == TokenType::Bool){
                    val = curToken.text == "true"? true:false;
                    next();
                
                } else {
                    std::cerr << "Unexpected value type in component property: " << (curToken.type == TokenType::Identifier? "kw":"other") << " : " << curToken.text << "\n";
                    exit(1);
                }

                comp.properties[propName] = val;

                if (curToken.type == TokenType::Symbol && curToken.text == ",") {
                    next();
                    continue;
                } else if (curToken.type == TokenType::Symbol && curToken.text == ")") {
                    next();
                    break;
                } else {
                    std::cerr << "Unexpected token in component properties: " << curToken.text << "\n";
                    exit(1);
                }
            }
        }
        consume(TokenType::Symbol, ";");

    } else if (curToken.type == TokenType::Symbol && curToken.text == ";") {
        next(); // simple component with no properties

    } else if (curToken.type == TokenType::Symbol && curToken.text == ".") {
        next();
        if (!expect(TokenType::Identifier)) exit(1);
        std::string propName = curToken.text;
        next();
        consume(TokenType::Symbol, "=");
        Variant val = parseVec3();
        comp.name = "Transform"; // special case
        comp.properties[propName] = val;
        consume(TokenType::Symbol, ";");

    } else {
        std::cerr << "Unexpected token after component name: " << curToken.text << "\n";
        exit(1);
    }

    return comp;
}

PEntity Parser::parseEntity() {
    PEntity e;
    if (!expect(TokenType::Keyword, "entity")) exit(1);
    next();
    if (!expect(TokenType::Identifier)) exit(1);
    e.name = curToken.text;
    next();
    consume(TokenType::Symbol, "{");

    while (!(curToken.type == TokenType::Symbol && curToken.text == "}")) {
        e.components.push_back(parseComponent());
    }

    consume(TokenType::Symbol, "}");
    return e;
}

PEntity Parser::parseUse() {
    PEntity e;
    if (!expect(TokenType::Keyword, "use")) exit(1);
    next();
    if (!expect(TokenType::Identifier)) exit(1);
    std::string pref = curToken.text;
    if (prefabs.find(pref) != prefabs.end())
        e = prefabs[pref];
    else
        std::cerr << "Could find prefab" << pref << std::endl;

    next();
    consume(TokenType::Symbol, "{");

    while (!(curToken.type == TokenType::Symbol && curToken.text == "}")) {
        PComponent overrideComp = parseComponent();

        auto it = std::find_if(e.components.begin(), e.components.end(), [&](const PComponent& c) {
            return c.name == overrideComp.name;
        });

        if (it != e.components.end()) {
            for (const auto& [key, val] : overrideComp.properties) {
                it->properties[key] = val;
            }
        } else {
            e.components.push_back(std::move(overrideComp));
        }
    }
    consume(TokenType::Symbol, "}");
    return e;
}

Scene Parser::parseScene() {
    Scene scene;
    if (!expect(TokenType::Keyword, "scene")) {
        std::cerr << "Scene must start with 'scene'\n";
        exit(1);
    }
    next();
    consume(TokenType::Symbol, "{");

    while (!(curToken.type == TokenType::Symbol && curToken.text == "}")) {
        if (curToken.type == TokenType::Keyword && curToken.text == "use") {
            scene.entities.push_back(parseUse());
        } else if (curToken.type == TokenType::Keyword && curToken.text == "entity") {
            scene.entities.push_back(parseEntity());
        } else {
            std::cerr << "Unexpected token in scene: " << curToken.text << "\n";
            exit(1);
        }
    }

    consume(TokenType::Symbol, "}");
    return scene;
}

void Parser::parsePrefabEntities() {
    while (curToken.type != TokenType::EndOfFile) {
        PEntity entity;

        if (curToken.type == TokenType::Keyword && curToken.text == "use") {
            entity = parseUse();
        } else if (curToken.type == TokenType::Keyword && curToken.text == "entity") {
            entity = parseEntity();
        } else {
            std::cerr << "Unexpected token at top level: " << curToken.text << "\n";
            exit(1);
        }

        if (entity.name.empty()) {
            std::cerr << "Entity missing name, cannot store in prefab map.\n";
            exit(1);
        }

        prefabs[entity.name] = std::move(entity);
    }
}
