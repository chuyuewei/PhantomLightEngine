cmake_minimum_required(VERSION 3.14)
project(PhantomLightEngine VERSION 0.1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 包含子目录
add_subdirectory(Engine)
add_subdirectory(Examples)

# 确保PhantomLightEngine目标存在
# 如果PhantomLightEngine是在Engine子目录中定义的，请确保它被正确导出
# 下面的安装规则假设PhantomLightEngine目标已经在某处定义

# 安装规则 - 确保目标存在
if(TARGET PhantomLightEngine)
    install(TARGETS PhantomLightEngine
        EXPORT PhantomLightEngineTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # 导出目标 - 使用单一导出集
    install(EXPORT PhantomLightEngineTargets
        FILE PhantomLightEngineTargets.cmake
        NAMESPACE PhantomLightEngine::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PhantomLightEngine
    )
endif()

# 创建配置文件
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "PhantomLightEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# 安装配置文件
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/PhantomLightEngineConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PhantomLightEngine
)