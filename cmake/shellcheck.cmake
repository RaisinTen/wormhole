find_program(SHELLCHECK_BIN NAMES shellcheck)

if(SHELLCHECK_BIN)
  add_custom_target(shellcheck
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    VERBATIM
    COMMAND ${SHELLCHECK_BIN} ${STARSHIP_SH_SOURCE_FILES})
else()
  message(WARNING "shellcheck: command not found")
endif()
