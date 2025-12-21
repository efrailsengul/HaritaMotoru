cmake_minimum_required(VERSION 3.24)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5")

project(HaritaMotoru)

# --- C++ 17 Standardı ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Ayarlar ---
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs" FORCE)
set(SFML_STATIC_LIBRARIES ON CACHE BOOL "SFML Static" FORCE)
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "Use system deps" FORCE)

include(FetchContent)

# 1. SFML 2.6.1 (Stabil)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(sfml)

# Alias Düzeltmesi
if(NOT TARGET SFML::Graphics)
    add_library(SFML::Graphics ALIAS sfml-graphics)
    add_library(SFML::Window ALIAS sfml-window)
    add_library(SFML::System ALIAS sfml-system)
    add_library(SFML::Audio ALIAS sfml-audio)
    add_library(SFML::Network ALIAS sfml-network)
endif()

# 2. ImGui (STANDART SÜRÜM - DOCKING YOK)
# v1.90 sürümü SFML 2.6 ile tam uyumludur.
# Resmi etiket olduğu için indirme hatası vermez.
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.90
)
FetchContent_MakeAvailable(imgui)

# 3. ImGui-SFML v2.6
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG v2.6
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "Disable internal find_package" FORCE)

FetchContent_MakeAvailable(imgui-sfml)

# --- PROJE ---
add_executable(HaritaMotoru main.cpp)

target_link_libraries(HaritaMotoru
        PRIVATE
        SFML::Graphics
        SFML::Audio
        SFML::Network
        ImGui-SFML::ImGui-SFML
)