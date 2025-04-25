cmake_minimum_required(VERSION 3.5)

project(crc-client-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CRC_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

include_directories(${PROJECT_SOURCE_DIR})

set(TS_FILES crc-client-qt_en_GB.ts)

set(PROJECT_SOURCES
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  ${TS_FILES}
)

# 添加库目标
add_library(crc_lib SHARED IMPORTED)
set_target_properties(crc_lib PROPERTIES
  IMPORTED_IMPLIB ${CRC_LIB_DIR}/libcrc_shared.dll.a
  IMPORTED_LOCATION ${CRC_LIB_DIR}/libcrc_shared.dll)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(crc-client-qt
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
  )
  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
  add_executable(crc-client-qt ${PROJECT_SOURCES})
  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(crc-client-qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets crc_lib)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.crc-client-qt)
endif()
set_target_properties(crc-client-qt PROPERTIES
  ${BUNDLE_ID_OPTION}
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE FALSE
  WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS crc-client-qt
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


# 添加windeployqt步骤
if(WIN32)
  # 查找windeployqt工具
  get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
  get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(WINDEPLOYQT_EXECUTABLE "${_qt_bin_dir}/windeployqt.exe")
  else()
    set(WINDEPLOYQT_EXECUTABLE "${_qt_bin_dir}/windeployqt.exe")
  endif()

  # 添加自定义目标用于打包
  add_custom_command(TARGET crc-client-qt POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/package"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/package"
    COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:crc-client-qt> "${CMAKE_CURRENT_BINARY_DIR}/package"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CRC_LIB_DIR}/libcrc_shared.dll" "${CMAKE_CURRENT_BINARY_DIR}/package"
    COMMAND "${WINDEPLOYQT_EXECUTABLE}" --compiler-runtime "${CMAKE_CURRENT_BINARY_DIR}/package/$<TARGET_FILE_NAME:crc-client-qt>"
    COMMENT "Deploying Qt libraries and creating standalone package..."
  )

  # 创建打包目标
  add_custom_target(package
    DEPENDS crc-client-qt
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/package" "${CMAKE_CURRENT_BINARY_DIR}/crc-client-qt"
    COMMAND "${CMAKE_COMMAND}" -E tar "cfv" "${CMAKE_CURRENT_BINARY_DIR}/crc-client-qt.zip" --format=zip "${CMAKE_CURRENT_BINARY_DIR}/crc-client-qt"
    COMMENT "Creating final package..."
  )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(crc-client-qt)
endif()
