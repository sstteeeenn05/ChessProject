/*****************************************************************
* File: Position.h
 * Author: §d²»ºö¡B´¿±R®¦¡BÃ¹¬ý²M¡B­ð¤_µx
 * Create Date: 2023/05/05
 * Editor: ´¿±R®¦¡BÃ¹¬ý²M
 * Update Date: 2023/05/24
 * Description: the declaration of Position
 *********************************************************************/

#pragma once

struct Position {
	int x, y;
	bool operator==(const Position& p) const { //Overload operation ==
		return p.x == x && p.y == y;
	}
	Position operator+(const Position& p) const { //Overload operation +
		return { p.x + x, p.y + y };
	}
	Position operator-(const Position& p) const { //Overload operation -
		return { x - p.x, y - p.y };
	}
	[[nodiscard]] bool valid() const { //Check if it is in the board range
		return x >= 0 && x < 8 && y >= 0 && y < 8;
	}
};