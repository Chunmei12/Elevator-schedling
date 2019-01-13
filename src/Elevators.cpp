#include <algorithm>
#include "stdlib.h"
#include "MessageBus.h"
#include "Elevators.h"
#include "Threads.h"
using namespace std;
Elevators::Elevators()
{
}

void Elevators::Start()
{
	REGISTER_ELEVATOR(MessageElevatorCall, Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep, Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest, Elevators::OnMessageElevatorRequest);

	//initialize 3 elevator
	myElevators.push_back(Elevator{1, 10, 6, Direction::Down});
	myElevators.push_back(Elevator{2, 10, 1, Direction::Up});
	myElevators.push_back(Elevator{3, 10, 8, Direction::Down});

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
	Scheduling(aMessage);
}

void Elevators::Scheduling(const MessageElevatorCall &aMessage)
{
	unsigned int elevatorId = 0;
	int minTimes = -1;

	int currentFloor;
	int currentDirection;

	for (auto &elevator : myElevators)
	{
		currentFloor = (int)elevator.CurrentFloor();
		currentDirection = elevator.CurrentDirection();

		int topInUpList = elevator.UpList().empty() ? 0 : elevator.UpList().back();
		int bottomInUpList = elevator.UpList().empty() ? 0 : elevator.UpList().front();
		int topInDownList = elevator.DownList().empty() ? 0 : elevator.DownList().back();
		int bottomInDownList = elevator.DownList().empty() ? 0 : elevator.DownList().front();

		int times = -1;

		// FD-SCAN ALGORITHEM
		bool condition1 = (aMessage.myDirection == currentDirection && aMessage.myDirection == Direction::Down && aMessage.myFloor <= currentFloor) || (aMessage.myDirection == currentDirection && aMessage.myDirection == Direction::Up && aMessage.myFloor >= currentFloor) || (aMessage.myDirection != currentDirection && aMessage.myDirection == Direction::Down && topInUpList == 0) || (aMessage.myDirection != currentDirection && aMessage.myDirection == Direction::Up && bottomInDownList == 0);
		bool condition2 = (aMessage.myDirection == currentDirection && aMessage.myDirection == Direction::Down && aMessage.myFloor > currentFloor) || (aMessage.myDirection == currentDirection && aMessage.myDirection == Direction::Up && aMessage.myFloor < currentFloor);
		bool condition3 = (aMessage.myDirection != currentDirection && aMessage.myDirection == Direction::Down && topInUpList != 0);
		bool condition4 = (aMessage.myDirection != currentDirection && aMessage.myDirection == Direction::Up && bottomInDownList != 0);

		if (condition1)
		{
			times = std::abs((int)aMessage.myFloor - currentFloor);
		}
		else if (condition2)
		{
			times = std::abs((int)aMessage.myFloor - currentFloor) + std::abs(topInDownList - bottomInDownList + topInUpList - bottomInUpList);
		}
		else if (condition3)
		{
			times = std::abs(2 * topInUpList - currentFloor - (int)aMessage.myFloor);
		}
		else if (condition4)
		{
			times = std::abs(currentFloor + (int)aMessage.myFloor - 2 * bottomInDownList);
		}
		
		//get the elevator who costs the minimum of time
		if (minTimes > times || minTimes == -1)
		{
			minTimes = times;
			elevatorId = elevator.Id();
		}
	}

	// insert the new external call into the elevator list was selected before
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
			break;
		}
	}
}

void Elevators::OnMessageElevatorRequest(const MessageElevatorRequest &aMessage)
{
	// Implement me!
	int currentFloor;
	int currentDirection;

	for (auto &elevator : myElevators)
	{
		currentFloor = elevator.CurrentFloor();
		currentDirection = elevator.CurrentDirection();

		//make a up request
		if (elevator.Id() == aMessage.myElevatorId && currentDirection == Direction::Up)
		{
			elevator.InsertUpList(aMessage.myFloor);
			break;
		}

		//make a down request
		if (elevator.Id() == aMessage.myElevatorId && currentDirection == Direction::Down)
		{
			elevator.InsertDownList(aMessage.myFloor);
			break;
		}
	}
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