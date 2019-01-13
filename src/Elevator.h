#pragma once

#include "Utils.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <list>
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

	unsigned int CurrentFloor() const;
	Direction CurrentDirection() const;

	std::list<unsigned int> UpList() const;  //get up list
	std::list<unsigned int> DownList() const; //get down list

	void InsertUpList(const unsigned int aFloorId); //insert one into the up list
	void InsertDownList(const unsigned int aFloorId); //insert one into the down list
	
	bool HasWork() const;
	void Step();
	unsigned int Id() const;

	std::string ToString() const;

  private:
	unsigned int myId;
	unsigned int myFloorCount;
	unsigned int myCurrentFloor;
	std::list<unsigned int> upList;  // up list
	std::list<unsigned int> downList; //down list
	Direction myCurrentDirection;
};
