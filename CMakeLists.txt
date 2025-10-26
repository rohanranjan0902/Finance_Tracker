cmake_minimum_required(VERSION 3.16)
project(FinTrack)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories
include_directories(src)

# Source files
set(MODEL_SOURCES
    src/models/User.cpp
    src/models/Account.cpp
    src/models/Transaction.cpp
    src/models/Budget.cpp
)

set(SERVICE_SOURCES
    src/services/TransactionService.cpp
    src/services/FraudDetectionService.cpp
)

set(ALL_SOURCES
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
    src/main.cpp
)

# Create the executable
add_executable(FinTrack ${ALL_SOURCES})

# Static linking for portable executable
if(MSVC)
    set_property(TARGET FinTrack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    target_link_options(FinTrack PRIVATE -static-libgcc -static-libstdc++)
endif()

# Link threading library
find_package(Threads REQUIRED)
target_link_libraries(FinTrack Threads::Threads)

# Platform-specific settings
if(WIN32)
    # Windows specific settings
    target_compile_definitions(FinTrack PRIVATE _WIN32_WINNT=0x0601)
endif()

if(UNIX)
    # Unix/Linux specific settings
    target_link_libraries(FinTrack pthread)
endif()

# Optional: Enable additional warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(FinTrack PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Set output directory
set_target_properties(FinTrack PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Custom target for running the application
add_custom_target(run
    COMMAND FinTrack
    DEPENDS FinTrack
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running FinTrack application"
)
