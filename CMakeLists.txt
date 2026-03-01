cmake_minimum_required(VERSION 3.20)
project(DarkBall)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DarkBall
    main.cpp
    entities/Ball.cpp
    entities/Ball.h
    systems/PhysicsWorld.cpp
    systems/PhysicsWorld.h
    systems/Terrain.h
    systems/Terrain.cpp
    core/Constants.h
    systems/ContactListener.h
    systems/ContactListener.cpp
    systems/Level.h
    systems/Level.cpp
)

# ---- Copy Assets ----
file(GLOB GAME_ASSETS "assets/*")
file(COPY ${GAME_ASSETS}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

file(GLOB LEVEL_FILES "levels/*")
file(COPY ${LEVEL_FILES}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/levels)

add_custom_command(TARGET DarkBall POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DarkBall>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:DarkBall>/assets"
)

add_custom_command(TARGET DarkBall POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DarkBall>/levels"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/levels"
        "$<TARGET_FILE_DIR:DarkBall>/levels"
)

# ---- SFML ----
set(SFML_DIR "D:/DEV_IMP/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

target_link_libraries(DarkBall PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# ---- nlohmann JSON ----
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(DarkBall PRIVATE nlohmann_json::nlohmann_json)

# ---- Box2D ----
target_include_directories(DarkBall PRIVATE
    D:/DEV_IMP/box2d-2.4.1/include
)

target_link_libraries(DarkBall PRIVATE
    D:/DEV_IMP/box2d-2.4.1/build/bin/Debug/box2d.lib
)