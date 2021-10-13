cmake_minimum_required(VERSION 3.5)
project(nav_layer_from_points)

set(CMAKE_CXX_STANDARD 17)

set(lib_name ${PROJECT_NAME}_core)

add_compile_options(-Wall -O3 -Werror)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_costmap_2d REQUIRED)
find_package(pluginlib REQUIRED)

include_directories(include)

add_library(${lib_name} SHARED
  src/costmap_layer.cpp
)

ament_target_dependencies(${lib_name}
  rclcpp
  geometry_msgs
  nav2_costmap_2d
  tf2_ros
  pluginlib
)

install(TARGETS ${lib_name}
  DESTINATION lib

)

install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

install(FILES costmap_plugins.xml
  DESTINATION share/${PROJECT_NAME}
)

# Export package
ament_export_dependencies(tf2_ros geometry_msgs nav2_costmap_2d pluginlib)
ament_export_include_directories(include)
ament_export_libraries(nav_layer_from_points)
pluginlib_export_plugin_description_file(nav2_costmap_2d costmap_plugins.xml)

ament_package()