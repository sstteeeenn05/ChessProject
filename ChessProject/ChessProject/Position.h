/*****************************************************************
* File: Position.h
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
 * Update Date: 2023/05/07
 * Description: prototype of the Position
 *********************************************************************/

#pragma once

class Position
{
public:

	int x;
	int y;

	Position(); //constructor and initialize position
	Position(int x, int y); //constructor and initialize position

	void setPos(int x, int y); //set position
	Position getPos(); //get position

	bool operator==(Position&); //overload operator==
};