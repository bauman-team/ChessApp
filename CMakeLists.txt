cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	#set(CMAKE_GENERATOR_PLATFORM win32)
	set(EXECUTABLE_OUTPUT_PATH "bin/")
	set(LIBRARY_OUTPUT_PATH "bin/")
endif()

set(PROJECT_NAME ChessApp)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/Chess/main.cpp
	src/Chess/Resources.h

	src/Chess/Game.h
	src/Chess/Game.cpp
	src/Chess/SideMenu.h
	src/Chess/SideMenu.cpp
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

set(LIBRARY_NAME ChessEngine)
add_library(${LIBRARY_NAME} SHARED
	src/ChessEngine/framework.h
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

if(WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		"${PROJECT_SOURCE_DIR}/build/external/SFML-2.5.1/lib/$<CONFIGURATION>/sfml-graphics-2.dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		"${PROJECT_SOURCE_DIR}/build/external/SFML-2.5.1/lib/$<CONFIGURATION>/sfml-system-2.dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		"${PROJECT_SOURCE_DIR}/build/external/SFML-2.5.1/lib/$<CONFIGURATION>/sfml-window-2.dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		"${PROJECT_SOURCE_DIR}/build/external/TGUI-0.8/lib/$<CONFIGURATION>/tgui.dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			"${PROJECT_SOURCE_DIR}/build/external/SFML-2.5.1/lib/$<CONFIGURATION>/sfml-graphics-d-2.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			"${PROJECT_SOURCE_DIR}/build/external/SFML-2.5.1/lib/$<CONFIGURATION>/sfml-system-d-2.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			"${PROJECT_SOURCE_DIR}/build/external/SFML-2.5.1/lib/$<CONFIGURATION>/sfml-window-d-2.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			"${PROJECT_SOURCE_DIR}/build/external/TGUI-0.8/lib/$<CONFIGURATION>/tgui-d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	endif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_sources(${LIBRARY_NAME} PRIVATE src/ChessEngine/dllmain.cpp)
	target_compile_definitions(${LIBRARY_NAME} PRIVATE "CHESSENGINE_EXPORTS")
elseif(UNIX)
	find_package (Threads) 
	target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_NAME}) 

add_subdirectory(external/SFML-2.5.1)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-system sfml-window) #sfml-audio sfml-network

add_subdirectory(external/TGUI-0.8)
target_link_libraries(${PROJECT_NAME} PUBLIC tgui)

target_include_directories(${PROJECT_NAME} PUBLIC src/ChessEngine)
target_include_directories(${PROJECT_NAME} PUBLIC external/SFML-2.5.1/include)
target_include_directories(${PROJECT_NAME} PUBLIC external/TGUI-0.8/include)

set(TESTS_NAME EngineTests)
add_executable(${TESTS_NAME}
	tests/EngineTests/LaunchTesting.cpp
	tests/EngineTests/UnitTesting.cpp
	tests/EngineTests/pch.cpp
	tests/EngineTests/pch.h
)

add_subdirectory(external/gtest)
target_link_libraries(${TESTS_NAME} PUBLIC gtest) #gtest_main

target_link_libraries(${TESTS_NAME} PUBLIC ${LIBRARY_NAME}) 

target_include_directories(${TESTS_NAME} PUBLIC src/ChessEngine)
target_include_directories(${TESTS_NAME} PUBLIC external/gtest/googletest/include)

if(UNIX)
enable_testing()
add_test(NAME ${TESTS_NAME}
	COMMAND ${TESTS_NAME})
endif()