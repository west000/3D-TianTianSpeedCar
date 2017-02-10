#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<math.h>
#include"D3DUtility.h"
#include"Camera.h"
#include"D3DGUI.h"
#include"pSystem.h"
#include"SkyBox.h"
#include"SetAndRender.h"
#include"LoadAndSetMesh.h"

using namespace std;

//全局设备
IDirect3DDevice9* Device = 0;

const int Width = 1366;
const int Height = 768;

//摄像机
Camera TheCamera(Camera::LANDOBJECT);
//天空盒
SkyBoxClass* pSkyBox = 0;

//------------------------------------------------------------------
//GUI界面
//------------------------------------------------------------------
bool LMBDown = false;
int MouseX = 0, MouseY = 0;

//五个GUI类的对象，代表五个页面
D3DGUIClass* MainGUI = NULL;   //主菜单页面
D3DGUIClass* AboutGUI = NULL;  //关于页面
D3DGUIClass* HelpGUI = NULL;   //帮助页面
D3DGUIClass* ContinueGUI = NULL;//继续页面
D3DGUIClass* LoadGUI = NULL;    //加载页面

int LoadGUIFontID = -1;
int HelpGUIFontID = -1;
int AboutGUIFontID = -1;
int CurrentGUI = GUI_MAIN_SCREEN;

//------------------------------------------------------------------
//技能盒子
//------------------------------------------------------------------
float RoadCenter[5] = { 0.0f, -14.0f, -5.2f, 5.2f, 14.0f };  //路的中心位置

//------------------------------------------------------------------
//子弹、导弹、车辆模型的外接体
//------------------------------------------------------------------
d3d::BOUNDINGBOX BoxCar;      //车辆外接体
d3d::BOUNDINGBOX BoxBullet;   //子弹外接体
d3d::BOUNDINGBOX BoxMissile;  //导弹外接体

//------------------------------------------------------------------
//界面上的文本信息
//------------------------------------------------------------------
LPD3DXFONT InfoText = 0;
LPD3DXFONT CountTimeText = 0;


bool Setup()
{
	//设置GUI界面
	SetGUI(Device);

	//设置文本信息
	SetInfoText(Device, &InfoText, &CountTimeText);

	//设置摄像机
	D3DXVECTOR3 position(0.0f, 15.0f, -50.0f);
	TheCamera.setPosition(&position);

	//设置天空盒
	pSkyBox = new SkyBoxClass(Device);
	pSkyBox->LoadShyTextureFromFile("GameMedia\\frontsnow1.jpg", "GameMedia\\backsnow1.jpg",
		"GameMedia\\leftsnow1.jpg", "GameMedia\\rightsnow1.jpg", "GameMedia\\topsnow1.jpg");
	pSkyBox->InitSkyBox(66000);

	//设置基本场景
	SetBasicSence(Device);

	//设置立方体
	SetCube(Device);

	//设置方块
	SetSquare(Device);

	//设置烟花
	SetFirework(Device);

	//加载所有网格模型
	LoadAllModel(Device);

	//设置子弹、车子模型的外接体
	SetBoundingBox(&BoxCar,&BoxBullet,&BoxMissile);

	//加载方块和立方体的纹理
	LoadTexture(Device);

	//播下随机种子
	srand((unsigned int)time(0));

	//进行投影变换
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)Width / (float)Height,
		1.0f,
		100000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}


void Cleanup()
{
	//清除纹理缓存
	void CleanupTexture();

	//清除网格的内存
	void CleanupMesh();
}

