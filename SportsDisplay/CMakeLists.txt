# CMakeList.txt : CMake project for SportsDisplay, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package( CURL REQUIRED )

message(STATUS "Building for ${SYSTEM}")



# Add source to this project's executable.
add_executable (SportsDisplay  "include/SportsDisplay.h" "src/DataProcess.cpp" "include/DataProcess.h" "src/SportsDisplay.cpp"  "include/Team.h" "src/Team.cpp" "include/Event.h" "src/Event.cpp" "src/ImageProcessing.cpp" "include/ImageProcessing.h")

if(SYSTEM STREQUAL "windows")
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(SportsDisplay PRIVATE ${CURL_LIBRARIES} jsoncpp_lib jsoncpp_object JsonCpp::JsonCpp)
elseif(SYSTEM STREQUAL "ubuntu")
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(SportsDisplay PRIVATE ${CURL_LIBRARIES} JsonCpp::JsonCpp jsoncpp_object ${JSONCPP_LIBRARIES})
endif()



target_include_directories(SportsDisplay PUBLIC ${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} include )


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SportsDisplay PROPERTY CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

add_custom_command(TARGET SportsDisplay PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/SportsDisplay/python $<TARGET_FILE_DIR:SportsDisplay>)
# TODO: Add tests and install targets if needed.