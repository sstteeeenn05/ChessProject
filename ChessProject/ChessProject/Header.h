/*****************************************************************
* File: Header.cpp
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
 * Update Date: 2023/05/07
 * Description: set the enum of Type and Color
 *********************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

enum Type
{
	KING='K',
	QUEEN='Q',
	BISHOP='B',
	KNIGHT='N',
	ROOK='R',
	PAWN='P',
	EMPTY=' '
};

enum Player
{
	WHITE,
	BLACK,
	NONE
};
