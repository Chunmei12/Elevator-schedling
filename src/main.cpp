#include <condition_variable>
#include <mutex>
#include "Elevators.h"
#include "Humans.h"
#include "MessageBus.h"
#include "Threads.h"

Elevators elevators;
Humans humans;

void RunElevators()
{
	elevators.Start();
}

void RunHumans()
{
	humans.Start();
}
void RunGenerateHuman()
{
	humans.GenerateHuman();
}

int main()
{
	std::string setup = "no";
	Log("\n", "if you want to keep generating random humans: yes or no?");
	std::cin >> setup;

	Threads::GetInstance().Start();
	MessageBus::GetInstance();

	Threads::GetInstance().AddElevatorWork(std::bind(&RunElevators));
	Threads::GetInstance().AddHumanWork(std::bind(&RunHumans));
	if (setup == "yes")
	{
		Log(setup);

		Threads::GetInstance().Timer(std::bind(&RunGenerateHuman), 6000);
	}

	Threads::GetInstance().Wait();

	return 0;
}