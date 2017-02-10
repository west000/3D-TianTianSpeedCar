#include"D3DUtility.h"
#include"SetAndRender.h"
#include"pSystem.h"
#include"LoadAndSetMesh.h"
#include<vector>

using namespace std;

//------------------------------------------------------------------
//��������
//------------------------------------------------------------------
ID3DXMesh* Pillar = 0;                        //·���ߵ�����
IDirect3DVertexBuffer9* BasicSenceVertex = 0; //�װ�Ķ��㻺��
IDirect3DTexture9* BasicSenceTex[3] = { 0 };  // ����������ͼ��ɳĮ����·����ľ

//------------------------------------------------------------------
//һ�����飬����������ͼ�ꡢ��ը���顢������������ľ
//------------------------------------------------------------------
IDirect3DVertexBuffer9* SquareVertexBuffer = 0;
IDirect3DTexture9* TreeTex = 0;          //��������
IDirect3DTexture9* ToolTex[3] = { 0 };   //���ֵ��߷��������
IDirect3DTexture9* BoomTex[25] = { 0 };  //��ը����

//------------------------------------------------------------------
//һ�������壬�������Ͳ��·�ϵ��ߵ�͸��������
//------------------------------------------------------------------
ID3DXMesh* CubeMesh = 0;
IDirect3DTexture9* CubeTex[5] = {0};
IDirect3DTexture9* GunTex = 0;           //���Ͳ������

//------------------------------------------------------------------
//�̻���Ч
//------------------------------------------------------------------
psys::PSystem* Firework = 0;

extern float RoadCenter[5];


