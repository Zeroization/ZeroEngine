cmake_minimum_required(VERSION 3.19)

set(TARGET_NAME ZeroEngineEditor)

file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(ZEROENGINE_EDITOR_HEADS "${HEADER_FILES}" PARENT_SCOPE)

target_include_directories(${TARGET_NAME}
        PUBLIC ../Runtime
        PUBLIC ./
)
target_link_libraries(${TARGET_NAME} PUBLIC ZeroEngineRuntime)
