#ifndef __LoadAndSetMeshH__
#define __LoadAndSetMeshH__
#include<cstring>
#include"D3DUtility.h"

using namespace std;


//
//加载X文件中的网格模型
//
bool LoadMeshModel(IDirect3DDevice9* device,char* filename,
	ID3DXMesh** mesh, vector<D3DMATERIAL9> &vecMtrl, vector<IDirect3DTexture9*> &vecTex);

//
//加载所有网格模型
//
bool LoadAllModel(IDirect3DDevice9* device);

//
//清除网格的内存
//
void CleanupMesh();

//
//设置各个模型的外接体
//
bool SetBoundingBox(d3d::BOUNDINGBOX *car_box, d3d::BOUNDINGBOX *bullet_box, d3d::BOUNDINGBOX *missile_box);

//
//对网格模型进行碰撞测试
//
bool CollisionTest(D3DXVECTOR3 vMin1, D3DXVECTOR3 vMax1,
	D3DXVECTOR3 vMin2, D3DXVECTOR3 vMax2);

//
//渲染车辆模型
//
void RenderCar(IDirect3DDevice9* device,float timeDelta, D3DXMATRIX trans,bool glassy);

#endif //__LoadAndSetMeshH__