# AI-assisted: Zephyr/ZMK CMake build integration for Z-Lander
if(CONFIG_Z_LANDER)
  zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ship.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/terrain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/zmk/render.c
  )
endif()
