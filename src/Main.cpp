#include <string>
#include <format>
#include "OGLWindow.h"
#include "Ex01TriangleDraw.h"
#include "Ex02QuadDraw.h"
#include "Ex03QuadIndexDraw.h"
#include "Ex04QuadColorDraw.h"
#include "Ex05QuadTextureDraw.h"
#include "Ex06QuadPerspDraw.h"
#include "Ex07CubePerspDraw.h"
#include "Ex08PhongDraw.h"
#include "Ex09InstancingDraw.h"
#include "Ex10PostFxDraw.h"

int main() 
{
	OGLWindow Window{800, 600, "Hello OpenGL"};

	//Ex01TriangleDraw Scene;
	//Ex02QuadDraw Scene;
	//Ex03QuadIndexDraw Scene;
	//Ex04QuadColorDraw Scene;
	//Ex05QuadTextureDraw Scene;
	//Ex06QuadPerspDraw Scene;
	//Ex07CubePerspDraw Scene;
	//Ex08PhongDraw Scene;
	//Ex09InstancingDraw Scene;
	Ex10PostFxDraw Scene;

	while(Window.IsOpened()) 
	{
		static float TitleUpdateMaxTime = 1.f;
		static float TitleUpdateElapsed = 0.f;

		float DeltaTime = Window.GetDeltaTime();
		TitleUpdateElapsed += DeltaTime;
		if (TitleUpdateElapsed >= TitleUpdateMaxTime) 
		{
			int Fps = 1.f / DeltaTime;
			std::string Title = std::format("OpenGL App | DeltaTime: {} - FPS: {}", DeltaTime, Fps);
			Window.SetTitle(Title);
			TitleUpdateElapsed -= TitleUpdateMaxTime;
		}

		Scene.Update(DeltaTime);
		
		Window.Update();
	}
	return 0;	
}