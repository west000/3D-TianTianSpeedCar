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

//ȫ���豸
IDirect3DDevice9* Device = 0;

const int Width = 1366;
const int Height = 768;

//�����
Camera TheCamera(Camera::LANDOBJECT);
//��պ�
SkyBoxClass* pSkyBox = 0;

//------------------------------------------------------------------
//GUI����
//------------------------------------------------------------------
bool LMBDown = false;
int MouseX = 0, MouseY = 0;

//���GUI��Ķ��󣬴������ҳ��
D3DGUIClass* MainGUI = NULL;   //���˵�ҳ��
D3DGUIClass* AboutGUI = NULL;  //����ҳ��
D3DGUIClass* HelpGUI = NULL;   //����ҳ��
D3DGUIClass* ContinueGUI = NULL;//����ҳ��
D3DGUIClass* LoadGUI = NULL;    //����ҳ��

int LoadGUIFontID = -1;
int HelpGUIFontID = -1;
int AboutGUIFontID = -1;
int CurrentGUI = GUI_MAIN_SCREEN;

//------------------------------------------------------------------
//���ܺ���
//------------------------------------------------------------------
float RoadCenter[5] = { 0.0f, -14.0f, -5.2f, 5.2f, 14.0f };  //·������λ��

//------------------------------------------------------------------
//�ӵ�������������ģ�͵������
//------------------------------------------------------------------
d3d::BOUNDINGBOX BoxCar;      //���������
d3d::BOUNDINGBOX BoxBullet;   //�ӵ������
d3d::BOUNDINGBOX BoxMissile;  //���������

//------------------------------------------------------------------
//�����ϵ��ı���Ϣ
//------------------------------------------------------------------
LPD3DXFONT InfoText = 0;
LPD3DXFONT CountTimeText = 0;


bool Setup()
{
	//����GUI����
	SetGUI(Device);

	//�����ı���Ϣ
	SetInfoText(Device, &InfoText, &CountTimeText);

	//���������
	D3DXVECTOR3 position(0.0f, 15.0f, -50.0f);
	TheCamera.setPosition(&position);

	//������պ�
	pSkyBox = new SkyBoxClass(Device);
	pSkyBox->LoadShyTextureFromFile("GameMedia\\frontsnow1.jpg", "GameMedia\\backsnow1.jpg",
		"GameMedia\\leftsnow1.jpg", "GameMedia\\rightsnow1.jpg", "GameMedia\\topsnow1.jpg");
	pSkyBox->InitSkyBox(66000);

	//���û�������
	SetBasicSence(Device);

	//����������
	SetCube(Device);

	//���÷���
	SetSquare(Device);

	//�����̻�
	SetFirework(Device);

	//������������ģ��
	LoadAllModel(Device);

	//�����ӵ�������ģ�͵������
	SetBoundingBox(&BoxCar,&BoxBullet,&BoxMissile);

	//���ط���������������
	LoadTexture(Device);

	//�����������
	srand((unsigned int)time(0));

	//����ͶӰ�任
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
	//���������
	void CleanupTexture();

	//���������ڴ�
	void CleanupMesh();
}

