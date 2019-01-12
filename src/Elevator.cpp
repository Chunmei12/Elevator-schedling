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
//new function to insert the new call or request into the  up list
void Elevator::InsertUpList(const unsigned int aFloorId)
{
	upList.push_back(aFloorId);
	upList.sort();
}
//new function to insert the new call or request into the  down list
void Elevator::InsertDownList(const unsigned int aFloorId)
{
	downList.push_back(aFloorId);
	downList.sort();
}
//get up list
std::list<unsigned int> Elevator::UpList() const
{
	return upList;
}
//get down list
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
	if (!upList.empty() || !downList.empty())
	{
		return true;
	}
	return false;
	// Implement me!
}

void Elevator::Step()
{
	// Implement me!
	if (HasWork())
	{
		//down direction
		if (myCurrentDirection == Direction::Down)
		{
			//check if there is a call or request at current floor
			std::list<unsigned int>::iterator it = std::find(downList.begin(), downList.end(), myCurrentFloor); //searche
			while (it != downList.end())
			{
				downList.erase(it);
				MessageElevatorArrived message = {myId, myCurrentFloor};
				SEND_TO_HUMANS(message);
				it = std::find(downList.begin(), downList.end(), myCurrentFloor);
			}
			// change the elevator direction from down to up
			// 1. when the floor arrrived at the first floor
			// 2. no more down call or requst meanwhile no up call on the lower floor
			// 3. all the down call is above the current floor
			if (myCurrentFloor <= 1 || (!downList.empty() && downList.front() >= myCurrentFloor && upList.empty()) || (downList.empty() && !upList.empty() && upList.back() >= myCurrentFloor && upList.front() > myCurrentFloor))
			{
				myCurrentDirection = Direction::Up;
			}
			else
			{
				myCurrentFloor--;
			}
		}
		//up direction
		else if (myCurrentDirection == Direction::Up)
		{
			//check if there is a call or request at current floor
			std::list<unsigned int>::iterator it = std::find(upList.begin(), upList.end(), myCurrentFloor); //searche
			while (it != upList.end())
			{
				upList.erase(it);
				MessageElevatorArrived message = {myId, myCurrentFloor};
				SEND_TO_HUMANS(message);
				it = std::find(upList.begin(), upList.end(), myCurrentFloor);
			}

			// change the elevator direction from up to down
			// 1. the floor arrrived at the hightest floor
			// 2. no more up call or requst meanwhile no down call on the higher floor
			// 3. all the up call is below the current floor
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
}

unsigned int Elevator::Id() const
{
	return myId;
}

std::string
Elevator::ToString() const
{
	return "- State: " + std::to_string(myCurrentFloor) + "/" + std::to_string(myFloorCount) + " " + ::ToString(myCurrentDirection);
}
