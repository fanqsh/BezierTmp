#include "BezierClass.h"

BezierClass::BezierClass()
{
	m_px = NULL;
	m_py = NULL;
	isFirst = true;
	m_posX = NULL;
	m_posY = NULL;
	m_conX = NULL;
	m_conY = NULL;
	m_currentIndex = -1;
}

BezierClass::~BezierClass()
{
}

void BezierClass::Frame(HDC hdc, float timePass)
{
	DrawBezierLine(hdc, m_px, m_py, 5);

	if (isFirst)
	{
		isFirst = false;
	}
}

float BezierClass::Get_C_N_I(int n, int i)
{
    int tmp1 = 1;
    int tmp2 = 1;
	int tmp_n = n;
	int tmp_i = i;
	int len = n - i;

    while (tmp_n > len)
        tmp1 *= tmp_n--;

    while (tmp_i > 0)
        tmp2 *= tmp_i--;
    
    return tmp1/tmp2;
}

float BezierClass::GetLastCoefficient(int n, int i, float t)
{
   float tmp1 = 1;
   float tmp2 = 1;
   int tmp_n = n;
   int tmp_i = i;
   while (tmp_n-- > i)
	   tmp1 = (1 - t) * tmp1;

   while (tmp_i-- > 0)
		tmp1 = tmp1 * t;

   return tmp1;
}

void BezierClass::GetPoint(float* x, float* y, float* px, float* py, int n, float t)
{
    for (int i = n; i >= 0; --i)
    {
        *x += Get_C_N_I(n, i) * GetLastCoefficient(n, i, t) * px[i];
        *y += Get_C_N_I(n, i) * GetLastCoefficient(n, i, t) * py[i];
    }
}

void BezierClass::DrawBezierLine(HDC hdc, float* px, float* py, int n)
{
	COLORREF color = RGB(200,0,0);
	COLORREF colorBc = RGB(0,0,0);
	HBRUSH hbrush = CreateSolidBrush(colorBc);
	//MoveToEx(hdc, 100, 100, NULL) ;
	int i = 0;
	RECT rec;

	for (int j = 0; j <= n; ++j)
	{
		if (m_conX == NULL)
		{
			m_conX = new int[n + 1];
		}
		if (m_conY == NULL)
		{
			m_conY = new int[n + 1];
		}

		rec.top = m_conY[j] - 2;
		rec.bottom = m_conY[j] + 2;
		rec.left = m_conX[j] - 2;
		rec.right = m_conX[j] + 2;

		FillRect(hdc, &rec, hbrush);
		Ellipse(hdc, m_px[j] - 2, m_py[j] - 2, m_px[j] + 2, m_py[j] + 2);
		m_conX[j] = m_px[j];
		m_conY[j] = m_py[j];

	}
	for (float t = 0.0; t < 1; t = 0.005 * (i++))
	{
		float Bx = 0.0;
		float By = 0.0;

		if (m_posX == NULL)
		{
			int num = 1 / 0.005;
			m_posX = new int[num + 1];
		}

		if (m_posY == NULL)
		{
			int num = 1 / 0.005;
			m_posY = new int[num + 1];
		}

		GetPoint(&Bx, &By, px, py, n, t);

		if (!isFirst)
			SetPixel (hdc, m_posX[i], m_posY[i], colorBc);

		SetPixel (hdc, (int)Bx, (int)By, color);
		m_posX[i] = Bx;
		m_posY[i] = By;
	}
}

bool BezierClass::Initialize()
{

	if (m_px == NULL)
	{
		m_px = new float[6];
		m_px[0] = 100;
		m_px[1] = 200;
		m_px[2] = 300;
		m_px[3] = 400;
		m_px[4] = 500;
		m_px[5] = 1000;
	}
	if (m_py == NULL)
	{
		m_py = new float[6];
		m_py[0] = 100;
		m_py[1] = 200;
		m_py[2] = 300;
		m_py[3] = 400;
		m_py[4] = 500;
		m_py[5] = 100;
	}
	return true;
}

void BezierClass::Shutdown()
{
	if (m_px)
	{
		delete[] m_px;
		m_px = NULL;
	}

	if (m_py)
	{
		delete[] m_py;
		m_py = NULL;
	}

	if (m_posX)
	{
		delete[] m_posX;
		m_posX = NULL;
	}

	if (m_posY)
	{
		delete[] m_posY;
		m_posY = NULL;
	}
}

void BezierClass::KeyDown(int x, int y)
{
	if (m_currentIndex > 0 && m_currentIndex < 7)
		return;

	for (int i = 0; i < 7; ++i)
	{
		if (x < m_px[i] + 5 && x > m_px[i] - 5 && y < m_py[i] + 5 && y > m_py[i] - 5)
		{
			m_currentIndex = i;
			return;
		}
	}
}

void BezierClass::KeyUp(int x, int y)
{
	m_currentIndex = -1;
}

void BezierClass::MouseMove(int dx, int dy)
{
	if (m_currentIndex < 0 || m_currentIndex >= 7)
		return;

	m_px[m_currentIndex] = dx;
	m_py[m_currentIndex] = dy;
}
