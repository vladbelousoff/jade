cmake_minimum_required(VERSION 3.29)
project(jade)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(JADE_ENABLE_TESTS "Enable Catch2" ON)
option(JADE_ENABLE_EXAMPLES "Enable examples" ON)
option(JADE_ENABLE_LIBASSERT "Enable Libassert" ON)

file(GLOB_RECURSE JADE_SOURCE_FILES "jade/*.cpp")
file(GLOB_RECURSE JADE_HEADER_FILES "jade/*.hpp")

add_library(${PROJECT_NAME} STATIC ${JADE_SOURCE_FILES} ${JADE_HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

include(FetchContent)

FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.9
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
        gl3w
        GIT_REPOSITORY https://github.com/vladbelousoff/gl3w.git
        GIT_TAG 1.0.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gl3w)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

target_compile_definitions(${PROJECT_NAME} PUBLIC GLM_FORCE_LEFT_HANDED GLM_FORCE_DEPTH_ZERO_TO_ONE)

set(
        EXTERNAL_LIBRARIES
        SDL2::SDL2-static
        SDL2::SDL2main
        gl3w
        spdlog
        glm::glm
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTERNAL_LIBRARIES})

if (JADE_ENABLE_LIBASSERT)
    FetchContent_Declare(
            libassert
            GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
            GIT_TAG v2.1.2
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(libassert)

    target_link_libraries(${PROJECT_NAME} PUBLIC libassert::assert)
    target_compile_definitions(${PROJECT_NAME} PUBLIC JADE_ENABLE_LIBASSERT)
endif ()

find_library(D3D11_LIBRARY d3d11)

if (D3D11_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${D3D11_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PUBLIC JADE_D3D11_SUPPORT)
endif ()

find_library(D3D_COMPILER d3dcompiler)
if (D3D11_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${D3D_COMPILER})
endif ()

if (JADE_ENABLE_TESTS)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    target_include_directories(Catch2 INTERFACE "${Catch2_SOURCE_DIR}/src")

    # Find all test source files
    file(GLOB TEST_SOURCES "tests/*.cpp")

    # Loop through each test source file and create an executable
    foreach (TEST_FILE ${TEST_SOURCES})
        # Get the filename without directory or extension
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        # Create an executable for the test file
        add_executable(${PROJECT_NAME}-test-${TEST_NAME} ${TEST_FILE})

        # Link the executable to the project library and Catch2
        target_link_libraries(${PROJECT_NAME}-test-${TEST_NAME}
                ${PROJECT_NAME}
                Catch2::Catch2WithMain
        )
    endforeach ()
endif ()

if (JADE_ENABLE_EXAMPLES)
    # Find all HLSL files in the examples directory
    file(GLOB_RECURSE HLSL_FILES "examples/*/*.hlsl")

    # Set the build directory for SPIR-V files
    set(SPV_OUTPUT_DIR "${CMAKE_BINARY_DIR}/examples")

    # Clear list to store SPIR-V outputs
    set(SPV_FILES)

    foreach (HLSL_FILE ${HLSL_FILES})
        # Get the relative path of the HLSL file
        file(RELATIVE_PATH RELATIVE_PATH_TO_SOURCE "${CMAKE_SOURCE_DIR}/examples" ${HLSL_FILE})

        # Replace directory separators with underscores to ensure unique output names
        string(REPLACE "/" "_" SAFE_FILE_NAME "${RELATIVE_PATH_TO_SOURCE}")
        string(REPLACE "\\" "_" SAFE_FILE_NAME "${SAFE_FILE_NAME}")

        # Determine shader stage based on extension
        if (HLSL_FILE MATCHES "\\.vert\\.hlsl$")
            set(SHADER_STAGE "vertex")
        elseif (HLSL_FILE MATCHES "\\.frag\\.hlsl$")
            set(SHADER_STAGE "fragment")
        elseif (HLSL_FILE MATCHES "\\.comp\\.hlsl$")
            set(SHADER_STAGE "compute")
        elseif (HLSL_FILE MATCHES "\\.geom\\.hlsl$")
            set(SHADER_STAGE "geometry")
        elseif (HLSL_FILE MATCHES "\\.tesc\\.hlsl$")
            set(SHADER_STAGE "tesscontrol")
        elseif (HLSL_FILE MATCHES "\\.tese\\.hlsl$")
            set(SHADER_STAGE "tessevaluation")
        else ()
            message(FATAL_ERROR "Unknown shader stage for file: ${HLSL_FILE}")
        endif ()

        # Determine the SPIR-V output file path (unique name)
        set(SPV_FILE "${SPV_OUTPUT_DIR}/${SAFE_FILE_NAME}.spv")

        # Determine the copied HLSL output file path
        set(COPIED_HLSL_FILE "${SPV_OUTPUT_DIR}/${SAFE_FILE_NAME}")

        # Add a custom command to compile the HLSL file to SPIR-V and copy the HLSL file
        add_custom_command(
                OUTPUT ${SPV_FILE} ${COPIED_HLSL_FILE}
                COMMAND glslc -fshader-stage=${SHADER_STAGE} ${HLSL_FILE} -o ${SPV_FILE}
                COMMAND ${CMAKE_COMMAND} -E copy ${HLSL_FILE} ${COPIED_HLSL_FILE}
                DEPENDS ${HLSL_FILE}
                COMMENT "Compiling ${HLSL_FILE} to SPIR-V: ${SPV_FILE} and copying to ${COPIED_HLSL_FILE}"
        )

        # Ensure the SPIR-V and HLSL files are part of the build system
        list(APPEND SPV_FILES ${SPV_FILE})
        list(APPEND HLSL_OUTPUT_FILES ${COPIED_HLSL_FILE})
    endforeach ()

    # Add a custom target to compile all SPIR-V and copy HLSL files
    add_custom_target(compile_shaders ALL DEPENDS ${SPV_FILES} ${HLSL_OUTPUT_FILES})

    # Add example executables
    file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
    foreach (EXAMPLE_FILE ${EXAMPLE_SOURCES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)
        add_executable(${PROJECT_NAME}-example-${EXAMPLE_NAME} ${EXAMPLE_FILE})
        target_link_libraries(${PROJECT_NAME}-example-${EXAMPLE_NAME} ${PROJECT_NAME})
        add_dependencies(${PROJECT_NAME}-example-${EXAMPLE_NAME} compile_shaders)
    endforeach ()
endif ()

