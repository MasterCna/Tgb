#include "Scan.h"
#include <process.h>

std::string chosenColor;
void MainScan(ScanContents scan);
void CheckColor(COLORREF pixel, int x, int y);
bool ColorMatch(COLORREF pixel);
void ScanThread(ScanContents* scan);

int main()
{

	//Test
	std::string gameWindow;
	std::cout << "Enter Game Window To TriggerBot" << std::endl;

	std::getline(std::cin, gameWindow);
	HWND appWnd = FindWindow(0, gameWindow.c_str());
	RECT rcClientPositioing;

	// Finding the Game
	while (!appWnd)
	{
		system("CLS");
		appWnd = FindWindow(0, gameWindow.c_str());
		std::cout << "Looking for " << gameWindow << std::endl;
		Sleep(100);
	}
	std::cout << "Found " << gameWindow << std::endl;

	// Choose color options
	while (atoi(chosenColor.c_str()) < 1 || atoi(chosenColor.c_str()) > 3)
	{
		system("CLS");
		std::cout << "Choose which color to trigger against\n" <<
			"1.Red\n2.Green\n3.Blue"
			<< std::endl;
		std::getline(std::cin, chosenColor);
	}

	// Color operations
	std::string color;
	if (chosenColor == "1")
		color = "Red";
	else if (chosenColor == "2")
		color = "Green";
	else if (chosenColor == "3")
		color = "Blue";
	system("CLS");

	// Succifully Status
	std::cout << "TriggerBot ONLINE, Hover Over " << color << " To Shoot...!" << std::endl;

	GetWindowRect(appWnd, &rcClientPositioing);

	HDC hdcMain = GetDC(HWND_DESKTOP);
	HDC hdcThread1 = GetDC(HWND_DESKTOP);
	HDC hdcThread2 = GetDC(HWND_DESKTOP);
	HDC hdcThread3 = GetDC(HWND_DESKTOP);

	// Find Screen Center
	int startingX = rcClientPositioing.right - ((rcClientPositioing.right - rcClientPositioing.left) / 2);
	int startingY = rcClientPositioing.bottom - ((rcClientPositioing.bottom - rcClientPositioing.top) / 2);



	ScanContents scanContents1(startingX, startingY, hdcThread1, 0, 30, -20, -10, 1);
	ScanContents scanContents2(startingX, startingY, hdcThread2, 20, 5, 0, -20, 2);
	ScanContents scanContents3(startingX, startingY, hdcThread3, 20, 30, 0, 10, 3);
	ScanContents scan(startingX, startingY, hdcMain, 30, 40, -30, -30);

	_beginthread((void(*)(void*))ScanThread, 0, (void*)&scanContents1);
	_beginthread((void(*)(void*))ScanThread, 0, (void*)&scanContents2);
	_beginthread((void(*)(void*))ScanThread, 0, (void*)&scanContents3);

	MainScan(scan);


	system("PAUSE");
}
void ScanThread(ScanContents* scan)
{
	int debugRuntime = clock();
	while (true)
	{
		for (int y = scan->StartY + scan->DeductY; y < scan->StartY + scan->CompareY; y += 3)
		{
			for (int x = scan->StartX + scan->DeductX; x < scan->StartX + scan->CompareX; x += 3)
			{
				//Sleep(100);
				//SetCursorPos(x, y);

				CheckColor(GetPixel(scan->Hdc, x, y), x, y);
				if (GetAsyncKeyState(VK_DELETE))
				{
					exit(0);
				}
			}
		}
		//std::cout << "[THREAD " << scan->ThreadNum << "]Took " << clock() - debugRuntime << " Milliseconds to scan whole area " << std::endl;
		//debugRuntime = clock();
	}
}


void MainScan(ScanContents scan)
{
	int debugRuntime = clock();
	while (true)
	{
		for (int y = scan.StartY + scan.DeductY; y < scan.StartY + scan.CompareY; y += 3)
		{
			for (int x = scan.StartX + scan.DeductX; x < scan.StartX + scan.CompareX; x += 3)
			{
				//Sleep(100);
				//SetCursorPos(x, y);
				CheckColor(GetPixel(scan.Hdc, x, y), x, y);
				if (GetAsyncKeyState(VK_DELETE))
				{
					exit(0);
				}
			}
		}
		//std::cout << "Took " << clock() - debugRuntime << " Milliseconds to scan whole area " << std::endl;
		//debugRuntime = clock();
	}
}

void CheckColor(COLORREF pixel, int x, int y)
{
	if (ColorMatch(pixel))
	{
		int rep = 0;
		while (rep < 5)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
			mouse_event(MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
			Sleep(80);
			rep++;
		}


	}


}

bool ColorMatch(COLORREF pixel)
{
	int r = GetRValue(pixel);
	int g = GetGValue(pixel);
	int b = GetBValue(pixel);

	// RED
	if (chosenColor == "1")
	{
		if (r > 50 && g < 40 && b < 40)
		{
			return true;
		}
	}

	// GREEN
	else if (chosenColor == "2")
	{
		if (r < 40 && g > 50 && b < 40)
		{
			return true;
		}
	}

	// Blue
	else if (chosenColor == "3")
	{
		if (r < 40 && g < 40 && b > 50)
		{
			return true;
		}
	}



	return false;
}