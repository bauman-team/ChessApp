#include "pch.h"
#include "Map.h"

const uint8_t Map::offsetHorizontal{ 1 }, Map::offsetVertical{ 8 }, Map::offsetMainDiag{ 7 }, Map::offsetSideDiag{ 9 };
const uint64_t Map::knightBorderAB{ 18'229'723'555'195'321'596 }, Map::knightBorderGH{ 4'557'430'888'798'830'399 },
Map::mapLeftBorder{ 72'340'172'838'076'673 }, Map::mapRightBorder{ 9'259'542'123'273'814'144 },
Map::mapUpBorder{ 18'374'686'479'671'623'680 }, Map::mapDownBorder{ 255 };

Map::Map() noexcept
{ // 0 - left down
	map[toUType(FigureType::Rook_black)] = 9'295'429'630'892'703'744;
	map[toUType(FigureType::Knight_black)] = 4'755'801'206'503'243'776;
	map[toUType(FigureType::Bishop_black)] = 2'594'073'385'365'405'696;
	map[toUType(FigureType::Queen_black)] = 576'460'752'303'423'488;
	map[toUType(FigureType::King_black)] = 1'152'921'504'606'846'976;
	map[toUType(FigureType::Pawn_black)] = 71'776'119'061'217'280;
	map[toUType(FigureType::Rook_white)] = 129;
	map[toUType(FigureType::Knight_white)] = 66;
	map[toUType(FigureType::Bishop_white)] = 36;
	map[toUType(FigureType::Queen_white)] = 8;
	map[toUType(FigureType::King_white)] = 16;
	map[toUType(FigureType::Pawn_white)] = 65'280;
	possibleCastling.fill(true);
	countOfMoves = 0;
}

Map::Map(const std::array<uint64_t, FIGURE_TYPES> _map) noexcept : Map()
{
	for (auto i = 0; i != FIGURE_TYPES; ++i)
		map[i] = _map[i];
}

Map::Map(const Map& baseMap) noexcept 
	: map(baseMap.map), possibleCastling(baseMap.possibleCastling), movesLog(baseMap.movesLog), countOfMoves(baseMap.countOfMoves) { }

Map& Map::operator=(const Map& baseMap) noexcept
{
	map = baseMap.map;
	possibleCastling = baseMap.possibleCastling;
	movesLog = baseMap.movesLog;
	countOfMoves = baseMap.countOfMoves;
	return *this;
}

GameStatus Map::CheckGameFinal(const Color &activePlayerColor)
{
	if (Figure::IsShahFor(activePlayerColor, map))
	{
		DisableCastlingForKing(activePlayerColor);
		DisableCastlingForRook(activePlayerColor);
		FindAllPossibleMoves(activePlayerColor);
		if (!GetAllPossibleMoves().empty())
			return GameStatus::Shah;
		return GameStatus::Mat;
	}
	else
	{
		DisableCastlingForRook(activePlayerColor);
		FindAllPossibleMoves(activePlayerColor);
		if (!GetAllPossibleMoves().empty())
			return GameStatus::Play;
		return GameStatus::Pat;
	}
}

std::vector<Pos> Map::GetPossibleMovesFrom(const Pos& figurePosition) const
{
	assert(figurePosition.IsValid());
	auto it{ allPossibleMoves.cbegin() }, end{ allPossibleMoves.cend() };
	std::vector<Pos> accessPosistions;
	for (; it != end; ++it)
		if ((*it)[0].GetPosBeforeMove() == figurePosition)
		{
			accessPosistions.push_back((*it)[0].GetPosAfterMove());
		}
	if (accessPosistions.empty())
		return {}; // returns empty vector
	return accessPosistions;
}

void Map::FindAllPossibleMoves(const Color& activeColor)
{
	auto copyMap{ *this };
	auto start{ activeColor == Color::Black ? 0 : 6 }; // used fixed enum order
	auto allMoves{ allPossibleMoves };
	uint64_t j{ 1 };
	while (j) // check all positions
	{
		for (auto i = start; i != start + 6; ++i)
			if (j & copyMap.map[i]) // is selected figure position
			{
				auto oneFigureMoves = Figure::FindPossibleMoves(Pos::BitboardToPosition(j), static_cast<FigureType>(i), copyMap); // find figure possible moves without checking shah
				allMoves.insert(allMoves.end(), oneFigureMoves.begin(), oneFigureMoves.end());
				break;
			}
		j <<= 1;
	}
	Figure::EraseForbiddenMoves(allMoves, copyMap);
	mut.lock();
	if (!allMoves.empty())
		allPossibleMoves = std::move(allMoves);
	mut.unlock();
}

MoveStatus Map::MakeMove(const Pos& previousPosition, const Pos& nextPosition, const FigureType selectedFigure)
{
	auto it{ allPossibleMoves.cbegin() }, end{ allPossibleMoves.cend() };
	MovesInfo moves;
	for (; it != end; ++it)
		if (((*it)[0].GetPosBeforeMove() == previousPosition && (*it)[0].GetPosAfterMove() == nextPosition) &&
			(selectedFigure == FigureType::Empty || selectedFigure == (*it)[1].GetTypeActiveFigure())) // search move in the vector of possible moves
			moves.push_back(*it);
	if (moves.size() == 1)
	{
		mut.lock();
		Move(moves[0]);
		mut.unlock();
		return MoveStatus::Made;
	}
	else if (moves.size() > 1)
		return MoveStatus::NeedFigureType;
	return MoveStatus::NotFound;
}

void Map::Move(std::vector<MoveInfo> move)
{
	++countOfMoves; // inc counter
	auto it = move.begin();
	for (; it != move.end(); ++it)
	{
		if ((*it).isEatenFigureExists())
			map[toUType((*it).GetTypeEatenFigure())] -= (*it).GetPosAfterMove().ToBitboard();
		if ((*it).GetPosBeforeMove().IsValid()) // used for pawn transform move
			map[toUType((*it).GetTypeActiveFigure())] -= (*it).GetPosBeforeMove().ToBitboard();
		map[toUType((*it).GetTypeActiveFigure())] += (*it).GetPosAfterMove().ToBitboard();
	}
	for (it = move.begin(); it != move.end(); ++it)
		movesLog.push_back(MoveInfo((*it).GetPosBeforeMove(), (*it).GetPosAfterMove(), (*it).GetTypeActiveFigure(), (*it).GetTypeEatenFigure(),
		 possibleCastling, (*it).GetCaptureEnPassant(), (*it).GetNumOfMove())); // save info about move
	it = move.begin();
	for (auto i = 0; i != 4; ++i)
		possibleCastling[i] = (*it).GetPossibleCastling()[i];
}

void Map::UndoMove()
{
	auto info{ GetLastMoveInfo() };
	for (auto i = 0; i != 4; ++i)
		possibleCastling[i] = info.GetPossibleCastling()[i];
	while (info != MoveInfo::NULL_INFO && info.GetNumOfMove() == countOfMoves)
	{
		movesLog.pop_back();
		if (info.isEatenFigureExists())
			map[toUType(info.GetTypeEatenFigure())] += info.GetPosAfterMove().ToBitboard();
		map[toUType(info.GetTypeActiveFigure())] -= info.GetPosAfterMove().ToBitboard();
		if (info.GetPosBeforeMove().IsValid()) // used for pawn transform move
			map[toUType(info.GetTypeActiveFigure())] += info.GetPosBeforeMove().ToBitboard();
		info = GetLastMoveInfo();
	}
	--countOfMoves; //dec counter
}

void Map::DoImitationMove(const Pos& from, const Pos& to) noexcept
{
	auto moved{ GetFigureType(from) };
	auto eaten{ GetFigureType(to) };
	if (eaten != FigureType::Empty)
		map[toUType(eaten)] -= to.ToBitboard();
	else if ((moved == FigureType::Pawn_black || moved == FigureType::Pawn_white)
		&& abs(from.GetX() - to.GetX()) == 1 && abs(from.GetY() - to.GetY()) == 1)
		map[toUType(moved == FigureType::Pawn_black ? FigureType::Pawn_white : FigureType::Pawn_black)]
		-= Pos{ to.GetX(), from.GetY() }.ToBitboard();
	map[toUType(moved)] -= from.ToBitboard();
	map[toUType(moved)] += to.ToBitboard();
}

void Map::UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType) noexcept
{
	auto moved{ GetFigureType(to) };
	auto eaten{ eatenType };
	if (eaten != FigureType::Empty)
		map[toUType(eaten)] += to.ToBitboard();
	else if ((moved == FigureType::Pawn_black || moved == FigureType::Pawn_white)
		&& abs(from.GetX() - to.GetX()) == 1 && abs(from.GetY() - to.GetY()) == 1)
		map[toUType(moved == FigureType::Pawn_black ? FigureType::Pawn_white : FigureType::Pawn_black)]
		+= Pos{ to.GetX(), from.GetY() }.ToBitboard();
	map[toUType(moved)] -= to.ToBitboard();
	map[toUType(moved)] += from.ToBitboard();
	
}

