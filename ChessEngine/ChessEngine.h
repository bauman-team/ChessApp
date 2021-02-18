#pragma once
#ifdef CHESSENGINE_EXPORTS
#define CHESSENGINE_API __declspec(dllexport)
#else
#define CHESSENGINE_API __declspec(dllimport)
#endif

//#include "Pos.h"
//#include "Figure.h"
//#include "MoveInfo.h"
//#include "Map.h"