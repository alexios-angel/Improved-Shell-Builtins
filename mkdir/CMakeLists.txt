project(mkdir_im)

# Create the executable
add_executable(${PROJECT_NAME} main.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES})