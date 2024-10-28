cmake_minimum_required (VERSION 3.8)

add_executable (boids-imgui "main.cpp" "UseImGui.cpp" "UseImGui.h")
add_executable(boids main.cpp)
set(CMAKE_PREFIX_PATH "C:/Users/Timofey/vcpkg/packages/glad_x64-windows/share/glad")
set(CMAKE_PREFIX_PATH "C:/Users/Timofey/vcpkg/packages/glfw3_x64-windows/share/glfw3")
set(CMAKE_PREFIX_PATH "C:/Users/Timofey/vcpkg/packages/imgui_x64-windows/share/imgui")
set(VCPKG_ENABLE_MANIFEST TRUE)
set(CMAKE_TOOLCHAIN_FILE "C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(glad_DIR "C:/Users/Timofey/vcpkg/packages/glad_x64-windows/share/glad")
set(glfw3_DIR "C:/Users/Timofey/vcpkg/packages/glfw3_x64-windows/share/glfw3")




find_package(glad REQUIRED)
target_link_libraries(boids-imgui PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(boids-imgui PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(boids-imgui PRIVATE imgui::imgui)