#include <Windows.h>
#include <thread>

#include <Exploit/Utils.hpp>
#include <Exploit/Globals.hpp>
#include <Communication/Communication.hpp>
#include <Exploit/TaskScheduler/TaskScheduler.hpp>

void MainThread()
{
    Communication::Initialize();

    while (true)
    {
        uintptr_t DataModel = TaskScheduler::GetDataModel();
        if (!DataModel)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
		}

        if (SharedVariables::LastDataModel != DataModel)
        {
            if (!Utils::IsInGame(DataModel))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }
			SharedVariables::ExecutionRequests.clear();

            TaskScheduler::SetupExploit();
            TaskScheduler::RequestExecution("print(\"YuB-X-Public successfully loaded\")");
			SharedVariables::LastDataModel = DataModel;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        std::thread(MainThread).detach();
    }
    return TRUE;
}

extern "C" __declspec(dllexport) int YuB_X(int nCode, WPARAM wParam, LPARAM lParam) { return CallNextHookEx(NULL, nCode, wParam, lParam); }