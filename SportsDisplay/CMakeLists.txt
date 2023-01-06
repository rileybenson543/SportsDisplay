# CMakeList.txt : CMake project for SportsDisplay, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package( CURL REQUIRED )
find_package(simdjson CONFIG REQUIRED)

message(STATUS "Building for ${SYSTEM}")



# Add source to this project's executable.
add_executable (SportsDisplay  "include/SportsDisplay.h" "src/DataProcess.cpp" "include/DataProcess.h" "src/SportsDisplay.cpp"  "include/Team.h" "src/Team.cpp" "include/Event.h" "src/Event.cpp" "src/ImageProcessing.cpp" "include/ImageProcessing.h" "src/NewsItem.cpp" "include/NewsItem.h" "src/Serial.cpp" "src/serialib.cpp" "include/Serial.h" "src/Scheduler.cpp" "include/Scheduler.h" "src/Datetime/datetime.cpp" "src/Datetime/timespan.cpp" "include/Datetime/datetime.h" "include/Datetime/timespan.h")

if(SYSTEM STREQUAL "windows")
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(SportsDisplay PRIVATE ${CURL_LIBRARIES} jsoncpp_lib jsoncpp_object JsonCpp::JsonCpp simdjson::simdjson)
elseif(SYSTEM STREQUAL "ubuntu")
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(SportsDisplay PRIVATE ${CURL_LIBRARIES} JsonCpp::JsonCpp jsoncpp_object ${JSONCPP_LIBRARIES} simdjson::simdjson)
endif()



target_include_directories(SportsDisplay PUBLIC ${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} include lib)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SportsDisplay PROPERTY CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif(CMAKE_COMPILER_IS_GNUCXX)

add_custom_command(TARGET SportsDisplay PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/SportsDisplay/python $<TARGET_FILE_DIR:SportsDisplay>)

install(TARGETS SportsDisplay DESTINATION bin)
install(FILES python/ConvertImages.py DESTINATION bin)
# TODO: Add tests