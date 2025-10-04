✅ PHASE 1: Setup & Rendering

    Target: Static 3D scene, camera movement, debug UI

    ✅ Draw triangle -> cube -> load model (e.g., glTF)

    ✅ Implement basic camera (free-fly or orbit)

    ✅ Add shader abstraction system

    ✅ Add simple lighting (Phong or Blinn-Phong)

    ✅ Use Dear ImGui for debug interface

🟡 PHASE 2: Core Systems

    Target: Game loop, input, assets, scene graph

    ✅ Game loop with fixed update / variable render

    ✅ Input abstraction (keyboard + mouse)

    ✅ Resource manager (shaders, textures, models)

    ✅ Scene graph: transform hierarchy

    ✅ Entity-Component-System (ECS) base (optional)
    
    ECS plus pro, Animations

🟠 PHASE 3: Gameplay Foundation

    Target: Playable 3rd-person character in simple level

    ✅ Third-person camera + player controller

    ✅ Collision system
        - AABB, groups

     Level loading from models (e.g., glTF maps)

     Basic animation system (skeleton, interpolation)

     Combat system v1 (light attack, hit detection)

     Sound engine integration (e.g., OpenAL or FMOD)

🔴 PHASE 4: Visual Polish & Style

    Target: DMC-like feel and presentation

     Post-processing effects (bloom, tone mapping)

     VFX system (particles, trails, etc.)

     Combo system (attack chains)

     Enemy AI (state machines)

     HUD, style rank (SSS, etc.)

🟣 PHASE 5: Tools + Performance

    Target: Polish + internal tools

    ✅ In-game debug tools (ImGui scene viewer, profiler)

     Level editor (basic placement)

     Asset pipeline automation (optimize mesh, textures)

     Framerate + GPU memory debugging
