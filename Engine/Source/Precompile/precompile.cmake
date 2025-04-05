set(PRECOMPILE_TOOLS_PATH ${CMAKE_BINARY_DIR}/Engine/Source/MetaParser)
set(ZERO_ENGINE_PRECOMPILE_PARAMS_IN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source/Precompile/precompile.json.in)
set(ZERO_ENGINE_PRECOMPILE_PARAMS_PATH ${PRECOMPILE_TOOLS_PATH}/precompile.json)
configure_file(${ZERO_ENGINE_PRECOMPILE_PARAMS_IN_PATH} ${ZERO_ENGINE_PRECOMPILE_PARAMS_PATH})

if (CMAKE_HOST_WIN32)
    set(PRECOMPILE_PRE_EXT)
    set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/MetaParser.exe)
    set(sys_include "*")
    # elseif () # TODO Linux
endif ()

# Parser 参数设置
set(PARSER_INPUT ${CMAKE_BINARY_DIR}/parser_header.h)
set(PRECOMPILE_TARGET "ZeroEnginePreCompile")
set(CMD_ARG ${PRECOMPILE_PARSER} ${ZERO_ENGINE_PRECOMPILE_PARAMS_PATH} ${PARSER_INPUT} ${CMAKE_SOURCE_DIR}/Engine/Source/Runtime ${sys_include} "ZeroEngine" 0)
add_custom_target(${PRECOMPILE_TARGET} ALL
        COMMAND ${CMAKE_COMMAND} -E echo "===================================="
        COMMAND ${CMAKE_COMMAND} -E echo "[Precompile] BEGIN"
        COMMAND ${CMAKE_COMMAND} -E echo "===================================="
        COMMAND ${CMAKE_COMMAND} -E echo "Command line args:  ${CMD_ARG}"
        COMMAND ${CMD_ARG}
        COMMAND ${CMAKE_COMMAND} -E echo "[Precompile] FINISHED"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # 确保在构建目录下执行
)