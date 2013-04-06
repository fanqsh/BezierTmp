#pragma once
#include <windows.h>
#include <math.h>

class BezierClass
{
public:
	BezierClass();
	~BezierClass();

	void Frame(HDC hdc, float timePass);

	bool Initialize();

	void Shutdown();

	void KeyDown(int x, int y);
	void KeyUp(int x, int y);
	void MouseMove(int dx, int dy);

private:
	float Get_C_N_I(int n, int i);
	float GetLastCoefficient(int n, int i, float t);
	void GetPoint(float* x, float* y, float* px, float* py, int n, float t);
	void DrawBezierLine(HDC hdc, float* px, float* py, int n);

private:
	float* m_px;
	float* m_py;
	int* m_posX;
	int* m_posY;
	int* m_conX;
	int* m_conY;
	bool isFirst;
	int m_currentIndex;
};

