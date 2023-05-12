#pragma once

struct Position {
	int x, y;
	bool operator==(const Position& p) {
		return p.x == x && p.y == y;
	}
	Position operator+(const Position& p) {
		return { p.x + x, p.y + y };
	}
	Position operator-(const Position& p) {
		return { x - p.x, y - p.y };
	}
	bool valid() {
		return x >= 0 && x < 8 && y >= 0 && y < 8;
	}
};