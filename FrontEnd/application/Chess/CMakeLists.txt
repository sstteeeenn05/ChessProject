project(ChessProject)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT02_SRC_LIST
        ${PROJECT_SOURCE_DIR}/ChessProject/main.cpp
        ${PROJECT_SOURCE_DIR}/ChessProject/Chess.cpp
        ${PROJECT_SOURCE_DIR}/ChessProject/Chess.h
        ${PROJECT_SOURCE_DIR}/ChessProject/DataType.h
        ${PROJECT_SOURCE_DIR}/ChessProject/Position.h
        ${PROJECT_SOURCE_DIR}/ChessProject/Board.h
        ${PROJECT_SOURCE_DIR}/ChessProject/Board.cpp)

add_executable(ChessProject ${PROJECT02_SRC_LIST})