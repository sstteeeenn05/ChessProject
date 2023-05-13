#pragma once

struct Position {
	int x, y;
	bool operator==(const Position& p) const {
		return p.x == x && p.y == y;
	}
	Position operator+(const Position& p) const {
		return { p.x + x, p.y + y };
	}
	Position operator-(const Position& p) const {
		return { x - p.x, y - p.y };
	}
	[[nodiscard]] bool valid() const {
		return x >= 0 && x < 8 && y >= 0 && y < 8;
	}
};