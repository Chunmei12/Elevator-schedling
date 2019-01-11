#include "MessageBus.h"
#include "Humans.h"
#include "assert.h"
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

///

Humans::Humans()
{
}

void Humans::Start()
{
	REGISTER_HUMAN(MessageElevatorReady, Humans::OnMessageElevatorReady);
	REGISTER_HUMAN(MessageElevatorArrived, Humans::OnMessageElevatorArrived);
	REGISTER_HUMAN(MessageHumanStep, Humans::OnMessageHumanStep);
	// REGISTER_HUMAN(MessageElevatorCall, Humans::OnMessageElevatorCall);

	myHumans.push_back(Human(1, 4)); //refuse the two same floor number
	// myHumans.push_back(Human(6, 4)); //refuse the two same floor number
	// myHumans.push_back(Human(3, 9)); //refuse the two same floor number
	// myHumans.push_back(Human(4, 5)); //refuse the two same floor number
	// myHumans.push_back(Human(7, 1)); //refuse the two same floor number
	// myHumans.push_back(Human(7, 9)); //refuse the two same floor number
}

void Humans::OnMessageElevatorReady(const MessageElevatorReady &aMessage)
{
	Log("[Humans] Elevator system ready");

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
//todo
	for (Human &human : myHumans)
	{
		if (human.GetState() == HumanState::HumanState_Idle)
		{

			MessageElevatorCall messageCall = {human.myFloor,(int) (human.myDestinationFloor - human.myFloor) > 0 ? Direction::Up : Direction::Down}; //coding
			SEND_TO_ELEVATORS(messageCall);
			human.SetStateWaiting();
		}
	}
}

void Humans::OnMessageElevatorArrived(const MessageElevatorArrived &aMessage)
{
	Log("[Humans] Elevator arrived at floor:", aMessage.myFloor);

	for (Human &human : myHumans)
	{
		if (human.GetState() == HumanState::HumanState_Waiting && human.myFloor == aMessage.myFloor)
		{
			MessageElevatorRequest message = {aMessage.myElevatorId, human.myDestinationFloor}; //coding
			SEND_TO_ELEVATORS(message);
			human.SetStateTraveling();
		}
		else if (human.GetState() == HumanState::HumanState_Traveling && human.myDestinationFloor == aMessage.myFloor)
		{
			human.SetStateArrived();
			//  delete human;
		}
	}

	// Implement me!
}

void Humans::OnMessageHumanStep(const MessageHumanStep &aMessage)
{
	Log("[Humans] Step");

	for (Human &human : myHumans)
	{

		human.Step();
	}

	PrivPrintTimers();

	// Implement me!

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

	Log("[Stats] Average wait time (steps): ", (sumWaiting / static_cast<float>(humansArrivedCounter)));
	Log("[Stats] Average travel time (steps): ", (sumTraveling / static_cast<float>(humansArrivedCounter)));
}
