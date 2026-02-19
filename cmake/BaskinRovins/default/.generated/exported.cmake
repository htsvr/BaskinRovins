set(DEPENDENT_MP_BIN2HEXBaskinRovins_default_P__I5I_c "c:/Program Files/Microchip/xc-dsc/v3.30/bin/xc-dsc-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFBaskinRovins_default_P__I5I_c "${CMAKE_CURRENT_LIST_DIR}/../../../../out/BaskinRovins/default.elf")
set(DEPENDENT_TARGET_DIRBaskinRovins_default_P__I5I_c "${CMAKE_CURRENT_LIST_DIR}/../../../../out/BaskinRovins")
set(DEPENDENT_BYPRODUCTSBaskinRovins_default_P__I5I_c ${DEPENDENT_TARGET_DIRBaskinRovins_default_P__I5I_c}/${sourceFileNameBaskinRovins_default_P__I5I_c}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRBaskinRovins_default_P__I5I_c}/${sourceFileNameBaskinRovins_default_P__I5I_c}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXBaskinRovins_default_P__I5I_c} ${DEPENDENT_DEPENDENT_TARGET_ELFBaskinRovins_default_P__I5I_c} --image ${sourceFileNameBaskinRovins_default_P__I5I_c} ${addressBaskinRovins_default_P__I5I_c} ${modeBaskinRovins_default_P__I5I_c} -mdfp=C:/Users/grace/.mchp_packs/Microchip/PIC24F-KA-KL-KM_DFP/1.5.253/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRBaskinRovins_default_P__I5I_c}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFBaskinRovins_default_P__I5I_c})
add_custom_target(
    dependent_produced_source_artifactBaskinRovins_default_P__I5I_c 
    DEPENDS ${DEPENDENT_TARGET_DIRBaskinRovins_default_P__I5I_c}/${sourceFileNameBaskinRovins_default_P__I5I_c}.s
    )
