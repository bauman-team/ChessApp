cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME ChessApp)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/Chess/main.cpp
	src/Chess/Resources.h

	src/Chess/Game.h
	src/Chess/Game.cpp
	src/Chess/Drawer.h
	src/Chess/Drawer.cpp
	src/Chess/MapProperties.h
	src/Chess/MapProperties.cpp
	src/Chess/Menu.h
	src/Chess/Menu.cpp
	src/Chess/Player.h
	src/Chess/Player.cpp
	src/Chess/PlayerWithAIGame.h 
	src/Chess/PlayerWithAIGame.cpp
	src/Chess/TwoPlayersGame.h
	src/Chess/TwoPlayersGame.cpp
)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_library(ChessEngine SHARED
	src/ChessEngine/framework.h
	src/ChessEngine/dllmain.cpp
	src/ChessEngine/pch.h
	src/ChessEngine/pch.cpp
	src/ChessEngine/ChessEngine.h
	src/ChessEngine/Figure.h
	src/ChessEngine/Figure.cpp
	src/ChessEngine/Map.h
	src/ChessEngine/Map.cpp
	src/ChessEngine/MoveInfo.h
	src/ChessEngine/MoveInfo.cpp
	src/ChessEngine/Pos.h
	src/ChessEngine/Pos.cpp
)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC ChessEngine) 

add_subdirectory(external/SFML-2.5.1)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-audio sfml-graphics sfml-network sfml-system sfml-window)

add_subdirectory(external/TGUI-0.8)
target_link_libraries(${PROJECT_NAME} PUBLIC tgui)

target_include_directories(${PROJECT_NAME} PUBLIC src/ChessEngine)
target_include_directories(${PROJECT_NAME} PUBLIC external/SFML-2.5.1/include)
target_include_directories(${PROJECT_NAME} PUBLIC external/TGUI-0.8/include)