//---------------------------------------------------------------------
//չʾ��̨���溯��
//---------------------------------------------------------------------
bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);
		Device->BeginScene();

		//��Ҫͳ�Ƶ����ݣ��ݻٳ��������������������Ѫ��HP = 1000��
		//��ǰ�ӵ�������ǰ������������·�̡�
		static int HP = 200;     //Ѫ��
		static int goldnum = 0;   //�����
		static int bulletnum = 5; //�ӵ���
		static int missilenum = 3;//������
		static int distance = 0;  //·��
		static int carnum = 0;    //�ݻٳ�����
		static float speed = 0.0f;//��ǰ�������ٶ�
		static float glassytime = 0.0f;   //͸��ʱ��
		static float speedtime = 0.0f;    //����ʱ��
		static float missiletime = 0.0f;  //����װ�ص�ʱ��
		static float time = 0.0f;         //��Ϸ���ѵ�ʱ��
		static bool isCollision = true; //�Ե�͸�����Ӿ͹�����ײ����
		static bool isRotate = false;   //�Ե��������ӣ���ת����״̬�Ϳ�����ʹ��5���͹ر�		
		//��ȡ�������������
		float angle;
		D3DXMATRIX matCar = ControlCar(Device, timeDelta, speed, isRotate, &angle);

		if (CurrentGUI == GUI_START_SCREEN )
		{	
			//ͳ����Ϸʱ��
			time += timeDelta;

			//���������
			ControlCamera(TheCamera, timeDelta, speed);
			D3DXMATRIX View;
			TheCamera.getViewMatrix(&View);
			Device->SetTransform(D3DTS_VIEW, &View);

			//��Ⱦ��������
			RenderBasicSence(Device, View);

			//��պ���
			D3DXMATRIX matSky, matTransSky, matRotSky;
			D3DXMatrixTranslation(&matTransSky, 0.0f, 0.0f, 0.0f);
			D3DXMatrixRotationY(&matRotSky, -0.001*timeDelta);
			matSky = matTransSky*matRotSky;
			pSkyBox->RenderSkyBox(&matSky);

			//��Ⱦ����
			RenderCar(Device, timeDelta, matCar,!isCollision);
			//�����ϵĻ��Ͳ
			RenderGun(Device, matCar);

			//��ȡ������λ��
			D3DXVECTOR3 carpos;
			carpos.x = matCar._41;  //x����
			carpos.y = matCar._42;  //y����
			carpos.z = matCar._43;  //z����
			D3DXVECTOR3 CarMin = D3DXVECTOR3(carpos.x, carpos.y, carpos.z) + BoxCar.vMin;  //������������С����
			D3DXVECTOR3 CarMax = D3DXVECTOR3(carpos.x, carpos.y, carpos.z) + BoxCar.vMax;  //�����������������

			//�ı���Ϣ
			RECT rect = { 600, 300, 700, 400 };
			static char strInfo[256] = { 0 };

			//������ӵ�λ������
			static std::vector<D3DXVECTOR3> vecCubePos(5);
			//���������λ������
			static std::vector<D3DXVECTOR3> vecCarPos(5);


			//-------------------------------------------------------------------
			//3���ӵ�׼��ʱ��
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
				
				//����������ܺ��ӵ�λ��
				for (vector<D3DXVECTOR3>::iterator iter = vecCubePos.begin();
					iter != vecCubePos.end(); ++iter)
				{
					iter->x = RoadCenter[rand() % 4 + 1];
					iter->y = 3.0f;
					iter->z = (rand() % 25) * 125;
				}

				//�������·�ϵĳ���
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
				//��Ϸ��ʼ��ǰ3�룬�ٶ�����
				if (time < 6.0f)
					speed += 0.3f;
				if (speed < 0.0f)
					speed = 0.0f;
				if (speed > 500.0f)
					speed = 500.0f;

				//͸��ʱ�����
				if (glassytime > 0.0f)
					glassytime -= timeDelta;
				else
				{
					glassytime = 0.0f;
					isCollision = true;
				}

				//����ʱ�����
				if (speedtime > 0.01f)
					speed += 0.06f;
				else
					speedtime = 0.0f;

				//ƫת�Ƕ���[-D3DX_PI*0.2��D3DX_PI*0.2]֮�⣬�����
				if (angle > D3DX_PI*0.2 || angle < -D3DX_PI*0.2)
					speed -= timeDelta;

				//-----------------------------------------------------------------
				//��·����Ⱦ����ĵ��ߺ���
				//-----------------------------------------------------------------
				int num = 0;
				for (std::vector<D3DXVECTOR3>::iterator iter = vecCubePos.begin();
					iter != vecCubePos.end(); ++iter)
				{
					if (iter->z > carpos.z - 30.0f)    //����Ұ��Χ�ھ���Ⱦ���ܺ���
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
							case 1:   //�Ե���͸����
								isCollision = false;
								glassytime = 5.0f;
								break;
							case 2:   //�Ե���������
								missilenum += 1;
								bulletnum += 5;
								break;
							case 3:   //�Ե����ٺ���
								speedtime = 2.0f;
								break;
							case 4:   //�Ե���������
								isRotate = true;
								break;
							case 0:   //�Ե���Һ���
								goldnum += 10;
								HP += 50;
								break;
							}
							iter->z -= 50.0f;  //�Ե����ӣ����Ӿ���ʧ
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
				//��·����Ⱦ�������
				//-----------------------------------------------------------------
				RenderOtherCar(Device, timeDelta, vecCarPos, CarMin, CarMax, carpos, BoxCar, matCar, HP, speed, carnum, isCollision);

				//-----------------------------------------------------------------
				//�������
				//-----------------------------------------------------------------
				static D3DXVECTOR3 miss_pos;      //������ǰ��λ��
				static bool isfire = false;       //�����Ƿ���
				static float miss_fly = 0.0f;     //�����ķ�������
				static float miss_time = 0.0f;    //�������кͱ�ը����ʱ��
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
								carnum++;   //�����ݻ�+1
								iter->z = carpos.z - 100.0f; //���ݻٵĳ�����ʧ
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
			//�ӵ����
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
						carnum++;   //�����ݻ�+1
						bullet_time = 1.0f;
						iter->z = carpos.z - 100.0f; //���ݻٵĳ�����ʧ
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
			//��ʾ��Ϸ��Ϣ
			//-----------------------------------------------------------------
			RenderToolSquare(Device, TheCamera);
			ShowInfoText(goldnum, speed, time, carpos.z, HP, carnum, missilenum, bulletnum, InfoText);

			//-----------------------------------------------------------------
			//��Ϸ����������
			//-----------------------------------------------------------------
			if (HP <= 0)
			{
				speed -= timeDelta * 50.0f;  //HPΪ0ʱ���ٶȼ���
				D3DXMATRIX boomPos;
				D3DXMatrixTranslation(&boomPos, carpos.x, carpos.y, carpos.z - 10.0f);
				RenderBoom(Device, timeDelta, boomPos, 10.0f);

				rect.top = 300;
				rect.left = 120;
				sprintf_s(strInfo, "��������...");
				CountTimeText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::RED);
			}
			if (carpos.z > 30000.0f && speed>0.1f)  //�ִ��յ�ʱ�ٶȼ���
				speed -= timeDelta * 100.0f;
			if (carpos.z > 30000.0f)
			{
				rect.top = 300;
				rect.left = 180;
				sprintf_s(strInfo, "�ִ��յ�");
				CountTimeText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::WHITE);

				D3DXMATRIX firework;
				D3DXMatrixTranslation(&firework, 0.0f, 0.0f, carpos.z + 100.0f);
				RenderFirework(Device, firework, timeDelta);
			}

			//-----------------------------------------------------------
			//����home����ͣ��Ϸ���ص�������Ϸҳ��
			//-----------------------------------------------------------
			if (::GetAsyncKeyState(VK_HOME) & 0x8000f)
				CurrentGUI = GUI_CONTINUE_SCREEN;
		}
		else if (CurrentGUI == GUI_MAIN_SCREEN)
		{
			ProcessGUI(MainGUI, LMBDown, MouseX, MouseY, GUI_MAIN_SCREEN, GUICallback);

			//�ص�����������������Ҫ���³�ʼ��
			HP = 200;      //Ѫ��
			goldnum = 0;   //�����
			bulletnum = 5; //�ӵ���
			missilenum = 3; //������
			distance = 0;   //·��
			carnum = 0;     //�ݻٳ�����
			speed = 0.0f;   //��ǰ�������ٶ�
			glassytime = 0.0f;   //͸��ʱ��
			speedtime = 0.0f;    //����ʱ��
			missiletime = 0.0f;  //����װ�ص�ʱ��
			time = 0.0f;         //��Ϸ���ѵ�ʱ��
			isCollision = true;  //�Ե�͸�����Ӿ͹�����ײ����
			isRotate = false;    //�ٵ��������ӣ���ת����״̬�Ϳ�����ʹ��5���͹ر�
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
//����GUI����
//---------------------------------------------------------------------
bool SetGUI(IDirect3DDevice9* device)
{
	//����GUIϵͳ
	MainGUI = new D3DGUIClass(Device, Width, Height);    //���˵�ҳ��
	AboutGUI = new D3DGUIClass(Device, Width, Height);   //����ҳ��
	HelpGUI = new D3DGUIClass(Device, Width, Height);    //������Ϣҳ��
	ContinueGUI = new D3DGUIClass(Device, Width, Height);//����ҳ��
	LoadGUI = new D3DGUIClass(Device, Width, Height);    //����ҳ��
	
	//Ϊ���ҳ���������ͼ
	if (!MainGUI->AddBackground("GameMedia\\maingui.jpg")) return false;
	if (!AboutGUI->AddBackground("GameMedia\\GUI1.jpg")) return false;
	if (!HelpGUI->AddBackground("GameMedia\\GUI2.jpg")) return false;
	if (!ContinueGUI->AddBackground("GameMedia\\GUI3.jpg")) return false;
	if (!LoadGUI->AddBackground("GameMedia\\maingui.jpg")) return false;
	
	//Ϊ���ڽ��桢���������������
	if (!AboutGUI->CreateTextFont("΢���ź�", 35, &AboutGUIFontID)) return false;
	if (!HelpGUI->CreateTextFont("΢���ź�", 35, &HelpGUIFontID)) return false;
	if (!LoadGUI->CreateTextFont("΢���ź�", 40, &LoadGUIFontID)) return false;

	//���˵�����
	if (!MainGUI->AddButton(BUTTON_START_ID, 650, 340, "GameMedia\\��ʼ��Ϸ1.png",
		"GameMedia\\��ʼ��Ϸ2.png", "GameMedia\\��ʼ��Ϸ3.png")) return false;

	if (!MainGUI->AddButton(BUTTION_HELP_ID, 650, 385, "GameMedia\\����1.png",
		"GameMedia\\����2.png", "GameMedia\\����3.png")) return false;

	if (!MainGUI->AddButton(BUTTON_ABOUT_ID, 650, 430, "GameMedia\\����1.png",
		"GameMedia\\����2.png", "GameMedia\\����3.png")) return false;

	if (!MainGUI->AddButton(BUTTON_QUIT_ID, 650, 475, "GameMedia\\�˳�1.png",
		"GameMedia\\�˳�2.png", "GameMedia\\�˳�3.png")) return false;


	//����ҳ�沼��
	if (!ContinueGUI->AddButton(BUTTON_START_ID, 650, 480, "GameMedia\\������Ϸ1.png",
		"GameMedia\\������Ϸ2.png", "GameMedia\\������Ϸ3.png")) return false;
	if (!ContinueGUI->AddButton(BUTTON_BACK_ID, 650, 525, "GameMedia\\���ز˵�1.png",
		"GameMedia\\���ز˵�2.png", "GameMedia\\���ز˵�3.png")) return false;


	//����ҳ�沼��
	if (!HelpGUI->AddButton(BUTTON_BACK_ID, 1120, 500, "GameMedia\\���ز˵�1.png",
		"GameMedia\\���ز˵�2.png", "GameMedia\\���ز˵�3.png")) return false;
	if (!HelpGUI->AddStaticText(STATIC_TEXT_ID, 
		"����������£�\n \
		1������A��D���������ұ��\n \
		2������C���л��ӽ�\n \
		3������J������\n \
		4�����¿ո��չ������繥��\n \
		5������T�����䵼��\n \
		6������G�������ӵ�\n\n \
		���߹��ܣ� \n \
		 ��Һ��ӣ�+10���\n  ͸�����ӣ���͸����\n  ���ٺ��ӣ�����2����\n  �������ӣ�+3������+10�ӵ�\n  �������ӣ��������ת����\n ",
		200, 150, D3DCOLOR_XRGB(0,255, 255), HelpGUIFontID)) return false;


	//���ڽ��沼��
	if (!AboutGUI->AddButton(BUTTON_BACK_ID, 1120, 500, "GameMedia\\���ز˵�1.png",
		"GameMedia\\���ز˵�2.png", "GameMedia\\���ز˵�3.png")) return false;
	if (!AboutGUI->AddStaticText(STATIC_TEXT_ID, "�汾��v1.0 \n���ߣ���ΰ�b \n     2015.08.14�ڹ㶫��ҵ��ѧ",
		540, 150, D3DCOLOR_XRGB(255, 255, 255), AboutGUIFontID)) return false;


	//���ؽ��沼��
	if (!LoadGUI->AddStaticText(STATIC_TEXT_ID, "��Ϸ������......",
		200,60, D3DCOLOR_XRGB(33, 55, 255), LoadGUIFontID)) return false;
	

	return true;
}

//---------------------------------------------------------------------
//GUI����Ļص�����
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
//���ڹ��̻ص�����
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
//������
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
