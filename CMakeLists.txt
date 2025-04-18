cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION "6.2.1")
project(plasma-framework VERSION ${PROJECT_VERSION})

set(QT_MIN_VERSION "6.7.0")
set(PROJECT_DEP_VERSION "6.2.1")
set(KF6_MIN_VERSION "6.5.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} )

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEGitCommitHooks)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)

include(ECMGenerateExportHeader)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)
include(KDEPackageAppTemplates)
include(ECMGenerateQmlTypes)
include(ECMMarkNonGuiExecutable)
include(ECMDeprecationSettings)
include(ECMQmlModule)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT
                  VARIABLE_PREFIX PLASMA
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/plasma_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/PlasmaConfigVersion.cmake"
                  SOVERSION 6)

################# now find all used packages #################

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Quick Gui Qml Svg QuickControls2 DBus)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED
    COMPONENTS
        Archive
        Config
        ConfigWidgets
        CoreAddons
        GlobalAccel
        GuiAddons
        I18n
        IconThemes
        KIO
        WindowSystem
        Notifications
        Package
        KirigamiPlatform
        KCMUtils
        Svg
)
find_package(PlasmaActivities REQUIRED ${PROJECT_DEP_VERSION})

# Wayland features
find_package(PlasmaWaylandProtocols 1.10.0 REQUIRED)
find_package(Qt6WaylandClient ${QT_MIN_VERSION} REQUIRED CONFIG)
find_package(Wayland 1.9 REQUIRED Client)

option(WITHOUT_X11 "Build without X11 support (skips finding X11)." OFF)
if(NOT WITHOUT_X11)
    #optional features
    find_package(X11 MODULE)
    set_package_properties(X11 PROPERTIES DESCRIPTION "X11 libraries"
                        URL "https://www.x.org/"
                        TYPE OPTIONAL
                        )
    find_package(XCB MODULE COMPONENTS XCB COMPOSITE DAMAGE XFIXES RENDER)
    set_package_properties(XCB PROPERTIES DESCRIPTION "X protocol C-language Binding"
                        URL "https://xcb.freedesktop.org/"
                        TYPE OPTIONAL
                        )
    if(X11_FOUND AND XCB_XCB_FOUND)
        set(HAVE_X11 1)
        #X11_Xrender discovery is done by FindX11
        #add_feature_info("X Rendering Extension (libXrender)" X11_Xrender_FOUND "Support for compositing, rendering operations, and alpha-blending. STRONGLY RECOMMENDED")
    else()
        set(HAVE_X11 0)
    endif()
endif()

find_package(OpenGL)
set_package_properties(OpenGL PROPERTIES DESCRIPTION "The OpenGL libraries"
                       URL "https://www.opengl.org/"
                       TYPE OPTIONAL
                      )

find_package(EGL)
set_package_properties(EGL PROPERTIES
                       PURPOSE "Fallback when OpenGL not available because of missing GLVND"
                       TYPE OPTIONAL
                      )

get_target_property(QtGui_Enabled_Features Qt6::Gui QT_ENABLED_PUBLIC_FEATURES)
if(OpenGL_GLX_FOUND AND HAVE_X11 AND (QtGui_Enabled_Features MATCHES "opengl"))
    add_feature_info(GLX OpenGL_GLX_FOUND "OpenGL GLX libraries.")
    set(HAVE_GLX 1)
else()
    set(HAVE_GLX 0)
endif()

# OpenGL_EGL_FOUND is defined by FindOpenGL
if(TARGET OpenGL::EGL)
    add_feature_info(EGL OpenGL_EGL_FOUND
                     "A platform-agnostic mechanism for creating rendering surfaces for use with other graphics libraries, such as OpenGL|ES and OpenVG.")
    set(HAVE_EGL 1)
else()
    set(HAVE_EGL 0)
    message(STATUS "OpenGL::EGL is not available")
endif()

#########################################################################

ecm_set_disabled_deprecation_versions(
    QT 6.7
    KF 6.5
)

#add_definitions(-Wno-deprecated)

include(PlasmaMacros.cmake)

#########################################################################


option(BUILD_EXAMPLES "Build and install Plasma examples." OFF)
option(BUILD_COVERAGE "Build Plasma Frameworks with gcov support" OFF)

if (BUILD_EXAMPLES)
    find_package(KF6DBusAddons ${KF6_MIN_VERSION})
endif()

if(BUILD_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
endif()

# make plasma_version.h available
include_directories(${CMAKE_CURRENT_BINARY_DIR})

################# list the subdirectories #################
add_definitions(-DTRANSLATION_DOMAIN=\"libplasma6\")
ki18n_install(po)
add_subdirectory(src)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()
add_subdirectory(templates)

################ create PlasmaConfig.cmake and install it ###########################

# create a Config.cmake and a ConfigVersion.cmake file and install them

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/Plasma")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS Plasma_QCH
        FILE PlasmaQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/PlasmaQchTargets.cmake\")")
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/PlasmaConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/PlasmaConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
  PATH_VARS  CMAKE_INSTALL_PREFIX
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/PlasmaConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/PlasmaConfigVersion.cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/PlasmaMacros.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT PlasmaTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE PlasmaTargets.cmake
        NAMESPACE Plasma::
        COMPONENT Devel)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plasma_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR}/Plasma COMPONENT Devel )

include(ECMFeatureSummary)
ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

# add clang-format target for all our source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
