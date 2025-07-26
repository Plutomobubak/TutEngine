#pragma once

#include "../component.h"

#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    using LibHandle = HMODULE;
#else
    #include <dlfcn.h>
    using LibHandle = void*;
#endif

class TutScript {
  protected:
    unsigned int entity = 0;
public:
    virtual ~TutScript() = default;
    virtual void Init() {}
    virtual void Update(float dt) {}

    void SetEntity(unsigned int e) { 
        entity = e; 
        std::cout << "Set entity to " << entity << std::endl;
    }
    unsigned int GetEntity() const { return entity; }
};

class Script : public Component {
public:
    std::string scriptName;

    unsigned int ownerEntity = 0;
    TutScript* instance = nullptr;
    LibHandle libHandle = nullptr;

    using CreateFunc = TutScript* (*)();

    void applyProperties(const Properties& props) override {
        if (props.find("src") != props.end()) {
            scriptName = std::get<std::string>(props.at("src"));
        }
    }
    void setEntity(unsigned int e) {
        ownerEntity = e;
    }

    void init() override {
        if (scriptName.empty()) return;

        std::string libPath =
#ifdef _WIN32
            "bin/scripts/" + scriptName + ".dll";
#else
            "bin/scripts/" + scriptName + ".so";
#endif

#ifdef _WIN32
        libHandle = LoadLibraryA(libPath.c_str());
#else
        libHandle = dlopen(libPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
#endif
        if (!libHandle) {
            std::cerr << "Failed to load script: " << libPath << "\n";
            return;
        }

#ifdef _WIN32
        CreateFunc create = (CreateFunc)GetProcAddress(libHandle, "CreateScript");
#else
        CreateFunc create = (CreateFunc)dlsym(libHandle, "CreateScript");
#endif

        if (!create) {
            std::cerr << "CreateScript not found in " << libPath << "\n";
            return;
        }

        instance = create();
        if (instance) {
          instance->SetEntity(ownerEntity);
          instance->Init();
        }
    }

    void update(float dt) override {
        if (instance) instance->Update(dt);
    }

    ~Script() {
        delete instance;
#ifdef _WIN32
        if (libHandle) FreeLibrary(libHandle);
#else
        if (libHandle) dlclose(libHandle);
#endif
    }
};
