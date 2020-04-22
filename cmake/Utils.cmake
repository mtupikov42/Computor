include_guard()

function(parse_version_string string majorname minorname patchname)
    set(major "0")
    set(minor "0")
    set(patch "0")

    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+$" "\\1" major "${string}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+$" "\\1" minor "${string}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)$" "\\1" patch "${string}")

    set(${majorname} ${major} PARENT_SCOPE)
    set(${minorname} ${minor} PARENT_SCOPE)
    set(${patchname} ${patch} PARENT_SCOPE)
endfunction()

function(deduct_qtdir var)
    get_target_property(QMAKE_PATH Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QMAKE_DIR "${QMAKE_PATH}" DIRECTORY)
    set(QTDIR "${QMAKE_DIR}/..")
    get_filename_component(QTDIR "${QTDIR}" ABSOLUTE)

    set(${var} "${QTDIR}" PARENT_SCOPE)
endfunction()

function(adjust_target_output_dir)
    if(APP_ROOT_OUTPUT_DIR)
        string(TOUPPER ${CMAKE_BUILD_TYPE} OUTPUTCONFIG)
        foreach(targetname ${ARGV})
            set_target_properties(${targetname}
                PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${APP_ROOT_OUTPUT_DIR}"
                LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${APP_ROOT_OUTPUT_DIR}"
                RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${APP_ROOT_OUTPUT_DIR}"
            )
        endforeach()
    endif()
endfunction()
