#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height){
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	this->RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;


	//Window Rectangle
	RECT wr;
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + this->width;
	wr.bottom = wr.top + this->height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	this->handle = CreateWindowEx(0,
		this->window_class_wide.c_str(), //Class Name
		this->window_title_wide.c_str(), //Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Windows Style
		wr.left, //Window XPos
		wr.top, //Window YPos
		wr.right - wr.left, //Window Width
		wr.bottom - wr.top, //Window Height
		NULL, //Window Parent Handle
		NULL, //Window Child Handle
		this->hInstance, //Instance Handle
		pWindowContainer); //Create Window Param
	
	if (this->handle == NULL) {
	ErrorLogger::Log(GetLastError(), "CreateWindowEX Failed for window: " + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

RenderWindow::~RenderWindow() {
	if (this->handle != NULL) {
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

bool RenderWindow::ProcessMessages() {
	//Message Initialization
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, //Message Storage Location
		this->handle, //Window Handle
		0, ///Msg Value Max
		0, //Msg Value Max
		PM_REMOVE)) //Remove Message after capture
	{ 
		TranslateMessage(&msg); //Translate from virtual key into char
		DispatchMessage(&msg); //Dispatch to window proc
	}

	if (msg.message == WM_NULL) {
		if (!IsWindow(this->handle)) {
			this->handle = NULL;
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_NCCREATE: {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr) {
			ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

void RenderWindow::RegisterWindowClass(){
	WNDCLASSEX wc; //Window Class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags
	wc.lpfnWndProc = HandleMessageSetup; //Window Proc Pointer
	wc.cbClsExtra = 0; //Byte Allocation for Structure
	wc.cbWndExtra = 0; //Byte Allocation for Instance
	wc.hInstance = this->hInstance; //Instance handle
	wc.hIcon = NULL; //Icon handle
	wc.hIconSm = NULL; //Small icon handle
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Cursor Initialisation
	wc.hbrBackground = NULL; //Window background brush handle
	wc.lpszMenuName = NULL; //Menu character string
	wc.lpszClassName = this->window_class_wide.c_str(); //Class name string
	wc.cbSize = sizeof(WNDCLASSEX); //Struct size
	RegisterClassEx(&wc); //Class register
}