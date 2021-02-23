#pragma once
//#ifdef _WIN32
	#ifdef CHESSENGINE_EXPORTS
		#define CHESSENGINE_API __declspec(dllexport)
	#else
		#define CHESSENGINE_API __declspec(dllimport)
	#endif
//#elif
//	#define LIBRARY_API
//#endif