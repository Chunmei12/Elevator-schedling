#include "MessageBus.h"
#include "stdlib.h"
#include "Elevators.h"
#include <algorithm>
using namespace std;
Elevators::Elevators()
{
}

void Elevators::Start()
{
	REGISTER_ELEVATOR(MessageElevatorCall, Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep, Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest, Elevators::OnMessageElevatorRequest);

	// myElevators.push_back(Elevator{1, 10, 6, Direction::Down});
	myElevators.push_back(Elevator{2, 10, 1, Direction::Up});
	// myElevators.push_back(Elevator{3, 10, 5, Direction::Up});
	{
		MessageElevatorStep message;
		SEND_TO_ELEVATORS(message);
	}

	{
		MessageElevatorReady message;
		SEND_TO_HUMANS(message);
	}
}

void Elevators::OnMessageElevatorCall(const MessageElevatorCall &aMessage)
{
	Log(aMessage.myFloor, aMessage.myDirection); //coding
	Scheduling(aMessage);						 //scheduling
												 // MessageElevatorRequest messageRequest = {e} SEND_TO_ELEVATORS(messageRequest)
												 // Implement me!
}

void Elevators::Scheduling(const MessageElevatorCall &aMessage)
{
	//chose one elevator
	unsigned int elevatorId;
	unsigned int minTimes = -1;
	for (auto &elevator : myElevators)
	{
		int currentFloor = (int)elevator.CurrentFloor();
		int topInUpList = elevator.UpList().empty() ? 0 : elevator.UpList().back();
		int bottomnUpList = elevator.UpList().empty() ? 0 : elevator.UpList().front();
		int topnDownList = elevator.DownList().empty() ? 0 : elevator.DownList().back();
		int bottomnDownList = elevator.DownList().empty() ? 0 : elevator.DownList().front();
		int times = -1;
		// if (elevator.CurrentDirection() == Direction::Stop)
		// {
		// 	times = std::abs((int)aMessage.myFloor - currentFloor);
		// }
		if (aMessage.myDirection == elevator.CurrentDirection())
		{
			if (aMessage.myDirection == Direction::Down)
			{
				if (aMessage.myFloor < elevator.CurrentFloor())
				{
					times = std::abs((int)aMessage.myFloor - currentFloor);
				}
			}
			if (aMessage.myDirection == Direction::Up)
			{
				if (aMessage.myFloor > elevator.CurrentFloor())
				{
					times = std::abs((int)aMessage.myFloor - currentFloor);
				}
			}
			// unsigned int index = GetPoint(elevator.GetRequestList(), aMessage.myFloor);
		}

		if (aMessage.myDirection != elevator.CurrentDirection())
		{
			if (aMessage.myDirection == Direction::Down)
			{
				if (topInUpList == 0)
				{
					times = std::abs(currentFloor - (int)aMessage.myFloor);
				}
				else
				{
					times = std::abs(2 * topInUpList - currentFloor - (int)aMessage.myFloor);
				}
			}
			if (aMessage.myDirection == Direction::Up)
			{
				if (topInUpList == 0)
				{
					times = std::abs(currentFloor - (int)aMessage.myFloor);
				}
				else
				{
					times = std::abs(currentFloor + (int)aMessage.myFloor - 2 * bottomnDownList);
				}
			}
			// unsigned int index = GetPoint(elevator.GetRequestList(), aMessage.myFloor);
		}
		if (minTimes > times || minTimes == -1)
		{
			minTimes = times;
			elevatorId = elevator.Id();
		}
		// elevator.InsertUpList(aMessage.myFloor); //delete
	}
	for (auto &elevator : myElevators)
	{
		if (elevator.Id() == elevatorId)
		{
			if (aMessage.myDirection == Direction::Up)
			{
				elevator.InsertUpList(aMessage.myFloor);
			}
			else
			{
				elevator.InsertDownList(aMessage.myFloor);
			}
		}
	}
}

void Elevators::OnMessageElevatorRequest(const MessageElevatorRequest &aMessage)
{
	for (auto &elevator : myElevators)
	{
		if (elevator.Id() == aMessage.myElevatorId)
		{
			if (elevator.CurrentFloor() < aMessage.myFloor)
			{
				elevator.InsertUpList(aMessage.myFloor);
			}
			else
			{
				elevator.InsertDownList(aMessage.myFloor);
			}
			break;
		}
	}
	// Implement me!
}

void Elevators::OnMessageElevatorStep(const MessageElevatorStep &aMessage)
{

	for (auto &elevator : myElevators)
	{
		Log("[Elevators", elevator.Id(), "] Step", elevator.ToString());
		elevator.Step();
	}

	MessageElevatorStep message;
	SEND_TO_ELEVATORS(message);
}