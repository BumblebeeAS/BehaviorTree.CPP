find_package(ament_cmake QUIET)

if ( ament_cmake_FOUND )

    add_definitions( -DUSING_ROS2 )
    message(STATUS "------------------------------------------")
    message(STATUS "BehaviourTree is being built using AMENT.")
    message(STATUS "------------------------------------------")
    find_package(rclcpp REQUIRED)
    find_package(tf2_ros REQUIRED)
    include_directories( ../include
        ${rclcpp_INCLUDE_DIRS}
        ${tf2_ros_INCLUDE_DIRS}
    )

elseif( CATKIN_DEVEL_PREFIX OR CATKIN_BUILD_BINARY_PACKAGE)

    add_definitions( -DUSING_ROS )
    message(STATUS "------------------------------------------")
    message(STATUS "BehaviourTree is being built using CATKIN.")
    message(STATUS "------------------------------------------")
    set(catkin_FOUND TRUE)
    find_package(catkin REQUIRED COMPONENTS
        roscpp
        tf2_ros
    )
    include_directories( ../include
        ${catkin_INCLUDE_DIRS}
    )
endif()

# compile as static libraries

set(CMAKE_DEBUG_POSTFIX "")

set(PROJECT_LIBRARIES ${catkin_LIBRARIES} ${ament_cmake_LIBRARIES} ${rclcpp_LIBRARIES} ${tf2_ros_LIBRARIES})

add_library(bt_sample_nodes STATIC
    crossdoor_nodes.cpp
    dummy_nodes.cpp
    movebase_node.cpp )

target_link_libraries(bt_sample_nodes PRIVATE ${BTCPP_LIBRARY} ${PROJECT_LIBRARIES})
set_target_properties(bt_sample_nodes PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
    ${BTCPP_LIB_DESTINATION} )

# to create a plugin, compile them in this way instead

add_library(crossdoor_nodes_dyn SHARED crossdoor_nodes.cpp )
target_link_libraries(crossdoor_nodes_dyn PRIVATE ${BTCPP_LIBRARY} ${PROJECT_LIBRARIES})
target_compile_definitions(crossdoor_nodes_dyn PRIVATE  BT_PLUGIN_EXPORT )
set_target_properties(crossdoor_nodes_dyn PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    ${BTCPP_BIN_DESTINATION} )

add_library(dummy_nodes_dyn     SHARED dummy_nodes.cpp )
target_link_libraries(dummy_nodes_dyn PRIVATE ${BTCPP_LIBRARY} ${PROJECT_LIBRARIES})
target_compile_definitions(dummy_nodes_dyn  PRIVATE BT_PLUGIN_EXPORT)
set_target_properties(dummy_nodes_dyn PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    ${BTCPP_BIN_DESTINATION} )


add_library(movebase_node_dyn   SHARED movebase_node.cpp )
target_link_libraries(movebase_node_dyn PRIVATE ${BTCPP_LIBRARY} ${PROJECT_LIBRARIES})
target_compile_definitions(movebase_node_dyn PRIVATE  BT_PLUGIN_EXPORT )
set_target_properties(movebase_node_dyn PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    ${BTCPP_BIN_DESTINATION} )
