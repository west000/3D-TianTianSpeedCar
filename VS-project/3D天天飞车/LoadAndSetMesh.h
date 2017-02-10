#ifndef __LoadAndSetMeshH__
#define __LoadAndSetMeshH__
#include<cstring>
#include"D3DUtility.h"

using namespace std;


//
//����X�ļ��е�����ģ��
//
bool LoadMeshModel(IDirect3DDevice9* device,char* filename,
	ID3DXMesh** mesh, vector<D3DMATERIAL9> &vecMtrl, vector<IDirect3DTexture9*> &vecTex);

//
//������������ģ��
//
bool LoadAllModel(IDirect3DDevice9* device);

//
//���������ڴ�
//
void CleanupMesh();

//
//���ø���ģ�͵������
//
bool SetBoundingBox(d3d::BOUNDINGBOX *car_box, d3d::BOUNDINGBOX *bullet_box, d3d::BOUNDINGBOX *missile_box);

//
//������ģ�ͽ�����ײ����
//
bool CollisionTest(D3DXVECTOR3 vMin1, D3DXVECTOR3 vMax1,
	D3DXVECTOR3 vMin2, D3DXVECTOR3 vMax2);

//
//��Ⱦ����ģ��
//
void RenderCar(IDirect3DDevice9* device,float timeDelta, D3DXMATRIX trans,bool glassy);

#endif //__LoadAndSetMeshH__