set(ARGOS3_HEADERS_PLUGINS_ROBOTS_ARENA_SIMULATOR
  simulator/arena_entity.h
  simulator/wall_entity.h)

# Install location for the arena simulator headers
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ARENA_SIMULATOR}
  DESTINATION include/argos3/plugins/robots/arena/simulator)

set(ARGOS3_SOURCES_PLUGINS_ROBOTS_ARENA_SIMULATOR
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ARENA_SIMULATOR}
  simulator/arena_entity.cpp
  simulator/wall_entity.cpp)

add_library(argos3plugin_${ARGOS_BUILD_FOR}_arena SHARED
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_ARENA_SIMULATOR})

target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_arena
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d
  argos3plugin_${ARGOS_BUILD_FOR}_entities)

# Install location for the arena plugin
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_arena
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
