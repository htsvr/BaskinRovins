include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(BaskinRovins_default_library_list )


# Main target for this project
add_executable(BaskinRovins_default_image_P__I5I_c ${BaskinRovins_default_library_list})

if(NOT CMAKE_HOST_WIN32)
    set_target_properties(BaskinRovins_default_image_P__I5I_c PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BaskinRovins_default_output_dir}")
endif()
set_target_properties(BaskinRovins_default_image_P__I5I_c PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf")


# Add the link options from the rule file.
BaskinRovins_default_link_rule( BaskinRovins_default_image_P__I5I_c)

# Call bin2hex function from the rule file
BaskinRovins_default_bin2hex_rule(BaskinRovins_default_image_P__I5I_c)
if(CMAKE_HOST_WIN32)
    add_custom_command(
        TARGET BaskinRovins_default_image_P__I5I_c
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BaskinRovins_default_output_dir}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BaskinRovins_default_image_P__I5I_c> ${BaskinRovins_default_output_dir}/${BaskinRovins_default_original_image_name}
        BYPRODUCTS ${BaskinRovins_default_output_dir}/${BaskinRovins_default_original_image_name}
        COMMENT "Copying elf to out location")
    set_property(
        TARGET BaskinRovins_default_image_P__I5I_c
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES
        ${BaskinRovins_default_output_dir}/${BaskinRovins_default_original_image_name})
endif()

