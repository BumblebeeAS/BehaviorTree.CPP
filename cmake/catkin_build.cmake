#---- Add the subdirectory cmake ----
set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${PROJECT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

if(BTCPP_GROOT_INTERFACE)
    find_package(ZeroMQ REQUIRED)
endif()

if(BTCPP_SQLITE_LOGGING)
    find_package(SQLite3 REQUIRED)
endif()

find_package(catkin REQUIRED COMPONENTS roslib roscpp)

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES ${BTCPP_LIBRARY}
        CATKIN_DEPENDS roslib roscpp)

set(BTCPP_EXTRA_INCLUDE_DIRS ${catkin_INCLUDE_DIRS} )

set( BTCPP_EXTRA_LIBRARIES
    ${catkin_LIBRARIES}
    ${ZeroMQ_LIBRARIES}
    ${SQLite3_LIBRARIES})

set( BTCPP_LIB_DESTINATION     ${CATKIN_PACKAGE_LIB_DESTINATION} )
set( BTCPP_INCLUDE_DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION} )
set( BTCPP_BIN_DESTINATION     ${CATKIN_GLOBAL_BIN_DESTINATION} )

mark_as_advanced(
    BTCPP_EXTRA_LIBRARIES
    BTCPP_EXTRA_INCLUDE_DIRS
    BTCPP_LIB_DESTINATION
    BTCPP_INCLUDE_DESTINATION
    BTCPP_BIN_DESTINATION )

macro(export_btcpp_package)
 # do nothing
endmacro()
