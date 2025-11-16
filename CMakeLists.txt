cmake_minimum_required(VERSION 3.15)
project(gaia VERSION 0.1.0)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# -------------------------------------------------------
# Global static build
# -------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# -------------------------------------------------------
# OpenGL
# -------------------------------------------------------
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

# -------------------------------------------------------
# Assimp configuration
# -------------------------------------------------------
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# -------------------------------------------------------
# Dependencies
# -------------------------------------------------------
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/glew/build/cmake)

include_directories(
    thirdparty/glfw/include 
    thirdparty/glm 
    thirdparty/glew/include 
    thirdparty/assimp/include
    thirdparty/physx/include
    thirdparty/openexr/include/Imath
    thirdparty/openexr/include/OpenEXR
    irrklang/include
    ${OPENGL_INCLUDE_DIRS}
)

# -------------------------------------------------------
# Source files
# -------------------------------------------------------
file(GLOB_RECURSE WOLF_SOURCE wolf/*.cpp wolf/*.c)
file(GLOB_RECURSE WOLF_HEADER wolf/*.hpp wolf/*.h)
file(GLOB_RECURSE TINYXML_SOURCE tinyxml2/*.cpp tinyxml2/*.c)
file(GLOB_RECURSE TINYXML_HEADER tinyxml2/*.hpp tinyxml2/*.h)
file(GLOB_RECURSE GAIA_SOURCE src/*.cpp src/*.c)
file(GLOB_RECURSE GAIA_HEADER src/*.hpp src/*.h)

# -------------------------------------------------------
# Executable
# -------------------------------------------------------
add_executable(gaia
    ${WOLF_SOURCE} ${WOLF_HEADER}
    ${TINYXML_SOURCE} ${TINYXML_HEADER}
    ${GAIA_SOURCE} ${GAIA_HEADER}
)

target_sources(gaia PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/gaia.rc)

# -------------------------------------------------------
# PhysX
# -------------------------------------------------------
set(PHYSX_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/physx/bin/win.x86_64.vc143.mt")

target_link_libraries(gaia PRIVATE
    optimized ${PHYSX_ROOT}/release/PhysX_64.lib
    optimized ${PHYSX_ROOT}/release/PhysXCommon_64.lib
    optimized ${PHYSX_ROOT}/release/PhysXFoundation_64.lib
    optimized ${PHYSX_ROOT}/release/PhysXCooking_64.lib
    optimized ${PHYSX_ROOT}/release/PhysXCharacterKinematic_static_64.lib
    optimized ${PHYSX_ROOT}/release/PhysXExtensions_static_64.lib
    optimized ${PHYSX_ROOT}/release/PhysXPvdSDK_static_64.lib
    optimized ${PHYSX_ROOT}/release/PVDRuntime_64.lib

    debug ${PHYSX_ROOT}/debug/PhysX_64.lib
    debug ${PHYSX_ROOT}/debug/PhysXCommon_64.lib
    debug ${PHYSX_ROOT}/debug/PhysXFoundation_64.lib
    debug ${PHYSX_ROOT}/debug/PhysXCooking_64.lib
    debug ${PHYSX_ROOT}/debug/PhysXCharacterKinematic_static_64.lib
    debug ${PHYSX_ROOT}/debug/PhysXExtensions_static_64.lib
    debug ${PHYSX_ROOT}/debug/PhysXPvdSDK_static_64.lib
    debug ${PHYSX_ROOT}/debug/PVDRuntime_64.lib
)

# -------------------------------------------------------
# OpenEXR
# -------------------------------------------------------
set(OPENEXR_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/openexr")

target_link_libraries(gaia PRIVATE
    optimized ${OPENEXR_ROOT}/lib/OpenEXR-3_4.lib
    optimized ${OPENEXR_ROOT}/lib/OpenEXRCore-3_4.lib
    optimized ${OPENEXR_ROOT}/lib/OpenEXRUtil-3_4.lib
    optimized ${OPENEXR_ROOT}/lib/Imath-3_2.lib
    optimized ${OPENEXR_ROOT}/lib/Iex-3_4.lib
    optimized ${OPENEXR_ROOT}/lib/IlmThread-3_4.lib
    optimized ${OPENEXR_ROOT}/lib/openjph.0.24.lib

    debug ${OPENEXR_ROOT}/lib/OpenEXR-3_4_d.lib
    debug ${OPENEXR_ROOT}/lib/OpenEXRCore-3_4_d.lib
    debug ${OPENEXR_ROOT}/lib/OpenEXRUtil-3_4_d.lib
    debug ${OPENEXR_ROOT}/lib/Imath-3_2_d.lib
    debug ${OPENEXR_ROOT}/lib/Iex-3_4_d.lib
    debug ${OPENEXR_ROOT}/lib/IlmThread-3_4_d.lib
    debug ${OPENEXR_ROOT}/lib/openjph.0.24_d.lib
)

# -------------------------------------------------------
# Core linking
# -------------------------------------------------------
target_link_libraries(gaia PRIVATE
    assimp
    glfw
    glew
    ${OPENGL_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/irrklang/lib/Winx64-visualStudio/irrKlang.lib
)

# -------------------------------------------------------
# Output
# -------------------------------------------------------
set_target_properties(gaia PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# -------------------------------------------------------
# DLL copying
# -------------------------------------------------------
add_custom_command(TARGET gaia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/thirdparty/glew/build/bin/$<IF:$<CONFIG:Debug>,Debug,Release>/glew32$<IF:$<CONFIG:Debug>,d,>.dll"
        "$<TARGET_FILE_DIR:gaia>"
)

add_custom_command(TARGET gaia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/irrklang/bin/winx64-visualStudio/irrKlang.dll"
        "$<TARGET_FILE_DIR:gaia>"
)

add_custom_command(TARGET gaia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PHYSX_ROOT}/$<CONFIG>"
        "$<TARGET_FILE_DIR:gaia>"
)

add_custom_command(TARGET gaia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OPENEXR_ROOT}/bin"
        "$<TARGET_FILE_DIR:gaia>"
)

# -------------------------------------------------------
# Move data folder
# -------------------------------------------------------
add_custom_command(TARGET gaia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:gaia>/data"
)
