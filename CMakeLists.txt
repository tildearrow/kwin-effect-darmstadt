cmake_minimum_required (VERSION 3.16)

project(kwin-effect-darmstadt)
set(PROJECT_VERSION "1.0")
set(PROJECT_VERSION_MAJOR 1)

set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "5.248.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    DBus
    Gui
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    I18n
    KCMUtils
    XmlGui
)


find_package(KWin REQUIRED)
find_package(KWinDBusInterface REQUIRED)
find_package(epoxy REQUIRED)

add_subdirectory(src)


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
