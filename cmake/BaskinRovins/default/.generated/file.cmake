# The following variables contains the files used by the different stages of the build process.
set(BaskinRovins_default_image_name "default.elf")
set(BaskinRovins_default_image_base_name "default")

# The output directory of the final image.
set(BaskinRovins_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/BaskinRovins")

# The full path to the final image.
set(BaskinRovins_default_full_path_to_image ${BaskinRovins_default_output_dir}/${BaskinRovins_default_image_name})
