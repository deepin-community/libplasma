add_subdirectory(core)
add_subdirectory(plasmaextracomponents)
add_subdirectory(kirigamiplasmastyle)

set(QQC2_VERSION "6.${Qt6Quick_VERSION_MINOR}")

# Find all the source qml files
FILE(GLOB_RECURSE inFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/plasmacomponents3/*")

    #for each file, replace @QQC2_VERSION@ with the version we found
FOREACH(infileName ${inFiles})
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${infileName}"
        "${CMAKE_CURRENT_BINARY_DIR}/${infileName}"
        @ONLY
    )
ENDFOREACH(infileName)

ecm_add_qml_module(org_kde_plasmacomponents3 URI "org.kde.plasma.components" VERSION 3.0 NO_PLUGIN)
file(GLOB _public_qml "${CMAKE_CURRENT_BINARY_DIR}/plasmacomponents3/*.qml")
ecm_target_qml_sources(org_kde_plasmacomponents3 VERSION 3.0 SOURCES ${_public_qml})
file(GLOB _mobiletextselection_qml "${CMAKE_CURRENT_BINARY_DIR}/plasmacomponents3/mobiletextselection/*.qml" "${CMAKE_CURRENT_BINARY_DIR}/plasmacomponents3/mobiletextselection/qmldir")
ecm_target_qml_sources(org_kde_plasmacomponents3 VERSION 3.0 PATH mobiletextselection SOURCES ${_mobiletextselection_qml})
file(GLOB _private_qml "${CMAKE_CURRENT_BINARY_DIR}/plasmacomponents3/private/*.qml")

set_source_files_properties(${_private_qml} PROPERTIES QT_QML_INTERNAL_TYPE TRUE)

ecm_target_qml_sources(org_kde_plasmacomponents3 VERSION 3.0 PATH private SOURCES ${_private_qml})
ecm_finalize_qml_module(org_kde_plasmacomponents3 DESTINATION ${KDE_INSTALL_QMLDIR})
