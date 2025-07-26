#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "../core/component.h"
#include "tokenizer.h"  // tokenizer should have its own header!
#include "utils.h"

// Forward declare Variant type here if needed.

struct PComponent {
    std::string name;
    std::unordered_map<std::string, Variant> properties;
};

struct PEntity {
    std::string name;
    std::vector<PComponent> components;
};

struct Scene {
    std::vector<PEntity> entities;
};

// Declare external variable here:
extern std::unordered_map<std::string, PEntity> prefabs;


class Parser {
public:
    Parser(const std::string& str);
    Scene parseScene();
    void parsePrefabEntities();

private:
    // put private members and methods here, just declarations
    Tokenizer tokenizer;
    Token curToken;

    void next();
    bool expect(TokenType type, const std::string& text = "");
    void consume(TokenType type, const std::string& text = "");
    glm::vec3 parseVec3();
    PComponent parseComponent();
    PEntity parseEntity();
    PEntity parseUse();
};

#endif
