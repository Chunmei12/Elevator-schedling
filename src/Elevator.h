#pragma once

#include "Utils.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Messages.h"

enum Direction;

class Elevator
{
  public:
	Elevator(
		const unsigned int aId,
		const unsigned int aFloorCount);

	Elevator(
		const unsigned int aId,
		const unsigned int aFloorCount,
		const unsigned int aCurrentFloor,
		const Direction aDirection);

	void SelectFloor(const unsigned int aFloorId);
	unsigned int CurrentFloor() const;
	std::vector <unsigned int>  GetRequestVector() const;
	Direction CurrentDirection() const;
	bool HasWork() const;

	void Step();
	unsigned int Id() const;

	std::string ToString() const;

  private:
	unsigned int myId;
	unsigned int myFloorCount;
	unsigned int myCurrentFloor;
	std::vector <unsigned int> q;
	Direction myCurrentDirection;
};
