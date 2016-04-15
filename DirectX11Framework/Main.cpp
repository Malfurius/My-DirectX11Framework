#include "Macros.h"
#include "Core.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int result = EXIT_FAILURE;
	Core* core = new Core();
	bool debug;
	debug = core->Initialize();

	if (debug)
		result = core->Run();

	SAFE_SHUTDOWN(core);

	return result;
}