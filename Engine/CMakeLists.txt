# Engine CMakeLists.txt
set(ENGINE_NAME PhantomLightEngine)

# 收集所有源文件
file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

file(GLOB_RECURSE ENGINE_HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp"
)

# 创建库
add_library(${ENGINE_NAME} STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# 设置包含目录
# 添加包含目录
target_include_directories(PhantomLightEngine 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# 添加子目录
add_subdirectory(Source/Core)
add_subdirectory(Source/Renderer)
add_subdirectory(Source/Physics)
add_subdirectory(Source/Audio)
add_subdirectory(Source/Resource)
add_subdirectory(Source/Scene)
add_subdirectory(Source/Input)
add_subdirectory(Source/Platform)
add_subdirectory(Source/Math)
add_subdirectory(Source/Utils)

# 安装规则
install(TARGETS ${ENGINE_NAME} EXPORT ${ENGINE_NAME}Targets)
install(DIRECTORY Include/ DESTINATION include)