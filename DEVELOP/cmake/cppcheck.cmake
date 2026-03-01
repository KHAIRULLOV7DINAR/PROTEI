    find_program(CPPCHECK cppcheck)
    if(NOT CPPCHECK)
        message(FATAL_ERROR "cppcheck not found.")
    endif()
    
    execute_process(
        COMMAND ${CPPCHECK}
            --enable=warning,performance,portability,information,missingInclude
            --std=c++17    
            --template="[[severity]][{id}]{message}{callstack} (On {file}:{line})"
            --verbose
            --quiet
            --suppress=missingIncludeSystem
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_QUIET
        COMMAND_ERROR_IS_FATAL ANY
    )