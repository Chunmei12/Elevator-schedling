#include "Elevator.h"
#include "MessageBus.h"

#include "Elevators.h"


Elevator::Elevator(
	const unsigned int aId,
	const unsigned int aFloorCount)
	: myId(aId), myFloorCount(aFloorCount), myCurrentFloor(1), myCurrentDirection(Direction::Up)
{
	Log("[Elevator]", myId, "Created", ToString());
}

Elevator::Elevator(
	const unsigned int aId,
	const unsigned int aFloorCount,
	const unsigned int aCurrentFloor,
	const Direction aDirection)
	: myId(aId), myFloorCount(aFloorCount), myCurrentFloor(aCurrentFloor), myCurrentDirection(aDirection)
{
	Log("[Elevator]", myId, "Created", ToString());
}

void Elevator::InsertUpList(const unsigned int aFloorId)
{
	upList.push_back(aFloorId);
	upList.sort(); 
}
void Elevator::InsertDownList(const unsigned int aFloorId)
{
	downList.push_back(aFloorId);
	downList.sort(); 
}
std::list<unsigned int> Elevator::UpList() const
{
	return upList;
}
std::list<unsigned int> Elevator::DownList() const
{
	return downList;
}
unsigned int
Elevator::CurrentFloor() const
{
	return myCurrentFloor;
}

Direction
Elevator::CurrentDirection() const
{
	return myCurrentDirection;
}

bool Elevator::HasWork() const
{

	return false;
	// Implement me!
}

void Elevator::Step()
{
	if (!upList.empty() || !downList.empty())
	{
		if (myCurrentDirection == Direction::Down)
		{
			std::list<unsigned int>::iterator it = std::find(downList.begin(), downList.end(), myCurrentFloor); //searche
			while (it != downList.end())
			{

				downList.erase(it);
				MessageElevatorArrived message = {myId, myCurrentFloor};
				SEND_TO_HUMANS(message);
				it = std::find(downList.begin(), downList.end(), myCurrentFloor);
			}

			if (myCurrentFloor <= 1 || (downList.empty() && !upList.empty() && upList.back() > myCurrentFloor && upList.front() > myCurrentFloor))
			{
				myCurrentDirection = Direction::Up;
			}
			else if (!downList.empty() && downList.front() >= myCurrentFloor && upList.empty())
			{
				myCurrentDirection = Direction::Up;
			}
			else
			{
				myCurrentFloor--;
			}
		}
		else if (myCurrentDirection == Direction::Up)
		{
			std::list<unsigned int>::iterator it = std::find(upList.begin(), upList.end(), myCurrentFloor); //searche
			while (it != upList.end())
			{
				upList.erase(it);
				MessageElevatorArrived message = {myId, myCurrentFloor};
				SEND_TO_HUMANS(message);
				it = std::find(upList.begin(), upList.end(), myCurrentFloor);
			}
			if (myCurrentFloor >= myFloorCount || (upList.empty() && !downList.empty() && downList.front() < myCurrentFloor && downList.back() < myCurrentFloor))
			{
				myCurrentDirection = Direction::Down;
			}
			else if (!upList.empty() && upList.back() <= myCurrentFloor && downList.empty())
			{
				myCurrentDirection = Direction::Down;
			}
			else 
			{
				myCurrentFloor++;
			}
		}
	}
	// Implement me!
}

unsigned int
Elevator::Id() const
{
	return myId;
}

std::string
Elevator::ToString() const
{
	return "- State: " + std::to_string(myCurrentFloor) + "/" + std::to_string(myFloorCount) + " " + ::ToString(myCurrentDirection);
}
