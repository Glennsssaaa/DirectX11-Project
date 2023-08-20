#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) {
	timer.Start();
	if (!render_window.Initialize(this, hInstance, window_title, window_class, width, height)) {
		return false;
	}
	if (!gfx.Initialize(render_window.GetHWND(), width, height)) {
		return false;
	}
	return true;
}

bool Engine::ProcessMessages() {
	return render_window.ProcessMessages();
}

void Engine::Update() {
	
	float dt = timer.GetMillisecondsElapsed();
	timer.Restart();

	gfx.carModel.AdjustRotation(0.0f, 0.001f * dt, 0.0f);
	gfx.cowModel.AdjustRotation(0.0f, 0.001f * dt, 0.0f);
	gfx.nanosuitModel.AdjustRotation(0.0f, 0.001f * dt, 0.0f);

	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown()) {
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
				gfx.Camera3D.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float Camera3DSpeed = 0.005f;
	if (keyboard.KeyIsPressed('W')) {
		gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetForwardVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed('S')) {
		gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetBackwardVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed('A')) {
		gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetLeftVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed('D')) {
		gfx.Camera3D.AdjustPosition(gfx.Camera3D.GetRightVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed(VK_SPACE)) {
		gfx.Camera3D.AdjustPosition(0.0f,Camera3DSpeed * dt,0.0f);
	}
	if (keyboard.KeyIsPressed('Z')) {
		gfx.Camera3D.AdjustPosition(0.0f,-Camera3DSpeed * dt,0.0f);
	}
}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}
