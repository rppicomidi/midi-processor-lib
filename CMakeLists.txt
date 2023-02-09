cmake_minimum_required(VERSION 3.13)

add_library(midi_processor_lib INTERFACE)
set(EMBEDDED_CLI_PATH ${CMAKE_CURRENT_LIST_DIR}/../../ext_lib/embedded-cli/lib/)
set(MONO_GRAPHICS_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-ssd1306-mono-graphics-lib/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../ext_lib/littlefs-lib lib/littlefs-lib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../rp2040_rtc lib/rp2040_rtc)
target_sources(midi_processor_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_chan_mes_remap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_chan_mes_remap_settings_view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_home_screen.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_mc_fader_pickup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_mc_fader_pickup_settings_view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_setup_screen.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_transpose.cpp
    ${CMAKE_CURRENT_LIST_DIR}/midi_processor_transpose_view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/preset_view.cpp
    ${CMAKE_CURRENT_LIST_DIR}/settings_file.cpp
    ${CMAKE_CURRENT_LIST_DIR}/settings_flash_view.cpp
    ${EMBEDDED_CLI_PATH}/src/embedded_cli.c
)
target_include_directories(midi_processor_lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${EMBEDDED_CLI_PATH}/include
    ${MONO_GRAPHICS_PATH}
)
target_link_libraries(midi_processor_lib INTERFACE mono_graphics_lib rp2040_rtc littlefs-lib pico_sync ui_text_item_chooser pico_stdlib)
