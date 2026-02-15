cmake_minimum_required(VERSION 3.20)
project(DarkBall)

set(CMAKE_CXX_STANDARD 20)

add_executable(DarkBall
    main.cpp
    entities/Ball.cpp
    entities/Ball.h
    systems/PhysicsWorld.cpp
    systems/PhysicsWorld.h
 "systems/Ground.h" "systems/Ground.cpp")

source_group("entities" FILES
    entities/Ball.cpp
    entities/Ball.h
)

source_group("systems" FILES
    systems/PhysicsWorld.cpp
    systems/PhysicsWorld.h
)

# ---- SFML ----
set(SFML_DIR "D:/DEV_IMP/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

target_link_libraries(DarkBall
    sfml-graphics
    sfml-window
    sfml-system
)

# ---- Box2D ----
target_include_directories(DarkBall PRIVATE
    D:/DEV_IMP/box2d-2.4.1/include
)

target_link_libraries(DarkBall
    D:/DEV_IMP/box2d-2.4.1/build/bin/Debug/box2d.lib
)