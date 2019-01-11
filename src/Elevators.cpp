#include "MessageBus.h"
#include "stdlib.h"
#include "Elevators.h"
#include<algorithm>
using namespace std;
Elevators::Elevators()
{
}

void Elevators::Start()
{
	REGISTER_ELEVATOR(MessageElevatorCall, Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep, Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest, Elevators::OnMessageElevatorRequest);

	myElevators.push_back(Elevator{1, 10, 6, Direction::Down});
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
	unsigned int min;
	for (auto &elevator : myElevators)
	{
		if (aMessage.myDirection == elevator.CurrentDirection() || elevator.CurrentDirection() == Direction::Stop)
		{
			vector<unsigned int>::iterator location = lower_bound(elevator.GetRequestVector().begin(),elevator.GetRequestVector().end(), aMessage.myFloor);
			 unsigned int abss =   abs(int(elevator.CurrentDirection() - aMessage.myFloor));
			 unsigned int costs = abss + location; //- elevator.GetRequestVector().begin();
				if(min > costs ) min = costs;
		}

	}
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