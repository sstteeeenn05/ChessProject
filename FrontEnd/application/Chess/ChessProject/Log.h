#pragma once

#include <utility>
#include <vector>
#include "DataType.h"
#include "Position.h"

struct ChessData {
	Player player = NONE, enemy = NONE;
	Position position;
	Type type = EMPTY;
	bool enPassanted = false;
	bool moved = false;
	ChessData() = default;
	ChessData(Position pos):position(pos){}
	ChessData(Position pos, ChessData data){
		*this = data;
		position = pos;
	}
	ChessData(Type t, ChessData data) {
		*this = data;
		type = t;
	}
	ChessData(Player p, Position pos, Type t):
		player(p),
		enemy(p==WHITE?BLACK:WHITE),
		position(pos),
		type(t) {}
	void setType(Type t) {
		type = t;
	}
	void set(ChessData data) {
		Position pos = position;
		*this = data;
		position = pos;
	}
	void clear() {
		player = NONE;
		enemy = NONE;
		type = EMPTY;
		enPassanted = false;
		moved = false;
	}
	ChessData previewSetType(Type t) {
		return ChessData(t, *this);
	}
	ChessData previewSet(ChessData data) {
		return ChessData(position, data);
	}
	ChessData previewClear() {
		return ChessData(position);
	}
};

struct Log {
	std::vector<std::pair<ChessData, ChessData>> changeList;
};