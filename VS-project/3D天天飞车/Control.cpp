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
	//缩小汽车模型
	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling, 0.15f, 0.15f, 0.15f);


	//模拟变道技能
	D3DXMATRIX Ry;				   //变道时车辆模型绕Y轴偏转的矩阵
	static float inx = 0.0f;       //变道时车辆在X轴上的偏移量
	static float angle = 0.0f;     //绕Y轴旋转的角度
	static int state = 0;          //变道的状态，1为向左、2为向右、0为没有变道
	static int count = 0;          //记录按下变道键的次数
	static int ischange = 0;       //标记一次变道是否已经结束

	static float lastx = 0.0f;     //保存变道前车辆在X轴的位置
	static float lasttime = 0.0f; 

	if ((::GetAsyncKeyState('A') & 0x8000f)&&ischange == 0 )
	{
		angle -= timeDelta*0.3f;
		state = 1;    //向左
		inx = angle;
		count++;
	}
	else if ((::GetAsyncKeyState('D') & 0x8000f) && ischange == 0)
	{
		angle += timeDelta*0.3f;
		state = 2;    //向右
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
	//变道修正
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
	D3DXMatrixRotationY(&Ry, angle);    //绕y轴旋转，模拟车辆偏移变道


	if (count == 1)
		lastx = lasttime;

	float finalx = lastx + sinf(inx * 3)*20.0f;
	if (finalx>15.0f)
		finalx = 15.0f;
	if (finalx < -15.0f)
		finalx = -15.0f;

	//
	//车辆旋转攻击技能，按空格键实现旋转攻击
	//
	static float r_angle = 0.0f;
	D3DXMATRIX Rotate;    //旋转角度
	if ((::GetAsyncKeyState(' ') & 0x8000f)&&isRotate)
		r_angle += timeDelta * 10;
	else
		r_angle = 0.0f;
	if (r_angle > D3DX_PI)
		r_angle = 0.0f;
	D3DXMatrixRotationY(&Rotate, r_angle);

	//
	//弹跳技能
	//
	static float movey = 0.0f;        //记录车子中心在y轴上的位置变化
	static int flag = 0;              //标志上升/下降，1为上升，0为下降
	if (::GetAsyncKeyState('J') & 0x8000f && movey < 0.0001f)
		flag = 1;

	if (flag == 1 && movey<15.0f)     //触发弹跳，在小于15.0f的时候上升
	{
		movey += timeDelta * 30;
	}
	else if (flag == 0 && movey>0.0f)  
	{
		movey -= timeDelta * 30;
	}

	if (movey >= 15.0f)   //触发弹跳，在大于15.0f的时候变为下降
	{
		flag = 0;
	}
	if (movey < 0.0f)
		movey = 0.0f;

	D3DXMATRIX jump;
	D3DXMatrixTranslation(&jump, 0.0f, movey, 0.0f);

	//
	//向前行走
	//
	static float movez = 0.0f;      //记录车子中心在z轴上的位置变化
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