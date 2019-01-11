#pragma once

#include <string>
// #include "Elevator.h"  it's a bug, the enum Direction o the head file is badly included 

enum Direction
{
	Up = 0,
	Down,
	Stop  //add a state
};

class MessageElevatorReady
{
};

class MessageElevatorStep
{
};

class MessageElevatorCall
{
  public:
	unsigned int myFloor;
	Direction myDirection;
};

class MessageElevatorRequest
{
  public:
	unsigned int myElevatorId;
	unsigned int myFloor;
};

class MessageElevatorArrived
{
  public:
	unsigned int myElevatorId;
	unsigned int myFloor;
};

class MessageHumanStep
{
};

static std::string
ToString(
	const Direction aDirection)
{
	switch (aDirection)
	{
	case Direction::Up:
		return "Up";
	case Direction::Down:
		return "Down";
	default:
		return "Unknown";
		break;
	}
}