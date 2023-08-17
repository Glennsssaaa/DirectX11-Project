#include "RenderWindow.h"

bool RenderWindow::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height){
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	this->RegisterWindowClass();

	this->handle = CreateWindowEx(0,
		this->window_class_wide.c_str(), //Class Name
		this->window_title_wide.c_str(), //Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Windows Style
		0, //Window XPos
		0, //Window YPos
		this->width, //Window Width
		this->height, //Window Height
		NULL, //Window Parent Handle
		NULL, //Window Child Handle
		this->hInstance, //Instance Handle
		nullptr); //Create Window Param
	
	if (this->handle == NULL) {
	ErrorLogger::Log(GetLastError(), "CreateWindowEX Failed for window: " + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return false;
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

	if (PeekMessage(&msg, //Message Storage Location
		this->handle, //Window Handle
		0, ///Msg Value Max
		0, //Msg Value Max
		PM_REMOVE)) { //Remove Message after capture
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

void RenderWindow::RegisterWindowClass(){
	WNDCLASSEX wc; //Window Class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags
	wc.lpfnWndProc = DefWindowProc; //Window Proc Pointer
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