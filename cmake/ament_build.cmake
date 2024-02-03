#---- Add the subdirectory cmake ----
set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${PROJECT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

if(BTCPP_GROOT_INTERFACE)
    find_package(ZeroMQ REQUIRED)
endif()

if(BTCPP_SQLITE_LOGGING)
    find_package(SQLite3 REQUIRED)
endif()

find_package(ament_index_cpp REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)

set( BTCPP_EXTRA_LIBRARIES
    $<BUILD_INTERFACE:ament_index_cpp::ament_index_cpp>
    ${rclcpp_LIBRARIES}
    ${tf2_ros_LIBRARIES}
    $<BUILD_INTERFACE:${ZeroMQ_LIBRARIES}>
    $<BUILD_INTERFACE:${SQLite3_LIBRARIES}>
)

set( BTCPP_EXTRA_INCLUDE_DIRS
    ${BTCPP_EXTRA_INCLUDE_DIRS}
    ${ament_cmake_INCLUDE_DIRS}
    ${rclcpp_INCLUDE_DIRS}
    ${tf2_ros_INCLUDE_DIRS}
)

ament_export_dependencies(ament_index_cpp)

ament_export_include_directories(include)

set( BTCPP_LIB_DESTINATION     lib )
set( BTCPP_INCLUDE_DESTINATION include )
set( BTCPP_BIN_DESTINATION     bin )

mark_as_advanced(
    BTCPP_EXTRA_LIBRARIES
    BTCPP_LIB_DESTINATION
    BTCPP_INCLUDE_DESTINATION
    BTCPP_BIN_DESTINATION )
    
macro(export_btcpp_package)
    ament_target_dependencies(${BTCPP_LIBRARY} PUBLIC rclcpp tf2_ros)
    ament_export_dependencies(rclcpp)
    ament_export_dependencies(tf2_ros)
    ament_export_include_directories(include)
    ament_export_libraries(${BTCPP_LIBRARY})
    ament_package()
endmacro()
