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

void func()
{
	std::string setup = "no";
	Log("\n", "If you want to keep generating random humans: yes or no?");
	std::cin >> setup;

	Threads::GetInstance().Start();
	MessageBus::GetInstance();

	Threads::GetInstance().AddElevatorWork(std::bind(&RunElevators));
	Threads::GetInstance().AddHumanWork(std::bind(&RunHumans));

	//keep generating random humans so it runs infinitely
	if (setup == "yes")
	{
		Threads::GetInstance().Timer(std::bind(&RunGenerateHuman), 6000);
	}

	Threads::GetInstance().Wait();
}

//Threads class to be able to execute work on a specific thread after a specific delay
void SpecificThread(std::function<void()> func, unsigned int interval)
{
	std::thread([func, interval]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		func();
	})
		.join();
}

int main()
{

	Log("\n", "Bonus assignments : execute work on a specific thread after a 6000 milliseconds delay");
	SpecificThread(func, 6000);

	return 0;
}