#include "Chess.h"

std::vector<Log> Chess::logList;
int Chess::logIndex = 0;
Position Chess::wKing, Chess::bKing;
Chess Chess::board[8][8];

Chess::Chess(Position pos):data(ChessData(pos)) {}

Chess::Chess(Player p, Position pos, Type t) :data(p, pos, t) {}

void Chess::init() {
	Type typeList[8] = {ROOK,KNIGHT,BISHOP,QUEEN,KING,BISHOP,KNIGHT,ROOK};
	for (int i = 0; i < 8; i++) board[0][i] = Chess(BLACK, { i, 0 }, typeList[i]);
	for (int i = 0; i < 8; i++) board[1][i] = Chess(BLACK, { i, 1 }, PAWN);
	for (int i = 2; i < 6; i++) for (int j = 0; j < 8; j++) board[i][j] = Chess({ j, i });
	for (int i = 0; i < 8; i++) board[6][i] = Chess(WHITE, { i, 6 }, PAWN);
	for (int i = 0; i < 8; i++) board[7][i] = Chess(WHITE, { i, 7 }, typeList[i]);
}

std::string Chess::getBoard(){
	std::string output;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++) {
			Chess& c = board[i][j];
			if (c.data.type == EMPTY || c.data.player == WHITE) output += char(c.data.type);
			else output += char(c.data.type + ('a' - 'A'));
		}
		//output += '\n';
	}
	return output;
}

std::string Chess::getMaskBoard(Position target){
	std::string output;
	std::vector<Position> posList = getValidPos(target);
	bool maskBoard[8][8] = { false };
	for (const auto& pos : posList) {
		maskBoard[pos.y][pos.x] = true;
	}
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++) output += '0' + maskBoard[i][j];
		//output += '\n';
	}
	return output;
}

bool Chess::canUndo() {
	return logIndex > 0;
}

bool Chess::canRedo(){
	return logIndex < logList.size();
}

bool Chess::undo() {
	if (!canUndo()) return false;
	logIndex--;
	for (const auto& change : logList[logIndex].changeList) {
		const auto& before = change.first;
		const auto& after = change.second;
		board[after.position.y][after.position.x].data = before;
	}
	return true;
}

bool Chess::redo() {
	if (!canRedo) return false;
	logIndex++;
	for (const auto& change : logList[logIndex].changeList) {
		const auto& before = change.first;
		const auto& after = change.second;
		board[before.position.y][before.position.x].data = after;
	}
	return true;
}

std::vector<Position> Chess::getValidPos(Position target){
	std::vector<Position> output;
	Chess& chess = board[target.y][target.x];
	switch (chess.data.type) {
		case KING:
			chess.checkSquare(output);
			break;
		case QUEEN:
			chess.checkStraight(output);
			chess.checkCross(output);
			break;
		case BISHOP:
			chess.checkCross(output);
			break;
		case KNIGHT:
			chess.checkL(output);
			break;
		case ROOK:
			chess.checkStraight(output);
			break;
		case PAWN:
			chess.checkPawn(output);
			break;
	}
	return output;
}

bool Chess::checkValid(Position pos, std::vector<Position>& output) {
	const Chess& chess = board[pos.y][pos.x];
	if (chess.data.player != data.player)
		output.push_back({ pos.x, pos.y });
	return chess.data.player == NONE;
}

void Chess::checkStraight(std::vector<Position>& output) {
	for (int nowX = data.position.x + 1; nowX < 8; nowX++) {
		if (!checkValid({ nowX, data.position.y }, output)) break;
	}
	for (int nowX = data.position.x - 1; nowX >=0; nowX--) {
		if (!checkValid({ nowX, data.position.y }, output)) break;
	}
	for (int nowY = data.position.y + 1; nowY < 8; nowY++) {
		if (!checkValid({ data.position.x, nowY }, output)) break;
	}
	for (int nowY = data.position.y - 1; nowY >= 0; nowY--) {
		if (!checkValid({ data.position.x, nowY }, output)) break;
	}
}

void Chess::checkCross(std::vector<Position>& output) {
	for (int nowX = data.position.x + 1, nowY = data.position.y + 1; nowX < 8 && nowY < 8; nowX++, nowY++) {
		if (!checkValid({ nowX, nowY }, output)) break;
	}
	for (int nowX = data.position.x - 1, nowY = data.position.y + 1; nowX >=0 && nowY < 8; nowX--, nowY++) {
		if (!checkValid({ nowX, nowY }, output)) break;
	}
	for (int nowX = data.position.x - 1, nowY = data.position.y - 1; nowX >=0 && nowY >=0; nowX--, nowY--) {
		if (!checkValid({ nowX, nowY }, output)) break;
	}
	for (int nowX = data.position.x + 1, nowY = data.position.y - 1; nowX < 8 && nowY >= 0; nowX++, nowY--) {
		if (!checkValid({ nowX, nowY }, output)) break;
	}
}

