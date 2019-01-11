#include "Elevator.h"
#include "MessageBus.h"

#include "Elevators.h"

// //自定义排序函数
// bool SortByM1(const unsigned int &v1, const unsigned int &v2) //注意：本函数的参数的类型一定要与vector中元素的类型一致
// {
// 	return v1 > v2; //升序排列
// }

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

void Elevator::SelectFloor(const unsigned int aFloorId)
{
	// q.push_back(aFloorId);
	// q.sort(); //todo:insert

	// Implement me!
}
void Elevator::InsertUpList(const unsigned int aFloorId)
{
	upList.push_back(aFloorId);
	upList.sort(); //todo:insert
}
void Elevator::InsertDownList(const unsigned int aFloorId)
{
	downList.push_back(aFloorId);
	downList.sort(); //todo:insert
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

	// if (q.empty() || (myCurrentDirection == Direction::Down && (int)myCurrentFloor <= 1) || (myCurrentDirection == Direction::Up && myCurrentFloor >= myFloorCount))
	// {
	return false;
	// }
	// else if (q.size())
	// {

	// return true;
	// }

	// Implement me!
}

void Elevator::Step()
{
	
	if (myCurrentDirection == Direction::Down)
	{

		if (myCurrentFloor == downList.back())
		{
			downList.pop_back();
			// if (!HasWork())
			// 	myCurrentDirection = Direction::Up;
			MessageElevatorArrived message = {myId, myCurrentFloor};
			SEND_TO_HUMANS(message);
		}
		if (myCurrentFloor > 1 && !downList.empty())
		{
			myCurrentFloor--;
		}
		if (myCurrentFloor <= 1 || downList.empty())
		{
			if (!downList.empty() && (upList.empty() || upList.back() < downList.back()))
			{
				// auto temp = downList;
				upList.push_back(downList.back());
			}
			myCurrentDirection = Direction::Up;
		}
		// if (q.empty())
		// 	myCurrentDirection = Direction::Up;
	}
	else if (myCurrentDirection == Direction::Up)
	{
		// if (!HasWork())
		// 	myCurrentDirection = Direction::Down;

		if (myCurrentFloor == upList.front())
		{
			upList.pop_front();

			MessageElevatorArrived message = {myId, myCurrentFloor};
			SEND_TO_HUMANS(message);
		}
		if (myCurrentFloor < myFloorCount && !upList.empty())
		{
			myCurrentFloor++;
		}
		if (myCurrentFloor >= myFloorCount || upList.empty())
		{
			if (!upList.empty() && (downList.empty() || downList.front() > upList.front()))
			{
				auto temp = upList;
				downList.push_front(upList.front());
			}
			myCurrentDirection = Direction::Down;
		}

		// if (q.empty())
		// 	myCurrentDirection = Direction::Down;
	}
	// }
	// else
	// {
	// 	myCurrentDirection = Direction::Stop; //转变方向和停止该线程
	// }
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
