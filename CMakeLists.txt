cmake_minimum_required(VERSION 3.6)
project(chess)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#=====================================================
# TODO: Change directory QT for windows/mac/linux
# Uncomment correct directory, and comment other directories
#=====================================================

#=====================================================
#Linux (tested in classroom):
#=====================================================
#set(Qt5Core_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5Core")
#set(Qt5Widgets_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5Widgets")
#set(Qt5Gui_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5Gui")

#For other linux variants locate local installation directory Qt5; e.g.:
#=====================================================
#set(Qt5Core_DIR "/home/administrator/Qt/5.15.2/gcc_64/lib/cmake/Qt5Core")
#set(Qt5Widgets_DIR "/home/administrator/Qt/5.15.2/gcc_64/lib/cmake/Qt5Widgets")
#set(Qt5Gui_DIR "/home/administrator/Qt/5.15.2/gcc_64/lib/cmake/Qt5Gui")


#=====================================================
# Mac
#=====================================================
#set(Qt5Core_DIR "~/Qt/5.9.2/clang_64/lib/cmake/Qt5Core")
#set(Qt5Widgets_DIR "~/Qt/5.9.2/clang_64/lib/cmake/Qt5Widgets")
#set(Qt5Gui_DIR "~/Qt/5.9.2/clang_64/lib/cmake/Qt5Gui")

#=====================================================
# Windows
#=====================================================
# set(Qt5Core_DIR "C:\\Qt5.15.2\\mingw81_46\\lib\\cmake\\Qt5Core")
# set(Qt5Widgets_DIR "C:\\Qt5.15.2\\mingw81_46\\lib\\cmake\\Qt5Widgets")
# set(Qt5Gui_DIR "C:\\Qt5.15.2\\mingw81_46\\lib\\cmake\\Qt5Gui")

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp guicode/ChessWindow.cpp guicode/ChessWindow.h guicode/chessboard.cpp guicode/chessboard.h game.cpp game.h SchaakStuk.cpp SchaakStuk.h guicode/resources.h SchaakGUI.cpp SchaakGUI.h guicode/message.cpp guicode/message.h guicode/fileIO.cpp guicode/fileIO.h)
add_executable(chess ${SOURCE_FILES})

target_link_libraries(chess
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

