#pragma once
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <vector>

#ifdef _WIN32
const std::string EXT = ".dll";
#else
const std::string EXT = ".so";
#endif

class ScriptCompiler {
public:
    static void compileAllScripts(const std::string& scriptDir, const std::string& outputDir) {
        namespace fs = std::filesystem;

        fs::create_directories(outputDir);

        for (const auto& entry : fs::directory_iterator(scriptDir)) {
            if (entry.path().extension() == ".cpp") {
                compileScript(entry.path(), outputDir);
            }
        }
    }

private:
    static void compileScript(const std::filesystem::path& srcPath, const std::string& outputDir) {
        std::string scriptName = srcPath.stem().string(); // without extension

        std::string outputPath = outputDir + "/" + scriptName + EXT;

        std::string command;

#ifdef _WIN32
        // Windows (MinGW assumed)
        command = "g++ -std=c++17 -shared -o \"" + outputPath + "\" \"" + srcPath.string() +
                  "\" -Iengine/core/components -Iengine/core/systems -Iengine/core -Iexternal/glad/include -Iexternal/glm -Iexternal/imgui -Iexternal/imgui/backends -Wno-nonnull -D_WIN32";
#else
        // Linux/macOS
        command = "g++ -std=c++17 -fPIC -shared -o \"" + outputPath + "\" \"" + srcPath.string() +
                  "\" -Iengine/core/components -Iengine/core/systems -Iengine/core -Iexternal/glad/include -Iexternal/glm -Iexternal/imgui -Iexternal/imgui/backends";
#endif

        std::cout << "Compiling: " << srcPath.filename() << " → " << outputPath << "\n";

        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << "Compilation failed for " << srcPath.filename() << "\n";
        }
    }
};