//---------------------------------------------------------------------
//展示后台缓存函数
//---------------------------------------------------------------------
bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);
		Device->BeginScene();

		//需要统计的数据：摧毁车辆数、金币数、分数、血量HP = 1000、
		//当前子弹数、当前导弹数、行走路程。
		static int HP = 200;     //血量
		static int goldnum = 0;   //金币数
		static int bulletnum = 5; //子弹数
		static int missilenum = 3;//导弹数
		static int distance = 0;  //路程
		static int carnum = 0;    //摧毁车辆数
		static float speed = 0.0f;//当前车辆的速度
		static float glassytime = 0.0f;   //透明时间
		static float speedtime = 0.0f;    //加速时间
		static float missiletime = 0.0f;  //导弹装载的时间
		static float time = 0.0f;         //游戏花费的时间
		static bool isCollision = true; //吃到透明盒子就关了碰撞测试
		static bool isRotate = false;   //吃到龙卷风盒子，旋转攻击状态就开启，使用5秒后就关闭		
		//获取车辆的世界矩阵
		float angle;
		D3DXMATRIX matCar = ControlCar(Device, timeDelta, speed, isRotate, &angle);

		if (CurrentGUI == GUI_START_SCREEN )
		{	
			//统计游戏时间
			time += timeDelta;

			//控制摄像机
			ControlCamera(TheCamera, timeDelta, speed);
			D3DXMATRIX View;
			TheCamera.getViewMatrix(&View);
			Device->SetTransform(D3DTS_VIEW, &View);

			//渲染基本场景
			RenderBasicSence(Device, View);

			//天空盒子
			D3DXMATRIX matSky, matTransSky, matRotSky;
			D3DXMatrixTranslation(&matTransSky, 0.0f, 0.0f, 0.0f);
			D3DXMatrixRotationY(&matRotSky, -0.001*timeDelta);
			matSky = matTransSky*matRotSky;
			pSkyBox->RenderSkyBox(&matSky);

			//渲染车辆
			RenderCar(Device, timeDelta, matCar,!isCollision);
			//车辆上的火箭筒
			RenderGun(Device, matCar);

			//获取车辆的位置
			D3DXVECTOR3 carpos;
			carpos.x = matCar._41;  //x分量
			carpos.y = matCar._42;  //y分量
			carpos.z = matCar._43;  //z分量
			D3DXVECTOR3 CarMin = D3DXVECTOR3(carpos.x, carpos.y, carpos.z) + BoxCar.vMin;  //车辆外接体的最小向量
			D3DXVECTOR3 CarMax = D3DXVECTOR3(carpos.x, carpos.y, carpos.z) + BoxCar.vMax;  //车辆外接体的最大向量

			//文本信息
			RECT rect = { 600, 300, 700, 400 };
			static char strInfo[256] = { 0 };

			//随机盒子的位置容器
			static std::vector<D3DXVECTOR3> vecCubePos(5);
			//随机车辆的位置容器
			static std::vector<D3DXVECTOR3> vecCarPos(5);


			//-------------------------------------------------------------------
			//3秒钟的准备时间
			//-------------------------------------------------------------------
			if (time < 3.0f)
			{
				if (time<1.0f)
					sprintf_s(strInfo, "%d", 3);
				else if (time<2.0f)
					sprintf_s(strInfo, "%d", 2);
				else 
					sprintf_s(strInfo, "%d", 1);
				CountTimeText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP , D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
				
				//随机产生技能盒子的位置
				for (vector<D3DXVECTOR3>::iterator iter = vecCubePos.begin();
					iter != vecCubePos.end(); ++iter)
				{
					iter->x = RoadCenter[rand() % 4 + 1];
					iter->y = 3.0f;
					iter->z = (rand() % 25) * 125;
				}

				//随机产生路上的车辆
				for (vector<D3DXVECTOR3>::iterator iter = vecCarPos.begin();
					iter != vecCarPos.end(); ++iter)
				{
					iter->x = RoadCenter[rand() % 4 + 1];
					iter->y = 0.0f;
					iter->z = (rand() % 25) * 125 + 50.0f;
				}
			}
			else
			{
				//游戏开始的前3秒，速度增加
				if (time < 6.0f)
					speed += 0.3f;
				if (speed < 0.0f)
					speed = 0.0f;
				if (speed > 500.0f)
					speed = 500.0f;

				//透明时间结束
				if (glassytime > 0.0f)
					glassytime -= timeDelta;
				else
				{
					glassytime = 0.0f;
					isCollision = true;
				}

				//加速时间结束
				if (speedtime > 0.01f)
					speed += 0.06f;
				else
					speedtime = 0.0f;

				//偏转角度在[-D3DX_PI*0.2，D3DX_PI*0.2]之外，会减速
				if (angle > D3DX_PI*0.2 || angle < -D3DX_PI*0.2)
					speed -= timeDelta;

				//-----------------------------------------------------------------
				//在路上渲染随机的道具盒子
				//-----------------------------------------------------------------
				int num = 0;
				for (std::vector<D3DXVECTOR3>::iterator iter = vecCubePos.begin();
					iter != vecCubePos.end(); ++iter)
				{
					if (iter->z > carpos.z - 30.0f)    //在视野范围内就渲染技能盒子
					{
						D3DXMATRIX matCube;
						D3DXMatrixTranslation(&matCube, iter->x, iter->y, iter->z);
						RenderCube(Device, matCube, timeDelta,num);

						D3DXVECTOR3 tempmin(iter->x - 2.0f, iter->y - 2.0f, iter->z - 2.0f);
						D3DXVECTOR3 tempmax(iter->x + 2.0f, iter->y + 2.0f, iter->z + 2.0f);

						if (CollisionTest(CarMin, CarMax,tempmin, tempmax))
						{					
							switch (num)
							{
							case 1:   //吃到穿透盒子
								isCollision = false;
								glassytime = 5.0f;
								break;
							case 2:   //吃到武器盒子
								missilenum += 1;
								bulletnum += 5;
								break;
							case 3:   //吃到加速盒子
								speedtime = 2.0f;
								break;
							case 4:   //吃到龙卷风盒子
								isRotate = true;
								break;
							case 0:   //吃到金币盒子
								goldnum += 10;
								HP += 50;
								break;
							}
							iter->z -= 50.0f;  //吃到箱子，箱子就消失
						}
					}
					else
					{
						iter->x = RoadCenter[rand() % 4 + 1];
						iter->z = carpos.z + (rand() % 25) * 125 + rand() % 100;
					}
					num++;
				}

				//-----------------------------------------------------------------
				//在路上渲染随机车辆
				//-----------------------------------------------------------------
				RenderOtherCar(Device, timeDelta, vecCarPos, CarMin, CarMax, carpos, BoxCar, matCar, HP, speed, carnum, isCollision);

				//-----------------------------------------------------------------
				//导弹打击
				//-----------------------------------------------------------------
				static D3DXVECTOR3 miss_pos;      //导弹当前的位置
				static bool isfire = false;       //导弹是否发射
				static float miss_fly = 0.0f;     //导弹的飞行增量
				static float miss_time = 0.0f;    //导弹飞行和爆炸的总时间
				if ((::GetAsyncKeyState('T')&0x8000f) && missilenum > 0 && !isfire)
				{
					missilenum--;
					isfire = true;
					miss_time = 3.0f;
				}

				if (isfire && miss_time>0.0f)
				{
					miss_time -= timeDelta;

					D3DXMATRIX miss_move; 
					miss_fly += 1.0f;
					float z = miss_fly;
					float y = 10.0 - miss_fly * 0.1f ;
			
					D3DXMatrixTranslation(&miss_move, 0.0f, y, miss_fly*6.0f + carpos.z);
					if (y > 0.0f)
						RenderBullet(Device, miss_move, 0.4f, &miss_pos);
					else
					{
						D3DXMATRIX boom_pos;
						D3DXMatrixTranslation(&boom_pos, 0.0f, 0.0f,miss_pos.z);
						RenderBoom(Device, timeDelta, boom_pos, 100.0f);

						D3DXVECTOR3 miss_min(-50.0f, -50.0f, miss_pos.z - 100.0f);
						D3DXVECTOR3 miss_max(50.0f, 50.0f, miss_pos.z + 100.0f);

						for (std::vector<D3DXVECTOR3>::iterator iter = vecCarPos.begin();
							iter != vecCarPos.end(); ++iter)
						{
							D3DXVECTOR3 tempmin = *iter + BoxCar.vMin;
							D3DXVECTOR3 tempmax = *iter + BoxCar.vMax;
							if (CollisionTest(miss_min, miss_max, tempmin, tempmax))
							{
								carnum++;   //车辆摧毁+1
								iter->z = carpos.z - 100.0f; //被摧毁的车辆消失
							}
						}
					}
				}
				if (miss_time < 0.0f)
				{
					isfire = false;
					miss_time = 0.0f;
					miss_fly = 0.0f;
				}
			
			}		

			//-----------------------------------------------------------------
			//子弹打击
			//-----------------------------------------------------------------
			static bool isshot = false;
			static float bullet_fly = 0.0f;
			static float bullet_time = 0.0f;
			static D3DXVECTOR3 bullet_pos;
			if ((::GetAsyncKeyState('G') & 0x8000f) && !isshot && bulletnum>0)
			{
				bullet_pos.x = carpos.x;
				bullet_pos.y = carpos.y + 2.5f;
				bulletnum--;
				isshot = true;
			}

			if (isshot && bullet_fly < 1000.0f)
			{
				bullet_fly += 1.0f;
				D3DXMATRIX bullet_move;
				D3DXMatrixTranslation(&bullet_move,bullet_pos.x,bullet_pos.y,carpos.z + bullet_fly);
				if (bullet_time<0.001f)
					RenderBullet(Device, bullet_move, 0.05f, &bullet_pos);
				
				D3DXVECTOR3 bullet_min = bullet_pos + BoxBullet.vMin;
				D3DXVECTOR3 bullet_max = bullet_pos + BoxBullet.vMax;
				
				for (std::vector<D3DXVECTOR3>::iterator iter = vecCarPos.begin();
					iter != vecCarPos.end(); ++iter)
				{
					D3DXVECTOR3 tempmin = *iter + BoxCar.vMin;
					D3DXVECTOR3 tempmax = *iter + BoxCar.vMax;
					if (CollisionTest(bullet_min, bullet_max, tempmin, tempmax))
					{
						carnum++;   //车辆摧毁+1
						bullet_time = 1.0f;
						iter->z = carpos.z - 100.0f; //被摧毁的车辆消失
					}
				}
				if (bullet_time > 0.001f)
				{
					bullet_time -= timeDelta;
					D3DXMATRIX boom_pos;
					D3DXMatrixTranslation(&boom_pos, bullet_pos.x, bullet_pos.y, bullet_pos.z);
					RenderBoom(Device, timeDelta, boom_pos, 10.0f);
				}
			}

			if (bullet_fly>500.0f)
			{
				isshot = false;
				bullet_time = 0.0f;
				bullet_fly = 0.0f;
			}

			//-----------------------------------------------------------------
			//显示游戏信息
			//-----------------------------------------------------------------
			RenderToolSquare(Device, TheCamera);
			ShowInfoText(goldnum, speed, time, carpos.z, HP, carnum, missilenum, bulletnum, InfoText);

			//-----------------------------------------------------------------
			//游戏结束的条件
			//-----------------------------------------------------------------
			if (HP <= 0)
			{
				speed -= timeDelta * 50.0f;  //HP为0时，速度减慢
				D3DXMATRIX boomPos;
				D3DXMatrixTranslation(&boomPos, carpos.x, carpos.y, carpos.z - 10.0f);
				RenderBoom(Device, timeDelta, boomPos, 10.0f);

				rect.top = 300;
				rect.left = 120;
				sprintf_s(strInfo, "出车祸了...");
				CountTimeText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::RED);
			}
			if (carpos.z > 30000.0f && speed>0.1f)  //抵达终点时速度减慢
				speed -= timeDelta * 100.0f;
			if (carpos.z > 30000.0f)
			{
				rect.top = 300;
				rect.left = 180;
				sprintf_s(strInfo, "抵达终点");
				CountTimeText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::WHITE);

				D3DXMATRIX firework;
				D3DXMatrixTranslation(&firework, 0.0f, 0.0f, carpos.z + 100.0f);
				RenderFirework(Device, firework, timeDelta);
			}

			//-----------------------------------------------------------
			//按下home键暂停游戏，回到继续游戏页面
			//-----------------------------------------------------------
			if (::GetAsyncKeyState(VK_HOME) & 0x8000f)
				CurrentGUI = GUI_CONTINUE_SCREEN;
		}
		else if (CurrentGUI == GUI_MAIN_SCREEN)
		{
			ProcessGUI(MainGUI, LMBDown, MouseX, MouseY, GUI_MAIN_SCREEN, GUICallback);

			//回到主界面所有数据需要重新初始化
			HP = 200;      //血量
			goldnum = 0;   //金币数
			bulletnum = 5; //子弹数
			missilenum = 3; //导弹数
			distance = 0;   //路程
			carnum = 0;     //摧毁车辆数
			speed = 0.0f;   //当前车辆的速度
			glassytime = 0.0f;   //透明时间
			speedtime = 0.0f;    //加速时间
			missiletime = 0.0f;  //导弹装载的时间
			time = 0.0f;         //游戏花费的时间
			isCollision = true;  //吃到透明盒子就关了碰撞测试
			isRotate = false;    //迟到龙卷风盒子，旋转攻击状态就开启，使用5秒后就关闭
		}
		else if (CurrentGUI == GUI_CONTINUE_SCREEN)
		{
			ProcessGUI(ContinueGUI, LMBDown, MouseX, MouseY, GUI_CONTINUE_SCREEN, GUICallback);
		}
		else if (CurrentGUI == GUI_HELP_SCREEN)
		{
			ProcessGUI(HelpGUI, LMBDown, MouseX, MouseY, GUI_HELP_SCREEN, GUICallback);
		}
		else if (CurrentGUI == GUI_ABOUT_SCREEN)
		{
			ProcessGUI(AboutGUI, LMBDown, MouseX, MouseY, GUI_ABOUT_SCREEN, GUICallback);
		}
		else if (CurrentGUI == GUI_LOAD_SCREEN)
		{
			ProcessGUI(LoadGUI, LMBDown, MouseX, MouseY,GUI_LOAD_SCREEN, GUICallback);
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//---------------------------------------------------------------------
//设置GUI界面
//---------------------------------------------------------------------
bool SetGUI(IDirect3DDevice9* device)
{
	//创建GUI系统
	MainGUI = new D3DGUIClass(Device, Width, Height);    //主菜单页面
	AboutGUI = new D3DGUIClass(Device, Width, Height);   //关于页面
	HelpGUI = new D3DGUIClass(Device, Width, Height);    //帮助信息页面
	ContinueGUI = new D3DGUIClass(Device, Width, Height);//继续页面
	LoadGUI = new D3DGUIClass(Device, Width, Height);    //加载页面
	
	//为五个页面加载纹理图
	if (!MainGUI->AddBackground("GameMedia\\maingui.jpg")) return false;
	if (!AboutGUI->AddBackground("GameMedia\\GUI1.jpg")) return false;
	if (!HelpGUI->AddBackground("GameMedia\\GUI2.jpg")) return false;
	if (!ContinueGUI->AddBackground("GameMedia\\GUI3.jpg")) return false;
	if (!LoadGUI->AddBackground("GameMedia\\maingui.jpg")) return false;
	
	//为关于界面、帮助界面添加文字
	if (!AboutGUI->CreateTextFont("微软雅黑", 35, &AboutGUIFontID)) return false;
	if (!HelpGUI->CreateTextFont("微软雅黑", 35, &HelpGUIFontID)) return false;
	if (!LoadGUI->CreateTextFont("微软雅黑", 40, &LoadGUIFontID)) return false;

	//主菜单布局
	if (!MainGUI->AddButton(BUTTON_START_ID, 650, 340, "GameMedia\\开始游戏1.png",
		"GameMedia\\开始游戏2.png", "GameMedia\\开始游戏3.png")) return false;

	if (!MainGUI->AddButton(BUTTION_HELP_ID, 650, 385, "GameMedia\\帮助1.png",
		"GameMedia\\帮助2.png", "GameMedia\\帮助3.png")) return false;

	if (!MainGUI->AddButton(BUTTON_ABOUT_ID, 650, 430, "GameMedia\\关于1.png",
		"GameMedia\\关于2.png", "GameMedia\\关于3.png")) return false;

	if (!MainGUI->AddButton(BUTTON_QUIT_ID, 650, 475, "GameMedia\\退出1.png",
		"GameMedia\\退出2.png", "GameMedia\\退出3.png")) return false;


	//继续页面布局
	if (!ContinueGUI->AddButton(BUTTON_START_ID, 650, 480, "GameMedia\\继续游戏1.png",
		"GameMedia\\继续游戏2.png", "GameMedia\\继续游戏3.png")) return false;
	if (!ContinueGUI->AddButton(BUTTON_BACK_ID, 650, 525, "GameMedia\\返回菜单1.png",
		"GameMedia\\返回菜单2.png", "GameMedia\\返回菜单3.png")) return false;


	//帮助页面布局
	if (!HelpGUI->AddButton(BUTTON_BACK_ID, 1120, 500, "GameMedia\\返回菜单1.png",
		"GameMedia\\返回菜单2.png", "GameMedia\\返回菜单3.png")) return false;
	if (!HelpGUI->AddStaticText(STATIC_TEXT_ID, 
		"具体操作如下：\n \
		1、按下A、D键进行左右变道\n \
		2、按下C键切换视角\n \
		3、按下J键弹跳\n \
		4、按下空格键展开龙卷风攻击\n \
		5、按下T键发射导弹\n \
		6、按下G键发射子弹\n\n \
		道具功能： \n \
		 金币盒子：+10金币\n  透明盒子：穿透车辆\n  加速盒子：加速2秒钟\n  武器盒子：+3导弹、+10子弹\n  龙卷风盒子：龙卷风旋转技能\n ",
		200, 150, D3DCOLOR_XRGB(0,255, 255), HelpGUIFontID)) return false;


	//关于界面布局
	if (!AboutGUI->AddButton(BUTTON_BACK_ID, 1120, 500, "GameMedia\\返回菜单1.png",
		"GameMedia\\返回菜单2.png", "GameMedia\\返回菜单3.png")) return false;
	if (!AboutGUI->AddStaticText(STATIC_TEXT_ID, "版本：v1.0 \n作者：黄伟塨 \n     2015.08.14于广东工业大学",
		540, 150, D3DCOLOR_XRGB(255, 255, 255), AboutGUIFontID)) return false;


	//加载界面布局
	if (!LoadGUI->AddStaticText(STATIC_TEXT_ID, "游戏加载中......",
		200,60, D3DCOLOR_XRGB(33, 55, 255), LoadGUIFontID)) return false;
	

	return true;
}

//---------------------------------------------------------------------
//GUI界面的回调函数
//---------------------------------------------------------------------
void GUICallback(int id, int state, int currentGUI)
{
	switch (id)
	{
	case BUTTON_START_ID:
		if (state == UGP_BUTTON_DOWN)
			CurrentGUI = GUI_START_SCREEN;
		break;
	case BUTTON_ABOUT_ID:
		if (state == UGP_BUTTON_DOWN)
			CurrentGUI = GUI_ABOUT_SCREEN;
		break;
	case BUTTION_HELP_ID:
		if (state == UGP_BUTTON_DOWN)
			CurrentGUI = GUI_HELP_SCREEN;
		break;
	case BUTTON_CONTINUE_ID:
		if (state == UGP_BUTTON_DOWN)
			CurrentGUI = GUI_CONTINUE_SCREEN;
		break;
	case BUTTON_BACK_ID:
		if (state == UGP_BUTTON_DOWN)
			CurrentGUI = GUI_MAIN_SCREEN;
		break;
	case BUTTON_QUIT_ID:
		if (state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;
	}
}

//---------------------------------------------------------------------
//窗口过程回调函数
//---------------------------------------------------------------------
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;


	case WM_LBUTTONDOWN:
		LMBDown = true;
		break;

	case WM_LBUTTONUP:
		LMBDown = false;
		break;

	case WM_MOUSEMOVE:
		MouseX = LOWORD(lParam);
		MouseY = HIWORD(lParam);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//---------------------------------------------------------------------
//主函数
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}
