#include"SetAndRender.h"
#include"D3DUtility.h"

void ControlCamera(Camera& theCamera,float timeDelta,float speed)
{
	theCamera.walk(speed * timeDelta);

	static bool change = false;
	static int i = 0;
	static float time = 0.0f;
	if (::GetAsyncKeyState('C') & 0x8000f)
	{
		time += timeDelta;
		if (time > 0.3f)
		{
			change = true;
			i++;
			time = 0.0f;
		}
	}
	if (change)
	{
		if (i % 2 == 1)
		{
			theCamera.strafe(10.0f);
			theCamera.walk(-10.0f);
		}
		else
		{
			theCamera.strafe(-10.0f);
			theCamera.walk(10.0f);
		}
		change = false;
	}

}

D3DXMATRIX ControlCar(IDirect3DDevice9* device, float timeDelta, float speed,bool isRotate,float* _angle)
{
	//��С����ģ��
	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling, 0.15f, 0.15f, 0.15f);


	//ģ��������
	D3DXMATRIX Ry;				   //���ʱ����ģ����Y��ƫת�ľ���
	static float inx = 0.0f;       //���ʱ������X���ϵ�ƫ����
	static float angle = 0.0f;     //��Y����ת�ĽǶ�
	static int state = 0;          //�����״̬��1Ϊ����2Ϊ���ҡ�0Ϊû�б��
	static int count = 0;          //��¼���±�����Ĵ���
	static int ischange = 0;       //���һ�α���Ƿ��Ѿ�����

	static float lastx = 0.0f;     //������ǰ������X���λ��
	static float lasttime = 0.0f; 

	if ((::GetAsyncKeyState('A') & 0x8000f)&&ischange == 0 )
	{
		angle -= timeDelta*0.3f;
		state = 1;    //����
		inx = angle;
		count++;
	}
	else if ((::GetAsyncKeyState('D') & 0x8000f) && ischange == 0)
	{
		angle += timeDelta*0.3f;
		state = 2;    //����
		inx = angle;
		count++;
	}
	else
	{
		state = 0;
		count = 0;
		ischange = 1;
	}

	//
	//�������
	//
	if (angle >= D3DX_PI*0.25)
		angle = D3DX_PI*0.25;
	if (angle <= -D3DX_PI*0.25)
		angle = -D3DX_PI*0.25;

	if (state == 0)
	{
		if (angle > 0.0f)
			angle -= timeDelta*0.3;
		else if (angle < 0.0f)
			angle += timeDelta*0.3;
	}
	if (angle<0.001f&&angle>-0.001f)
	{
		ischange = 0;
		angle = 0.0f;
	}
	D3DXMatrixRotationY(&Ry, angle);    //��y����ת��ģ�⳵��ƫ�Ʊ��


	if (count == 1)
		lastx = lasttime;

	float finalx = lastx + sinf(inx * 3)*20.0f;
	if (finalx>15.0f)
		finalx = 15.0f;
	if (finalx < -15.0f)
		finalx = -15.0f;

	//
	//������ת�������ܣ����ո��ʵ����ת����
	//
	static float r_angle = 0.0f;
	D3DXMATRIX Rotate;    //��ת�Ƕ�
	if ((::GetAsyncKeyState(' ') & 0x8000f)&&isRotate)
		r_angle += timeDelta * 10;
	else
		r_angle = 0.0f;
	if (r_angle > D3DX_PI)
		r_angle = 0.0f;
	D3DXMatrixRotationY(&Rotate, r_angle);

	//
	//��������
	//
	static float movey = 0.0f;        //��¼����������y���ϵ�λ�ñ仯
	static int flag = 0;              //��־����/�½���1Ϊ������0Ϊ�½�
	if (::GetAsyncKeyState('J') & 0x8000f && movey < 0.0001f)
		flag = 1;

	if (flag == 1 && movey<15.0f)     //������������С��15.0f��ʱ������
	{
		movey += timeDelta * 30;
	}
	else if (flag == 0 && movey>0.0f)  
	{
		movey -= timeDelta * 30;
	}

	if (movey >= 15.0f)   //�����������ڴ���15.0f��ʱ���Ϊ�½�
	{
		flag = 0;
	}
	if (movey < 0.0f)
		movey = 0.0f;

	D3DXMATRIX jump;
	D3DXMatrixTranslation(&jump, 0.0f, movey, 0.0f);

	//
	//��ǰ����
	//
	static float movez = 0.0f;      //��¼����������z���ϵ�λ�ñ仯
	movez += timeDelta * speed;
	D3DXMATRIX ahead;
	D3DXMatrixTranslation(&ahead, finalx, 0.0f, movez);

	D3DXMATRIX world = Rotate* Ry * scaling* ahead * jump;
	lasttime = world._41;

	return world;
}

D3DXMATRIX ControlOtherCar(IDirect3DDevice9* device, D3DXVECTOR3 pos)
{
	D3DXMATRIX result, scaling, moving, rotate;
	D3DXMatrixScaling(&scaling, 0.15f, 0.15f, 0.15f);
	D3DXMatrixRotationY(&rotate, D3DX_PI);
	D3DXMatrixTranslation(&moving, pos.x, pos.y, pos.z );

	result = scaling*rotate*moving;
	return result;
}