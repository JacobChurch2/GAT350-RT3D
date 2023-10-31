#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{
	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World05>();
	world->Initialize();

	string test = "The Quick Brown Fox Jumped Over The Lazy Dog 123!@#";

	std::cout << test << std::endl;

	std::cout << nc::StringUtils::ToUpper(test) << std::endl;

	std::cout << nc::StringUtils::ToLower(test) << std::endl;

	if (nc::StringUtils::IsEqualIgnoreCase(test, nc::StringUtils::ToLower(test))) {
		std::cout << "Equal works" << std::endl;
	}

	if (!nc::StringUtils::IsEqualIgnoreCase(test, "The Quick Browe Fox Jumped Over The Lazy Dog 123!@#")) {
		std::cout << "Equal works" << std::endl;
	}

	std::cout << nc::StringUtils::CreateUnique(test) << endl;
	std::cout << nc::StringUtils::CreateUnique(test) << endl;
	std::cout << nc::StringUtils::CreateUnique(test) << endl;

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