void Map::DisableCastlingForKing(const Color& kingColor) noexcept // if made a King move or was attacked disable possible castling
{
	auto kingCoeff{ static_cast<uint8_t>(2 * toUType(kingColor)) };
	possibleCastling[kingCoeff] = false;
	possibleCastling[kingCoeff + 1] = false;
}

void Map::DisableCastlingForRook(const Color& kingColor) noexcept // if Rook was eaten
{
	auto kingCoeff{ static_cast<uint8_t>(2 * toUType(kingColor)) };
	if (possibleCastling[kingCoeff] || possibleCastling[kingCoeff + 1])
	{
		if (kingColor == Color::White)
		{
			if (GetFigureType(Pos{ 0, 0 }) != FigureType::Rook_white)
				possibleCastling[kingCoeff] = false;
			if (GetFigureType(Pos{ 7, 0 }) != FigureType::Rook_white)
				possibleCastling[kingCoeff + 1] = false;
		}
		else
		{
			if (GetFigureType(Pos{ 0, 7 }) != FigureType::Rook_black)
				possibleCastling[kingCoeff] = false;
			if (GetFigureType(Pos{ 7, 7 }) != FigureType::Rook_black)
				possibleCastling[kingCoeff + 1] = false;
		}
	}
}

FigureType Map::GetFigureType(const int index) const noexcept
{
	auto bitboard{ 1ULL << index };
	for (auto i = 0; i != FIGURE_TYPES; ++i)
		if (map[i] & bitboard)
			return static_cast<FigureType>(i);
	return FigureType::Empty;
}

const MoveInfo Map::GetLastMoveInfo() const noexcept
{
	if (!movesLog.empty())
		return movesLog.back();
	return {};
}