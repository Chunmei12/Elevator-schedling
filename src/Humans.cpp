#include "MessageBus.h"
#include "Humans.h"
#include "assert.h"
#include <iomanip>

Human::Human(
	const unsigned int aFloor,
	const unsigned int aDestinationFloor)
	: myFloor(aFloor), myDestinationFloor(aDestinationFloor), myState(HumanState_Idle), myWaitingCounter(0), myTravelingCounter(0)
{
	if (myFloor == myDestinationFloor)
	{
		Log("[Human][Error] Same floor and destination floor: ", myFloor);
		assert(false);
	}
}

HumanState
Human::GetState() const
{
	return myState;
}

void Human::SetStateWaiting()
{
	myState = HumanState_Waiting;
}

void Human::SetStateTraveling()
{
	myState = HumanState_Traveling;
}

void Human::SetStateArrived()
{
	myState = HumanState_Arrived;
}

void Human::Step()
{
	if (myState == HumanState_Traveling)
	{
		++myTravelingCounter;
	}
	if (myState == HumanState_Waiting)
	{
		++myWaitingCounter;
	}
}

unsigned int
Human::GetWaitingCounter() const
{
	return myWaitingCounter;
}

unsigned int
Human::GetTravelingCounter() const
{
	return myTravelingCounter;
}

Humans::Humans()
{
}

void Humans::Start()
{
	REGISTER_HUMAN(MessageElevatorReady, Humans::OnMessageElevatorReady);
	REGISTER_HUMAN(MessageElevatorArrived, Humans::OnMessageElevatorArrived);
	REGISTER_HUMAN(MessageHumanStep, Humans::OnMessageHumanStep);

	//refuse the two same floor number, initial state
	myHumans.push_back(Human(1, 4));
}

void Humans::OnMessageElevatorReady(const MessageElevatorReady &aMessage)
{
	Log("[Humans] Elevator system ready");

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
}

void Humans::OnMessageElevatorArrived(const MessageElevatorArrived &aMessage)
{
	// Implement me!
	// deciding the next step depends on the human state
	for (Human &human : myHumans)
	{
		Direction humanDirection = (int)(human.myDestinationFloor - human.myFloor) > 0 ? Direction::Up : Direction::Down;
		if (human.GetState() == HumanState::HumanState_Waiting && human.myFloor == aMessage.myFloor && aMessage.myDirection == humanDirection)
		{
			//make a internal request
			MessageElevatorRequest message = {aMessage.myElevatorId, human.myDestinationFloor}; // make a internal request
			SEND_TO_ELEVATORS(message);

			//update the human state to traveling
			human.SetStateTraveling();

			//print the state updated
			Log(std::setw(40), "[Human State:Traveling(", human.myFloor, ",", human.myDestinationFloor, ")]", "Elevator ", aMessage.myElevatorId, " arrived at floor:", aMessage.myFloor);
		}
		else if (human.GetState() == HumanState::HumanState_Traveling && human.myDestinationFloor == aMessage.myFloor && aMessage.myDirection == humanDirection)
		{
			//update the human state to arrived
			human.SetStateArrived();
			
			//print the state updated
			Log(std::setw(40), "[Human State:Arrived(", human.myFloor, ",", human.myDestinationFloor, ")]", "Elevator ", aMessage.myElevatorId, " arrived at floor:", aMessage.myFloor);
		}
	}
}

void Humans::OnMessageHumanStep(const MessageHumanStep &aMessage)
{
	for (Human &human : myHumans)
	{
		// Implement me!
		if (human.GetState() == HumanState::HumanState_Idle)
		{
			// Human make a external call
			MessageElevatorCall messageCall = {human.myFloor, (int)(human.myDestinationFloor - human.myFloor) > 0 ? Direction::Up : Direction::Down}; //coding
			SEND_TO_ELEVATORS(messageCall);

			//set up the human state waitting
			human.SetStateWaiting();

			//print the state updated
			Log(std::setw(40), "[Human State:Waiting(", human.myFloor, ",", human.myDestinationFloor, ")]");
		}
		human.Step();
	}

	PrivPrintTimers();

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
}

void Humans::PrivPrintTimers()
{
	unsigned int sumWaiting = 0;
	unsigned int sumTraveling = 0;
	unsigned int humansArrivedCounter = 0;

	for (const Human &human : myHumans)
	{
		if (human.GetState() != HumanState_Arrived)
		{
			continue;
		}

		sumWaiting += human.GetWaitingCounter();
		sumTraveling += human.GetTravelingCounter();
		++humansArrivedCounter;
	}

	if (humansArrivedCounter == 0)
	{
		return;
	}

	Log(std::setw(100), "[Stats] Average wait time (steps): ", (sumWaiting / static_cast<float>(humansArrivedCounter)));
	Log(std::setw(100), "[Stats] Average travel time (steps): ", (sumTraveling / static_cast<float>(humansArrivedCounter)));
}

void Humans::GenerateHuman()
{
	//get different random number from one to ten
	srand(time(0));
	unsigned int srcfloor = rand() % 10 + 1;
	unsigned int destinatonFloor = rand() % 10 + 1;

	if (srcfloor != destinatonFloor)
	{
		//push the new human into the humans vector
		myHumans.push_back(Human(srcfloor, destinatonFloor));

		//print the state updated
		Log(std::setw(40), "[Generate New Human State:Idl(", srcfloor, ",", destinatonFloor, ")]");
	}
}