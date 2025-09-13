find_package (SDL3 REQUIRED)
find_package (SDL3_image REQUIRED)

add_executable (4_in_a_line "4_in_a_line.cpp" "4_in_a_line.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 4_in_a_line PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries (4_in_a_line PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)