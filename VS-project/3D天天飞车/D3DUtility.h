#ifndef __D3DUtilityH__
#define __D3DUtilityH__

#include <d3dx9.h>
#include <string>
#include <limits>
#include<vector>

namespace d3d
{
	//
	// ��ʼ��D3D����
	//
	bool InitD3D(
		HINSTANCE hInstance,      
		int width, int height,    
		bool windowed,           
		D3DDEVTYPE deviceType,     
		IDirect3DDevice9** device);
	//
	//��Ϣѭ������
	//
	int EnterMsgLoop(
		bool(*ptr_display)(float timeDelta));

	//
	//���ڻص�����
	//
	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	//
	// �ͷ���Դģ��
	//
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}

	//
	// 8����ɫ
	//
	const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

	//
	// ���ֹ�Դ
	//

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	//
	// 5�ֲ���
	//

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	//
	// �����
	//

	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct BoundingSphere
	{
		BoundingSphere();

		D3DXVECTOR3 _center;
		float       _radius;
	};

	//
	// ���������С����
	//

	const float INFINITY1 = FLT_MAX;
	const float EPSILON = 0.001f;

	//
	// ����ṹ��
	//

	struct Vertex
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			float nx, float ny, float nz,
			float u, float v)
		{
			_x = x;  _y = y;  _z = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u = u;  _v = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v;

		static const DWORD FVF;
	};


	//��Ӻнṹ��
	struct BOUNDINGBOX
	{
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;
	};


	// ��ȡ��[lowBound, highBound]��Χ�ڵ������ 
	float GetRandomFloat(float lowBound, float highBound);


	// ��������min��max��ȡ�������
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

	//
	// ��float�͵���ת��ΪDWORD��
	//
	DWORD FtoDw(float f);
}

#endif // __D3DUtilityH__