cmake_minimum_required(VERSION 3.16)
project(vokey VERSION 0.3.2)

set(CMAKE_CXX_STANDARD 17)

# Setup vokey_manager and generate the QT stuff using uic
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
add_executable(vokey_manager source/manager/VokeyManager.cpp)

# Setup vokey_service
set(CMAKE_AUTOUIC FALSE)
add_executable(vokey_service source/service/VokeyService.cpp source/service/Action.cpp source/service/EventRecognizer.cpp source/service/VoiceRecognizer.cpp source/service/Candy.h)

include_directories(source/common)

# Link Qt5 for vokey_manager's GUI
find_package(Qt5Widgets REQUIRED)
target_link_libraries(vokey_manager Qt5::Widgets)

# Pass version and build date to vokey_service and vokey_manager
string(TIMESTAMP today "%d/%m/%Y")
target_compile_definitions(vokey_service PRIVATE VOKEY_VERSION="${CMAKE_PROJECT_VERSION}" BUILD_DATE="${today}")
target_compile_definitions(vokey_manager PRIVATE VOKEY_VERSION="${CMAKE_PROJECT_VERSION}" BUILD_DATE="${today}")

# Find X11 for vokey_service macro execution
find_package(X11 REQUIRED)

# Find Pocketsphinx for vokey_service voice recognition
find_package(PkgConfig REQUIRED)
pkg_check_modules(POCKETSPHINX REQUIRED IMPORTED_TARGET pocketsphinx)
pkg_get_variable(MODELDIR pocketsphinx modeldir)
target_compile_definitions(vokey_service PRIVATE MODELDIR="${MODELDIR}")

# Find libpulse-simple
pkg_check_modules(PULSESIMPLE REQUIRED IMPORTED_TARGET libpulse-simple)

# Link X11, libpulse-simple and Pocketsphinx to vokey_service
target_link_libraries(vokey_service PkgConfig::PULSESIMPLE PkgConfig::POCKETSPHINX X11::X11 X11::Xtst)

# Installation
install(TARGETS vokey_service vokey_manager RUNTIME DESTINATION /usr/bin)
install(FILES sounds/listening_begin.wav sounds/listening_end.wav DESTINATION /usr/share/sounds/vokey)
install(FILES desktop/vokey.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)
install(FILES "desktop/Vokey Manager.desktop" DESTINATION /usr/share/applications)
