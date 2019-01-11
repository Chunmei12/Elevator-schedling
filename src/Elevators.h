#pragma once

#include <mutex>
#include <vector>
#include "Elevator.h"
#include "Messages.h"
#include "Sorts.h"
enum Direction;
class Elevators
{
  public:
	Elevators();

	void Start();

	void OnMessageElevatorCall(
		const MessageElevatorCall &aMessage);
	void OnMessageElevatorRequest(
		const MessageElevatorRequest &aMessage);

	void OnMessageElevatorStep(
		const MessageElevatorStep &aMessage);

  void Scheduling(const MessageElevatorCall &aMessage);
  private:
	std::vector<Elevator> myElevators;
};
