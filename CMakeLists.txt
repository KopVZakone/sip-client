cmake_minimum_required(VERSION 3.16)

project(sip-client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup()

qt_add_executable(appsip-client
    main.cpp
)

qt_add_qml_module(appsip-client
    URI SipClient
    VERSION 0.1
    QML_FILES
        ui/Main.qml
        ui/screens/CallScreen.qml
        ui/screens/ContactsScreen.qml
        ui/screens/HistoryScreen.qml
        ui/screens/AccountsScreen.qml
        ui/screens/SettingsScreen.qml
        ui/utils/Utils.js
        SOURCES core/databasemanager.h core/databasemanager.cpp
        SOURCES models/settingsmanager.h models/settingsmanager.cpp
        SOURCES core/audiomanager.h core/audiomanager.cpp
        SOURCES core/sipmanager.h core/sipmanager.cpp
)
target_include_directories(appsip-client
    PRIVATE
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/models

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appsip-client PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appsip-client
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appsip-client
    PRIVATE Qt6::Quick Qt6::Sql
)

# Компиляция и подключение библиотек
set(PJSIP_PROJECT_ROOT "/usr/local/src/pjproject-2.16")
add_subdirectory(${PJSIP_PROJECT_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/pjsip_build")
set(PJSIP_LIBS
    pjsua2 pjsip-ua pjsip-simple pjsip
    pjmedia-codec pjmedia-audiodev
    pjmedia-videodev pjmedia pjnath pjlib-util
    pjlib
)
set(PJSIP_THIRD_PARTY_LIBS
    g7221 gsm ilbc resample speex srtp webrtc yuv
)
target_link_libraries(appsip-client
    PRIVATE  ${PJSIP_LIBS}
            ${PJSIP_THIRD_PARTY_LIBS}
)




include(GNUInstallDirs)
install(TARGETS appsip-client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
