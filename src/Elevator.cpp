#include "Elevator.h"
#include "MessageBus.h"

#include "Elevators.h"

//自定义排序函数
bool SortByM1(const unsigned int &v1, const unsigned int &v2) //注意：本函数的参数的类型一定要与vector中元素的类型一致
{
	return v1 > v2; //升序排列
}

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
	q.push_back(aFloorId);
	if (myCurrentDirection == Direction::Up)
	{
		std::sort(q.begin(), q.end(), SortByM1);
	}
	else
	{
		std::sort(q.begin(), q.end());
	}
	// Implement me!
}
std::vector<unsigned int> Elevator::GetRequestVector() const
{
	return q;
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
	if (q.size())
	{

		return true;
	}
	else
	{
		return false;
	}
	// Implement me!
}

void Elevator::Step()
{
	if (HasWork())
	{
		if (myCurrentDirection == Direction::Stop)
		{
			myCurrentDirection = (int)(q.back() - myCurrentFloor) > 0 ? Direction::Up : Direction::Down;
		}

		if (myCurrentDirection == Direction::Down)
		{
			myCurrentFloor--;
			if (myCurrentFloor == q.back())
			{
				q.pop_back();
				MessageElevatorArrived message = {myId, myCurrentFloor};
				SEND_TO_HUMANS(message);
			}
		}
		else if (myCurrentDirection == Direction::Up)
		{
			myCurrentFloor++;
			if (myCurrentFloor == q.back())
			{
				q.pop_back();
				MessageElevatorArrived message = {myId, myCurrentFloor};
				SEND_TO_HUMANS(message);
			}
		}
	}
	else
	{
		myCurrentDirection = Direction::Stop; //转变方向和停止该线程
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
