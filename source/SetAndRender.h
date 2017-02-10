#ifndef __SetAndRenderH__
#define __SetAndRenderH__

#include"D3DUtility.h"
#include"Camera.h"
#include<vector>

using namespace std;

//GUI界面
bool SetGUI(IDirect3DDevice9* device);
void GUICallback(int id, int state, int currentGUI);

//设置文本信息
bool SetInfoText(IDirect3DDevice9* device, LPD3DXFONT* infotext,LPD3DXFONT* counttimetext);

//显示界面上的文本信息
void ShowInfoText(int goldnum,float speed, float time, float distance,
	int HP, int carnum, int missilenum, int bulletnum, LPD3DXFONT InfoText);

//基本场景
bool SetBasicSence(IDirect3DDevice9* device);
void RenderBasicSence(IDirect3DDevice9* device,D3DXMATRIX mat);


//设置通用的方块，立方体
bool SetSquare(IDirect3DDevice9* device);
bool SetCube(IDirect3DDevice9* device);


//加载所有纹理图片
void LoadTexture(IDirect3DDevice9* device);

//清除纹理缓存
void CleanupTexture();


//渲染爆炸效果
void RenderBoom(IDirect3DDevice9* device, float timeDelta,
	D3DXMATRIX pos, float scale);

//渲染立方体
void RenderCube(IDirect3DDevice9* device,D3DXMATRIX mat, float timeDelta,int i);

//渲染界面道具图标
void RenderToolSquare(IDirect3DDevice9* device, Camera& theCamera);



//设置烟花效果
bool SetFirework(IDirect3DDevice9* device);

//渲染烟花效果
void RenderFirework(IDirect3DDevice9* device, D3DXMATRIX position, float timeDelta);

//渲染子弹、导弹模型
void RenderBullet(IDirect3DDevice9* device, D3DXMATRIX trans, float scale,D3DXVECTOR3* pos);

//渲染火箭筒
void RenderGun(IDirect3DDevice9* device, D3DXMATRIX trans);




//控制摄像机
void ControlCamera(Camera& theCamera, float timeDelta, float speed);

//控制车辆
D3DXMATRIX ControlCar(IDirect3DDevice9* device, float timeDelta, float speed, bool isRotate,float* angle);

//
//控制其他车辆
//
D3DXMATRIX ControlOtherCar(IDirect3DDevice9* device, D3DXVECTOR3 pos);

//渲染路上的随机车辆
void RenderOtherCar(IDirect3DDevice9* device, float timeDelta, vector<D3DXVECTOR3> &vecCarPos,
	D3DXVECTOR3 CarMin, D3DXVECTOR3 CarMax, D3DXVECTOR3 carpos, d3d::BOUNDINGBOX &BoxCar, D3DXMATRIX matCar,
	int &HP, float &speed, int &carnum, bool isCollision);

#endif //__SetAndRenderH__