void Chess::checkSquare(std::vector<Position>& output) {
	for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
		if (dy || dx) {
			Position p = data.position + Position{ dx, dy };
			if(p.valid()) checkValid(p, output);
		}
	}
}

void Chess::checkL(std::vector<Position>& output) {
	Position posList[8] = {
		{1,2},
		{-1,2},
		{-1,-2},
		{1,-2},
		{2,1},
		{-2,1},
		{-2,-1},
		{2,-1}
	};
	for (const auto& pos : posList) {
		Position p = data.position + pos;
		if(p.valid()) checkValid(p, output);
	}
}

void Chess::checkPawn(std::vector<Position>& output) {
	const auto generatePosByPlayer = [&](int x, int y)->Position {
		return data.position + Position{ x, data.player == WHITE ? y * -1 : y };
	};
	if (!data.moved) checkValid(generatePosByPlayer(0, 2), output);
	checkValid(generatePosByPlayer(0, 1), output);
	Position leftUp = generatePosByPlayer(1, 1);
	Position rightUp = generatePosByPlayer(-1, 1);
	if (leftUp.valid() && board[leftUp.y][leftUp.x].data.type != EMPTY)
		checkValid(leftUp, output);
	if (rightUp.valid() && board[rightUp.y][rightUp.x].data.type != EMPTY)
		checkValid(rightUp, output);
}

bool Chess::canMove(Position source, Position target) {
	std::vector<Position> validPos = getValidPos(source);
	for (const auto& pos : validPos) if (target == pos) return true;
	return false;
}

bool Chess::move(Player player, Position source, Position target) {
	if (player != getNowPlayer()) return false;
	if (!canMove(source, target)) return false;
	while (logList.size() > logIndex) logList.pop_back();
	Log log;
	Chess& sChess = board[source.y][source.x];
	Chess& tChess = board[target.y][target.x];

	const auto generatePosByPlayer = [&](int x, int y)->Position {
		return sChess.data.position + Position{ x, sChess.data.player == WHITE ? y * -1 : y };
	};
	if (sChess.data.type==PAWN && !sChess.data.enPassanted) {
		if ((sChess.data.player == WHITE && sChess.data.position.y == 3) ||
			(sChess.data.player == BLACK && sChess.data.position.y == 4)) {
			Chess& left = board[sChess.data.position.y][sChess.data.position.x - 1];
			Chess&right = board[sChess.data.position.y][sChess.data.position.x + 1];
			if (left.data.type == PAWN || right.data.type == PAWN) {
				sChess.data.enPassanted = true;
				if (target == generatePosByPlayer(0, 1) + left.data.position) {
					log.changeList.push_back({ left.data, left.data.previewClear() });
					left.data.clear();
				}
				if (target == generatePosByPlayer(0, 1) + right.data.position) {
					log.changeList.push_back({ right.data, right.data.previewClear() });
					right.data.clear();
				}
			}
		}
	}

	log.changeList.push_back({ sChess.data, sChess.data.previewClear()});
	log.changeList.push_back({ tChess.data, tChess.data.previewSet(sChess.data) });
	tChess.data.set(sChess.data);
	sChess.data.clear();

	if (tChess.checkPromotion()) {
		Type promotion = tChess.doPromotion();
		log.changeList.push_back({ tChess.data, tChess.data.previewSetType(promotion) });
		tChess.data.setType(promotion);
	}

	tChess.data.moved = true;

	logList.push_back(log);

	return true;
}

bool Chess::checkPromotion(){
	return data.type == PAWN && 
		(data.player == WHITE && data.position.y == 0 ||
			data.player == BLACK && data.position.y == 7);
}

Type Chess::doPromotion() {
	int choose = 0;
	do {
		std::cout << "playing;" << (data.player == WHITE ? "white" : "black") << ";;;promotion";
		std::cin >> choose;
		if (choose == 1) return QUEEN;
		if (choose == 2) return BISHOP;
		if (choose == 3) return KNIGHT;
		if (choose == 4) return ROOK;
		std::cout << "playing;" << (data.player == WHITE ? "white" : "black") << ";;;failed";
	} while (choose < 1 && choose>4);
}

Player Chess::getNowPlayer() {
	return logIndex & 1 ? BLACK : WHITE;
}

Player Chess::getNowEnemy(){
	return logIndex & 1 ? WHITE : BLACK;
}
