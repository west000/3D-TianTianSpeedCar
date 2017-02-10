#ifndef __SetAndRenderH__
#define __SetAndRenderH__

#include"D3DUtility.h"
#include"Camera.h"
#include<vector>

using namespace std;

//GUI����
bool SetGUI(IDirect3DDevice9* device);
void GUICallback(int id, int state, int currentGUI);

//�����ı���Ϣ
bool SetInfoText(IDirect3DDevice9* device, LPD3DXFONT* infotext,LPD3DXFONT* counttimetext);

//��ʾ�����ϵ��ı���Ϣ
void ShowInfoText(int goldnum,float speed, float time, float distance,
	int HP, int carnum, int missilenum, int bulletnum, LPD3DXFONT InfoText);

//��������
bool SetBasicSence(IDirect3DDevice9* device);
void RenderBasicSence(IDirect3DDevice9* device,D3DXMATRIX mat);


//����ͨ�õķ��飬������
bool SetSquare(IDirect3DDevice9* device);
bool SetCube(IDirect3DDevice9* device);


//������������ͼƬ
void LoadTexture(IDirect3DDevice9* device);

//���������
void CleanupTexture();


//��Ⱦ��ըЧ��
void RenderBoom(IDirect3DDevice9* device, float timeDelta,
	D3DXMATRIX pos, float scale);

//��Ⱦ������
void RenderCube(IDirect3DDevice9* device,D3DXMATRIX mat, float timeDelta,int i);

//��Ⱦ�������ͼ��
void RenderToolSquare(IDirect3DDevice9* device, Camera& theCamera);



//�����̻�Ч��
bool SetFirework(IDirect3DDevice9* device);

//��Ⱦ�̻�Ч��
void RenderFirework(IDirect3DDevice9* device, D3DXMATRIX position, float timeDelta);

//��Ⱦ�ӵ�������ģ��
void RenderBullet(IDirect3DDevice9* device, D3DXMATRIX trans, float scale,D3DXVECTOR3* pos);

//��Ⱦ���Ͳ
void RenderGun(IDirect3DDevice9* device, D3DXMATRIX trans);




//���������
void ControlCamera(Camera& theCamera, float timeDelta, float speed);

//���Ƴ���
D3DXMATRIX ControlCar(IDirect3DDevice9* device, float timeDelta, float speed, bool isRotate,float* angle);

//
//������������
//
D3DXMATRIX ControlOtherCar(IDirect3DDevice9* device, D3DXVECTOR3 pos);

//��Ⱦ·�ϵ��������
void RenderOtherCar(IDirect3DDevice9* device, float timeDelta, vector<D3DXVECTOR3> &vecCarPos,
	D3DXVECTOR3 CarMin, D3DXVECTOR3 CarMax, D3DXVECTOR3 carpos, d3d::BOUNDINGBOX &BoxCar, D3DXMATRIX matCar,
	int &HP, float &speed, int &carnum, bool isCollision);

#endif //__SetAndRenderH__