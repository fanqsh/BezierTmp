#include "system.h"


SystemClass::SystemClass(void)
{
	bexit = false;
	m_spacingTime = 0.0f;
	m_timer = NULL;
	m_bezier = NULL;
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass(void)
{
}



//调用串口初始化函数和其它一些类的初始化函数
//本例子中只调用初始化窗口函数
bool SystemClass::Initialize()
{
	int screenWidth = 0, screenHeight = 0;
	bool result = false;
	//初始化窗口
	InitializeWindows(screenWidth, screenHeight);

	//创建计时器并初始化
	m_timer = new TimerClass;
	if (!m_timer)
		return false;
	result = m_timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"m_timer->Initialize 初始化失败！", L"error!!", MB_OK);
		return false;
	}

	m_bezier = new BezierClass;
	if (!m_bezier)
		return false;
	result = m_bezier->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"m_bezier->Initialize initialize fail!", L"error!!", MB_OK);
	}

	return true;
}

void SystemClass::Shutdown()
{
	//其他类的一些销毁工作
	if (m_timer)
	{
		delete m_timer;
		m_timer = NULL;
	}

	if (m_bezier)
	{
		m_bezier->Shutdown();
		delete m_bezier;
		m_bezier = NULL;
	}

	//执行窗口的销毁
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result = 1;

	//初始化消息结构
	ZeroMemory(&msg, sizeof(MSG));

	//循环进行消息处理
	done = false;

	while (!done)
	{
		//处理windows消息
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//接收到WM_QUIT消息，退出程序.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//result = bexit;	//若按了ESC，也退出程序
			//渲染或其它处理可以放在此处

			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result = false;


	if (bexit)
		return false;

	m_timer->Frame();
	m_bezier->Frame(m_hdc, m_timer->GetTime());
	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//获取应用程序句柄
	ApplicationHandle = this;

	//得到应用程序实例句柄
	m_hinstance = GetModuleHandle(NULL);

	//应用程序名字
	m_applicationName = L"二货联盟";

	//设置窗口类参数
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	//制定回调函数，指定后windows会自动调用回调函数处理各类消息事件
	wc.lpfnWndProc		= WndProc;//制定回调函数
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_applicationName;
	wc.cbSize			= sizeof(WNDCLASSEX);

	//注册窗口类
	RegisterClassEx(&wc);

	//获取windows桌面分辨率
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//窗口模式 ： 800*600
	screenWidth = 700;
	screenHeight = 700;

	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//隐藏鼠标
	//ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	//显示光标
	//ShowCursor(true);

	//释放窗口句柄
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//释放应用程序实例
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		//检测按键消息
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE)
			bexit = true;
		return 0;

	case WM_KEYUP:
			return 0;
	case WM_SIZE:
			return 0;

	case WM_LBUTTONDOWN:
		if( wparam & MK_LBUTTON )
		{
			SetCapture(hwnd);

			m_mousePosition.x = LOWORD(lparam);
			m_mousePosition.y = HIWORD(lparam);
			m_bezier->KeyDown(m_mousePosition.x, m_mousePosition.y);
		}
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		m_bezier->KeyUp(LOWORD(lparam), HIWORD(lparam));

		return 0;

	case WM_MOUSEMOVE:
		if (wparam & MK_LBUTTON)
		{
			mousePosition mousePos;
			mousePos.x = (int)LOWORD(lparam);
			mousePos.y = (int)HIWORD(lparam);

			m_bezier->MouseMove(mousePos.x, mousePos.y);

			m_mousePosition = mousePos;

		}
		return 0;
	case WM_MOUSEWHEEL:
		return 0; 

		//其它消息发送给windows进行缺省处理
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;        // 指向图形环境的句柄 WM_PAINT事件中使用
	switch (umessage)
	{
	case WM_PAINT: //窗口内容需要重画时调用 
		{
			//BeginPaint测试 这个只获得被破坏的范围 不能获得整个窗口
			ApplicationHandle->m_hdc = BeginPaint(hwnd,&ps);     //BeginPaint hdc仅代表了窗口被破坏的区域 并不能操作整个屏幕
			// 在此做你自己的绘制 

			EndPaint(hwnd,&ps);

			// 返回
			return(0);
		} break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}



