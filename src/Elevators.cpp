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

	myElevators.push_back(Elevator{1, 10, 10, Direction::Down});
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
	auto &minElevator  = myElevators.front();
	unsigned int minTimes;
	for (auto &elevator : myElevators)
	{
		int currentFloor = (int)elevator.CurrentFloor();
		int topFloor = elevator.GetRequestList().back();
		int bottomFloor = elevator.GetRequestList().front();
		int times;
		if (elevator.CurrentDirection() == Direction::Stop)
		{

			times = std::abs((int)aMessage.myFloor - currentFloor);
		}
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
				times = std::abs(2 * topFloor - (int)aMessage.myFloor - currentFloor);
			}
			if (aMessage.myDirection == Direction::Up)
			{
				times = std::abs((int)aMessage.myFloor + currentFloor - 2 * bottomFloor);
			}
			// unsigned int index = GetPoint(elevator.GetRequestList(), aMessage.myFloor);
		}
		if(minTimes > times) { minTimes = times; minElevator = elevator;}
	}
	minElevator.SelectFloor(aMessage.myFloor);
}

void Elevators::OnMessageElevatorRequest(const MessageElevatorRequest &aMessage)
{
	for (auto &elevator : myElevators)
	{
		if (elevator.Id() == aMessage.myElevatorId)
		{
			elevator.SelectFloor(aMessage.myFloor);
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