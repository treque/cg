cmake_minimum_required(VERSION 3.12)
project(TP3)

set(HEADER_FILES
    Couleur.h
    Cst.h
    HighResolutionTimer.h
    Intersection.h
    ISurface.h
    Lumiere.h
    MathUtils.h
    Matrice4.h
    NuanceurProg.h
    Plan.h
    Quadrique.h
    Rayon.h
    Scene.h
    Singleton.h
    StringUtils.h
    textfile.cpp
    textfile.h
    Triangle.h
    Var.h
    Vecteur3.h
)

set(SOURCE_FILES
    Couleur.cpp
    Intersection.cpp
    ISurface.cpp
    Lumiere.cpp
    main.cpp
    Matrice4.cpp
    NuanceurProg.cpp
    Plan.cpp
    Quadrique.cpp
    Rayon.cpp
    Scene.cpp
    StringUtils.cpp
    Triangle.cpp
    Var.cpp
    Vecteur3.cpp
)

file(GLOB SCENE_FILES
    Scenes/*.dat
    )

file(GLOB SHADER_FILES
    Nuanceurs/*.glsl
)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES} ${SCENE_FILES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Weverything)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-c++98-compat -Wno-c++98-compat-pedantic)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-padded)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-global-constructors -Wno-exit-time-destructors)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-documentation)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-source-uses-openmp)
endif()

if (USE_OPENMP)
    target_compile_options(${PROJECT_NAME} PRIVATE -fopenmp)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fopenmp")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
endif()

foreach(item ${SHADER_FILES})
    get_filename_component(FILE_NAME ${item} NAME)
    set(DEST "${PROJECT_BINARY_DIR}/Nuanceurs/${FILE_NAME}")
    add_custom_command(
        OUTPUT ${DEST}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/Nuanceurs/"
        COMMAND ${CMAKE_COMMAND} -E copy ${item} ${DEST}
        DEPENDS ${item}
    )
    list(APPEND SHADER_DEST_FILES ${DEST})
endforeach()

add_custom_target(
    Nuanceurs ALL
    DEPENDS ${SHADER_DEST_FILES}
    )
