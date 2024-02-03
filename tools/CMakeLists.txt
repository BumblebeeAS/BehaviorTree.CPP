find_package(ament_cmake QUIET)

if ( ament_cmake_FOUND )

    add_definitions( -DUSING_ROS2 )
    message(STATUS "------------------------------------------")
    message(STATUS "BehaviourTree is being built using AMENT.")
    message(STATUS "------------------------------------------")
    find_package(rclcpp REQUIRED)

elseif( CATKIN_DEVEL_PREFIX OR CATKIN_BUILD_BINARY_PACKAGE)

    add_definitions( -DUSING_ROS )
    message(STATUS "------------------------------------------")
    message(STATUS "BehaviourTree is being built using CATKIN.")
    message(STATUS "------------------------------------------")
    set(catkin_FOUND TRUE)
    find_package(catkin REQUIRED COMPONENTS
        roscpp
    )

endif()

include_directories(${PROJECT_SOURCE_DIR}/3rdparty
        ${catkin_INCLUDE_DIRS}
)

add_executable(bt3_log_cat         bt_log_cat.cpp )
target_link_libraries(bt3_log_cat  ${BTCPP_LIBRARY} )
install(TARGETS bt3_log_cat
        DESTINATION ${BTCPP_BIN_DESTINATION} )

if( ZMQ_FOUND )
    add_executable(bt3_recorder         bt_recorder.cpp )
    target_link_libraries(bt3_recorder  ${BTCPP_LIBRARY} ${ZMQ_LIBRARIES})
    install(TARGETS bt3_recorder
            DESTINATION ${BTCPP_BIN_DESTINATION} )
endif()

add_executable(bt3_plugin_manifest         bt_plugin_manifest.cpp )
target_link_libraries(bt3_plugin_manifest  ${BTCPP_LIBRARY} ${catkin_LIBRARIES})
install(TARGETS bt3_plugin_manifest
        DESTINATION ${BTCPP_BIN_DESTINATION} )



