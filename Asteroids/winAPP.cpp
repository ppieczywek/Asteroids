#include "stdafx.h"
#include<string>
#include "GameEngineClass.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{	
	srand((unsigned int)time(0));

	GameEngineClass Engine;
	if(!Engine.Initialize(nCmdShow)) return 0;
	Engine.Run();
    return 0;
};






