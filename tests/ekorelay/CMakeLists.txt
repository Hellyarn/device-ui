cmake_minimum_required(VERSION 3.20)
project(ekorelay_preview LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
if(NOT EXISTS "${LVGL_ROOT}/lvgl.h")
  message(FATAL_ERROR "Pass -DLVGL_ROOT=/path/to/lvgl-9.3.0")
endif()
get_filename_component(UI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
file(READ "${UI_ROOT}/include/lv_conf.h" LV_CONFIG)
string(REPLACE "(RAM_SIZE * 1024U)" "(16 * 1024 * 1024U)" LV_CONFIG "${LV_CONFIG}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/lv_conf.h" "${LV_CONFIG}")
file(GLOB_RECURSE LV_SOURCES CONFIGURE_DEPENDS "${LVGL_ROOT}/src/*.c")
file(GLOB UI_SOURCES CONFIGURE_DEPENDS "${UI_ROOT}/generated/ui_320x240/*.c")
add_executable(ekorelay_preview preview.cpp ${LV_SOURCES} ${UI_SOURCES}
  "${UI_ROOT}/locale/lv_i18n.c"
  "${UI_ROOT}/source/graphics/TFT/EkoRelayImages.c"
  "${UI_ROOT}/source/graphics/TFT/EkoRelayBoot.cpp"
  "${UI_ROOT}/source/graphics/TFT/EkoRelayHome.cpp")
target_compile_definitions(ekorelay_preview PRIVATE HAS_TFT=1 VIEW_320x240 LV_CONF_INCLUDE_SIMPLE LV_LVGL_H_INCLUDE_SIMPLE)
target_include_directories(ekorelay_preview PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${LVGL_ROOT}" "${UI_ROOT}/include" "${UI_ROOT}/locale" "${UI_ROOT}/generated/ui_320x240")
if(UNIX)
  target_link_libraries(ekorelay_preview PRIVATE m)
endif()
enable_testing()
add_test(NAME ekorelay_ui COMMAND ekorelay_preview "${CMAKE_CURRENT_BINARY_DIR}")