//���û�������
bool SetBasicSence(IDirect3DDevice9* device)
{
	device->CreateVertexBuffer(
		18 * sizeof(d3d::Vertex),
		D3DUSAGE_WRITEONLY,
		d3d::Vertex::FVF,
		D3DPOOL_MANAGED,
		&BasicSenceVertex,
		0);

	d3d::Vertex *v;
	BasicSenceVertex->Lock(0, 0, (void**)&v, 0);

	//���ɳĮ
	v[0] = { -34000.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	v[1] = { -30000.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	v[2] = { -20.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	v[3] = { -34000.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	v[4] = { -20.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	v[5] = { -20.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

	//·��
	v[6] = { -20.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	v[7] = { -20.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	v[8] = { 20.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	v[9] = { -20.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	v[10] = { 20.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	v[11] = { 20.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

	//�ұ�ɳĮ
	v[12] = { 20.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	v[13] = { 20.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	v[14] = { 30000.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	v[15] = { 20.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	v[16] = { 34000.0f, 0, 34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	v[17] = { 34000.0f, 0, -34000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

	BasicSenceVertex->Unlock();
	D3DXCreateTextureFromFile(
		device,
		"GameMedia\\sea.jpg",
		&BasicSenceTex[0]);

	D3DXCreateTextureFromFile(
		device,
		"GameMedia\\road.jpg",
		&BasicSenceTex[1]);

	D3DXCreateTextureFromFile(
		device,
		"GameMedia\\tree.png",
		&BasicSenceTex[2]);

	//����Բ����
	D3DXCreateCylinder(device, 0.5f, 0.5f, 5.0f, 20, 20, &Pillar, 0);

	return true;
}


//��Ⱦ��������
void RenderBasicSence(IDirect3DDevice9* device,D3DXMATRIX view)
{
	//��������Ѱַģʽ
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//���õƹ�
	D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);
	device->SetLight(0, &light);
	device->LightEnable(0, true);

	//������Ⱦ״̬
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
	
	//����任
	D3DXMATRIX T, R, P, S;
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);
	D3DXMatrixIdentity(&T);  //��ʱTΪһ��4��4�ĵ�λ����
	T = T * S;               //��ʱT�ĵ����Խ�����S�����Խ��ߣ�T������Ԫ�ض�Ϊ0
	device->SetTransform(D3DTS_WORLD, &T);

	//��Ⱦ·�桢ɳĮ
	device->SetStreamSource(0, BasicSenceVertex, 0, sizeof(d3d::Vertex));
	device->SetMaterial(&d3d::WHITE_MTRL);
	device->SetFVF(d3d::Vertex::FVF);

	device->SetTexture(0, BasicSenceTex[0]);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	device->SetTexture(0, BasicSenceTex[1]);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 2);
	device->SetTexture(0, BasicSenceTex[0]);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 12, 2);


	//��Ⱦ·���ߵ���
	device->SetRenderState(D3DRS_SPECULARENABLE, false);
	device->SetStreamSource(0, SquareVertexBuffer, 0, sizeof(d3d::Vertex));
	device->SetFVF(d3d::Vertex::FVF);
	device->SetTexture(0, TreeTex);
	device->SetMaterial(&d3d::WHITE_MTRL);
	D3DXMatrixScaling(&S, 3.0f, 6.0f, 1.0f);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);  //�����ں�ͨ��
	for (int i = 0; i < 375; i++)
	{
		D3DXMatrixTranslation(&T, -22.0f, 5.0f, -10.0f+ (i * 80.0f));
		P = S * T;
		device->SetTransform(D3DTS_WORLD, &P);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		D3DXMatrixTranslation(&T, 22.0f, 5.0f, -10.0f + (i * 80.0f));
		P = S * T;
		device->SetTransform(D3DTS_WORLD, &P);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);  //�ر��ں�ͨ��


	//��Ⱦ·���ߵ�����
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
	device->SetMaterial(&d3d::YELLOW_MTRL);
	D3DXMatrixScaling(&S, 1.0f, 2.0f, 1.0f);
	for (int i = 0; i < 600; i++)
	{
		D3DXMatrixTranslation(&T, -20.0f, 0.0f, -30.0f + (i * 50.0f));
		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		Pillar->DrawSubset(0);

		D3DXMatrixTranslation(&T, 20.0f, 0.0f, -30.0f + (i * 50.0f));
		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		Pillar->DrawSubset(0);
	}
}


//���÷���
bool SetSquare(IDirect3DDevice9* device)
{
	device->CreateVertexBuffer(
		6 * sizeof(d3d::Vertex),
		D3DUSAGE_WRITEONLY,
		d3d::Vertex::FVF,
		D3DPOOL_MANAGED,
		&SquareVertexBuffer,
		0);

	d3d::Vertex *v;
	SquareVertexBuffer->Lock(0, 0, (void**)&v, 0);

	v[0] = d3d::Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = d3d::Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = d3d::Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = d3d::Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = d3d::Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = d3d::Vertex(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	SquareVertexBuffer->Unlock();

	return true;
}


//����������
bool SetCube(IDirect3DDevice9* device)
{
	HRESULT hr = 0;
	hr = D3DXCreateMeshFVF(
		12,
		24,
		D3DXMESH_MANAGED,
		d3d::Vertex::FVF,
		device,
		&CubeMesh);

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXCreateMeshFVF() - FAILED", 0, 0);
		return false;
	}

	d3d::Vertex* v = 0;
	CubeMesh->LockVertexBuffer(0, (void**)&v);

	// ǰ�涥������
	v[0] = d3d::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = d3d::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = d3d::Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = d3d::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	//���涥������
	v[4] = d3d::Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[5] = d3d::Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = d3d::Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = d3d::Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// ���涥������
	v[8] = d3d::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[9] = d3d::Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = d3d::Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[11] = d3d::Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// ���涥������
	v[12] = d3d::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = d3d::Vertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = d3d::Vertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = d3d::Vertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// ���涥������
	v[16] = d3d::Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[17] = d3d::Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[18] = d3d::Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[19] = d3d::Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// ���涥������
	v[20] = d3d::Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[21] = d3d::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = d3d::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[23] = d3d::Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	CubeMesh->UnlockVertexBuffer();

	WORD* indices = 0;
	CubeMesh->LockIndexBuffer(0, (void**)&indices);

	// ���ǰ����������
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// ��������������
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// ��䶥����������
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// ��������������
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// ���������������
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// ���������������
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	CubeMesh->UnlockIndexBuffer();


	DWORD* attributeBuffer = 0;
	CubeMesh->LockAttributeBuffer(0, &attributeBuffer);
	for (int a = 0; a < 2; a++)
		attributeBuffer[a] = 0;

	for (int b = 2; b < 4; b++)
		attributeBuffer[b] = 1;

	for (int c = 4; c < 6; c++)
		attributeBuffer[c] = 2;

	for (int a = 6; a < 8; a++)
		attributeBuffer[a] = 3;

	for (int b = 8; b < 10; b++)
		attributeBuffer[b] = 4;

	for (int c = 10; c < 12; c++)
		attributeBuffer[c] = 5;
	CubeMesh->UnlockAttributeBuffer();
}


//��������ͼƬ
void LoadTexture(IDirect3DDevice9* device)
{
	//��ը�����25������
	D3DXCreateTextureFromFile(device, "BoomTex\\explode00.dds", &BoomTex[0]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode01.dds", &BoomTex[1]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode02.dds", &BoomTex[2]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode03.dds", &BoomTex[3]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode04.dds", &BoomTex[4]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode05.dds", &BoomTex[5]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode06.dds", &BoomTex[6]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode07.dds", &BoomTex[7]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode08.dds", &BoomTex[8]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode09.dds", &BoomTex[9]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode10.dds", &BoomTex[10]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode11.dds", &BoomTex[11]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode12.dds", &BoomTex[12]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode13.dds", &BoomTex[13]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode14.dds", &BoomTex[14]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode15.dds", &BoomTex[15]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode16.dds", &BoomTex[16]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode17.dds", &BoomTex[17]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode18.dds", &BoomTex[18]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode19.dds", &BoomTex[19]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode20.dds", &BoomTex[20]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode21.dds", &BoomTex[21]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode22.dds", &BoomTex[22]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode23.dds", &BoomTex[23]);
	D3DXCreateTextureFromFile(device, "BoomTex\\explode24.dds", &BoomTex[24]);

	//���ֵ��߷��������
	D3DXCreateTextureFromFile(device, "GameMedia\\money.jpg", &ToolTex[0]);
	D3DXCreateTextureFromFile(device, "GameMedia\\zhadan.jpg", &ToolTex[1]);
	D3DXCreateTextureFromFile(device, "GameMedia\\huojiantong.jpg", &ToolTex[2]);

	//��ľ������
	D3DXCreateTextureFromFile(device, "GameMedia\\tree.png", &TreeTex);

	//���Ͳ����
	D3DXCreateTextureFromFile(device, "GameMedia\\gun.jpg", &GunTex);

	//���ֵ��������������
	D3DXCreateTextureFromFile(device, "GameMedia\\GoldCube.dds", &CubeTex[0]);  //�������
	D3DXCreateTextureFromFile(device, "GameMedia\\GlassyCube.dds", &CubeTex[1]); //͸����͸����
	D3DXCreateTextureFromFile(device, "GameMedia\\RocketCube.dds", &CubeTex[2]);  //��������
	D3DXCreateTextureFromFile(device, "GameMedia\\SpeedCube.dds", &CubeTex[3]);   //���ٺ���
	D3DXCreateTextureFromFile(device, "GameMedia\\RotateCube.dds", &CubeTex[4]);  //��������
}


//��Ⱦ�����ϵĵ���ͼ��
void RenderToolSquare(IDirect3DDevice9* device, Camera& theCamera)
{
	D3DXVECTOR3 pos;
	theCamera.getPosition(&pos);
	
	//Alpha�������, ���û��ϵ��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_LIGHTING, false);//�ص�

	device->SetStreamSource(0, SquareVertexBuffer, 0, sizeof(d3d::Vertex));
	device->SetMaterial(&d3d::WHITE_MTRL);
	device->SetFVF(d3d::Vertex::FVF);

	D3DXMATRIX move;
	D3DXMatrixTranslation(&move, pos.x + 13.0f, pos.y + 6.0f, pos.z + 20.0f);
	device->SetTransform(D3DTS_WORLD, &move);
	device->SetTexture(0, ToolTex[0]);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	D3DXMatrixTranslation(&move, pos.x + 13.0f, pos.y + 2.00f, pos.z + 20.0f);
	device->SetTransform(D3DTS_WORLD, &move);
	device->SetTexture(0, ToolTex[1]);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	D3DXMatrixTranslation(&move, pos.x + 13.0f, pos.y + -2.0f, pos.z + 20.0f);
	device->SetTransform(D3DTS_WORLD, &move);
	device->SetTexture(0, ToolTex[2]);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	device->SetRenderState(D3DRS_LIGHTING, true); //����
}


//��Ⱦ·���ϵĵ���������
void RenderCube(IDirect3DDevice9* device, D3DXMATRIX mat, float timeDelta, int i)
{
	static float angle = D3DX_PI*0.25;
	angle += timeDelta;
	if (angle > 6.28)
		angle = 0.0f;

	D3DXMATRIX result, Ry, Rx;
	D3DXMatrixRotationX(&Rx, D3DX_PI*0.25);
	D3DXMatrixRotationY(&Ry, angle);    //���������ת
	result = Rx * Ry * mat;


	device->SetTransform(D3DTS_WORLD, &result);

	//Alpha�������, ���û��ϵ��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//alphaͨ������������
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetMaterial(&d3d::WHITE_MTRL);
	device->SetTexture(0, CubeTex[i]);
	for (int i = 0; i < 6; ++i)
		CubeMesh->DrawSubset(i);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

//��Ⱦ��ըЧ��
void RenderBoom(IDirect3DDevice9* device, float timeDelta,
	D3DXMATRIX pos, float scale)
{
	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling, scale, scale, scale);

	D3DXMATRIX final;
	final = scaling * pos;
	device->SetTransform(D3DTS_WORLD, &final);

	//��ը����ͼƬ�л���������ըЧ��
	static int i = 0;
	static float time = 0.0f;
	time += timeDelta;
	if (time > 0.1f)
	{
		i += 2;
		time = 0.0f;
	}
	device->SetRenderState(D3DRS_LIGHTING, false);  //�رյƹ�
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);  //�����ں�ͨ��
	
	device->SetMaterial(&d3d::WHITE_MTRL);
	device->SetTexture(0, BoomTex[i % 25]);
	device->SetStreamSource(0, SquareVertexBuffer, 0, sizeof(d3d::Vertex));
	device->SetFVF(d3d::Vertex::FVF);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);  //�ر��ں�ͨ��
	device->SetRenderState(D3DRS_LIGHTING, true);    //�����ƹ�
}


//�����̻�Ч��
bool SetFirework(IDirect3DDevice9* device)
{
	D3DXVECTOR3 origin(0.0f, 10.0f, 50.0f);
	Firework = new psys::Firework(&origin, 6000);
	Firework->init(device, "GameMedia\\flare.bmp");

	return true;
}


//��Ⱦ�̻�Ч��
void RenderFirework(IDirect3DDevice9* device, D3DXMATRIX position, float timeDelta)
{
	Firework->update(timeDelta);

	if (Firework->isDead())
		Firework->reset();

	device->SetTransform(D3DTS_WORLD, &position);
	Firework->render();

}


//��Ⱦ���Ͳ
void RenderGun(IDirect3DDevice9* device, D3DXMATRIX trans)
{
	D3DXMATRIX moving, scaling, result;
	D3DXMatrixTranslation(&moving, 0.0f, 11.5f, -1.0f);
	D3DXMatrixScaling(&scaling, 3.0f, 3.0f, 20.0f);
	result = moving * scaling *trans;

	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetTransform(D3DTS_WORLD, &result);
	device->SetMaterial(&d3d::WHITE_MTRL);
	device->SetTexture(0, GunTex);
	for (int i = 0; i < 6; i++)
		CubeMesh->DrawSubset(i);
	device->SetRenderState(D3DRS_LIGHTING, true);
}

//��Ⱦ·�ϵ��������
void RenderOtherCar(IDirect3DDevice9* device, float timeDelta, vector<D3DXVECTOR3> &vecCarPos,
	D3DXVECTOR3 CarMin, D3DXVECTOR3 CarMax, D3DXVECTOR3 carpos, d3d::BOUNDINGBOX &BoxCar, D3DXMATRIX matCar,
	int &HP, float &speed, int &carnum, bool isCollision)
{
	int num = 0;
	static float crashtime[5] = { 0.0f };  //������ײ���ڷɵ�ʱ��
	static int direct[5] = { 0 };          //���ӱ�ײ��ķ���
	static float car_roll[5] = { 0.0f };   //ÿһ����ײ�ĳ����ڿ��з�ת�ĽǶ�
	static float car_fly[5] = { 0.0f };    //ÿһ����ײ�ĳ��ӱ�ײ�ɵľ���
	static float car_speed[5] = { 0.0f };  //ÿһ����ײ�ĳ��ӱ�ײ��ķ����ٶ�
	int showcar = 0;
	for (std::vector<D3DXVECTOR3>::iterator iter = vecCarPos.begin();
		iter != vecCarPos.end(); ++iter)
	{
		//��ʾ�����ķ�Χ��û�н�����Ұ�ĳ���������Ⱦ
		if (iter->z > carpos.z - 80.0f)
		{
			++showcar;
			car_speed[num] += 0.01f;
			iter->z -= car_speed[num] * timeDelta;
			D3DXMATRIX matcar = ControlOtherCar(device, *iter);

			if (crashtime[num]<0.001f)
				RenderCar(device, timeDelta, matcar, false);

			D3DXVECTOR3 tempmin = *iter + BoxCar.vMin;
			D3DXVECTOR3 tempmax = *iter + BoxCar.vMax;
			if (CollisionTest(CarMin, CarMax, tempmin, tempmax) && crashtime[num] < 0.001f && isCollision)
			{
				HP -= 50;  //ײ��һ������50 HP	
				speed -= 5.0f;  //�ٶȼ���5
				++carnum;
				crashtime[num] = 1.5f;
				if (iter->x > carpos.x + 1.0f)
					direct[num] = 1;
				else if (iter->x<carpos.x - 1.0f)
					direct[num] = -1;
				else
					direct[num] = 2;
			}

			if (crashtime[num] > 0.001f)
			{
				crashtime[num] -= timeDelta;

				D3DXMATRIX move, roll, result;
				D3DXMatrixRotationY(&result, D3DX_PI);
				if (direct[num] == 1)    //���ҷ�
				{
					car_fly[num] += timeDelta;
					car_roll[num] += timeDelta;
					if (car_roll[num] > D3DX_PI)
						car_roll[num] = 0.0f;

					D3DXMatrixTranslation(&move, car_fly[num] * 180.0f, car_fly[num] * 40.0f, car_fly[num] * 50.0f);
					D3DXMatrixRotationZ(&roll, car_roll[num]);
				}
				else if (direct[num] == -1)   //�����
				{
					car_fly[num] += timeDelta;
					car_roll[num] -= timeDelta;
					if (car_roll[num]< -D3DX_PI)
						car_roll[num] = 0.0f;

					D3DXMatrixTranslation(&move, -car_fly[num] * 180.0f, car_fly[num] * 40.0f, car_fly[num] * 50.0f);
					D3DXMatrixRotationZ(&roll, car_roll[num]);
				}
				else if (direct[num] == 2)    //��ǰ��
				{
					car_fly[num] += timeDelta * 5.0f;
					car_roll[num] -= timeDelta * 3.0f;
					if (car_roll[num]<-D3DX_PI)
						car_roll[num] = 0.0f;

					D3DXMatrixTranslation(&move, 0.0f, car_fly[num] * 10.0f, car_fly[num] * 60.0f);
					D3DXMatrixRotationX(&roll, car_roll[num]);
				}

				result = roll * result * move * matCar;
				RenderCar(device, timeDelta, result, false);
				D3DXMATRIX boomPos;
				D3DXMatrixTranslation(&boomPos, result._41, result._42, result._43);

				RenderBoom(device, timeDelta, boomPos, 10.0f);
				PlaySound("GameMedia\\carboom.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP | SND_NODEFAULT);
			}
			else
			{
				crashtime[num] = 0.0f;
				direct[num] = 0;
				car_roll[num] = 0;
				car_fly[num] = 0;
				car_speed[num] = 0;
			}
		}
		else
		{
			crashtime[num] = 0.0f;
			direct[num] = 0;
			car_roll[num] = 0;
			car_fly[num] = 0;
			car_speed[num] = 0;
		}
		++num;
	}

	//�����·�ϵ������������û�ˣ��ͼ���������������
	if (showcar == 0)
	{
		for (vector<D3DXVECTOR3>::iterator iter = vecCarPos.begin();
			iter != vecCarPos.end(); ++iter)
		{
			iter->x = RoadCenter[rand() % 4 + 1];
			iter->y = 0.0f;
			iter->z = carpos.z + (rand() % 25) * 125 + 50.0f;
		}
	}
}

//���ý����ϵ��ı���Ϣ
bool SetInfoText(IDirect3DDevice9* device,LPD3DXFONT* infotext,LPD3DXFONT* counttimetext)
{
	D3DXCreateFont(device, 25, 0, 1000, 0, true, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Times New Roman", infotext);

	D3DXCreateFont(device, 300, 0, 1000, 0,true, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Times New Roman", counttimetext);
	return true;
}

//��ʾ�����ϵ��ı���Ϣ
void ShowInfoText(int goldnum, float speed, float time, float distance,
	 int HP,int carnum, int missilenum, int bulletnum, LPD3DXFONT InfoText)
{
	RECT rect = { 600, 300, 700, 400 };
    static char strInfo[256] = { 0 };

	rect.top = 180.0f;
	rect.left = 1230.0f;
	sprintf_s(strInfo, "�������%d", goldnum);
	InfoText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::YELLOW);

	rect.top = 360.0f;
	rect.left = 1230.0f;
	sprintf_s(strInfo, "�ӵ�����%d", bulletnum);
	InfoText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::CYAN);

	rect.top = 560.0f;
	rect.left = 1230.0f;
	sprintf_s(strInfo, "��������%d", missilenum);
	InfoText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::RED);

	rect.top = 50.0f;
	rect.left = 200.0f;
	sprintf_s(strInfo, "����ֵ: %d      �ٶ�: %.3f km/h      ����ʱ�䣺 %.3f s", HP, speed, time);
	InfoText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::GREEN);

	rect.top = 80.0f;
	rect.left = 200.0f;
	sprintf_s(strInfo, "�ݻٳ�������%d ��", carnum);
	InfoText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::BLUE);

	rect.top = 80.0f;
	rect.left = 600.0f;
	sprintf_s(strInfo, "���߾���: %.3f m", distance);
	InfoText->DrawTextA(NULL, strInfo, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT, d3d::BLUE);
}

//���������
void CleanupTexture()
{
	d3d::Release<ID3DXMesh*>(Pillar);
	d3d::Release<ID3DXMesh*>(CubeMesh);
	d3d::Release<IDirect3DTexture9*>(TreeTex);
	d3d::Release<IDirect3DTexture9*>(GunTex);
	
	for (int i = 0; i < 3; i++)
		d3d::Release<IDirect3DTexture9*>(BasicSenceTex[i]);

	for (int i = 0; i < 25; i++)
		d3d::Release<IDirect3DTexture9*>(BoomTex[i]);

	for (int i = 0; i < 5; i++)
		d3d::Release<IDirect3DTexture9*>(CubeTex[i]);
}