#include"LoadAndSetMesh.h"
#include"D3DUtility.h"
#include<vector>

using namespace std;

//------------------------------------------------------------------
//����ģ��
//------------------------------------------------------------------
//��������ģ��
ID3DXMesh* CarMesh = 0;
vector<D3DMATERIAL9> CarMtrl(0);
vector<IDirect3DTexture9*> CarTex(0);
//��������ģ��
ID3DXMesh* WheelMesh = 0;
vector<D3DMATERIAL9> WheelMtrl(0);
vector<IDirect3DTexture9*> WheelTex(0);
//�ӵ�����ģ��
ID3DXMesh* BulletMesh = 0;
std::vector<D3DMATERIAL9> BulletMtrl(0);
std::vector<IDirect3DTexture9*> BulletTex(0);



//
//��������ģ��
//
bool LoadMeshModel(IDirect3DDevice9* device,char* filename,
	ID3DXMesh** mesh,vector<D3DMATERIAL9> &vecMtrl,vector<IDirect3DTexture9*> &vecTex)
{
	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;//ID3DXBuffer��һ��COM����ʹ����֮����뽫���ͷţ���ֹ�ڴ�й¶
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	//D3DXLoadMeshFromX������������һ��HRESULT���͵����ݣ�����FAILED�����жϴ��������Ƿ�ɹ�
	hr = D3DXLoadMeshFromX(
		filename,	//XFile�ļ���
		D3DXMESH_MANAGED,	//������־����SDK
		device,
		&adjBuffer,  //��������ڽ���Ϣ������
		&mtrlBuffer,//���������ʵ�����
		0,
		&numMtrls,//���ʵ���Ŀ
		mesh);	//�����Ѿ�������ݵ�����

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	//���ز���������
	//
	//���.X�ļ���������˲��ʵ�����
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		//��������һ�����������У��Ա���Ƶ�ʱ����Ը��ݲ�ͬ����Ƭ���ò���
		for (int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			vecMtrl.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					device,
					mtrls[i].pTextureFilename,
					&tex);
				vecTex.push_back(tex);
			}
			else  
			{
				//���û������ͼƬ�ļ���·�����ͽ���������Ϊ0
				vecTex.push_back(0);
			}
		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer//��ΪID3DXBuffer��һ��COM����������������ͷ����ڴ�

	//
	//�Ż�����ʹ��ȾЧ����ߣ���Ҫͨ�����Ա���ڽ���Ϣ
	//
	hr = (*mesh)->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),	//��ȡδ���Ż����ڽ������ڴ���ָ��
		0, 0, 0);	//������������һ�����飬����˾����Ż����������ڽ���Ϣ
	//���ĸ�������һ�����飬���������Ƭ���ػ���Ϣ����������û���õ��ػ���Ϣ������ݣ�����Ϊ0

	d3d::Release<ID3DXBuffer*>(adjBuffer); //ͬ����Ҫ�ͷ����ڴ�

	if (FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}

	return true;
}

//
//���ؼ�������ģ�ͺ���
//
bool LoadAllModel(IDirect3DDevice9* device)
{
	//������������ģ�ͣ����ӡ����ӡ��ӵ�
	LoadMeshModel(device, "XFile\\car.x", &CarMesh, CarMtrl, CarTex);
	LoadMeshModel(device, "XFile\\wheel.x", &WheelMesh, WheelMtrl, WheelTex);
	LoadMeshModel(device, "XFile\\bullet.x", &BulletMesh, BulletMtrl, BulletTex);

	return true;
}

//
//���������ڴ�
//
void CleanupMesh()
{	
	d3d::Release<ID3DXMesh*>(CarMesh);
	d3d::Release<ID3DXMesh*>(WheelMesh);
	d3d::Release<ID3DXMesh*>(BulletMesh);
}

//
//��Ⱦ����ģ��
//
void RenderCar(IDirect3DDevice9* device,float timeDelta, D3DXMATRIX trans,bool glassy)
{
	device->SetTransform(D3DTS_WORLD, &trans);

	//����ģ��
	//glassy�Ǳ���Ƿ�͸�����Ե���͸���Ӿʹ���������
	if (!glassy)
		for (int i = 0; i < CarMtrl.size(); i++)
		{
			device->SetMaterial(&CarMtrl[i]);
			device->SetTexture(0, CarTex[i]);
			CarMesh->DrawSubset(i);
		}
	else
	{
		for (int i = 0; i < CarMtrl.size(); i++)
		{
			device->SetMaterial(&d3d::YELLOW_MTRL);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //�߿����Ⱦ��ʽ
			CarMesh->DrawSubset(i);
		}
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //����������֮����������Ϊ�����Ⱦ��ʽ
	}
		
	//--------------------------------------------------
	//������ת
	//---------------------------------------------------
	D3DXMATRIX RollingX;
	static float rollangle = 0.0f;
	rollangle += timeDelta * 30;
	D3DXMatrixRotationX(&RollingX, rollangle);  //��ת����

	if (rollangle > 2 * D3DX_PI)
		rollangle = 0.0f;

	D3DXMATRIX moving;  //������ģ���ƶ������ʵ�λ��
	D3DXMatrixTranslation(&moving, -19.4f, 8.3f, 18.5f);

	//�Һ���
	D3DXMATRIX result;
	result = RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}

	D3DXMATRIX fanzhuang;     //���ַ�ת180��
	D3DXMatrixRotationY(&fanzhuang, D3DX_PI);

	//�����
	D3DXMatrixTranslation(&moving, 21.1f, 8.3f, 18.5f);
	result = fanzhuang*RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}

	//��ǰ��
	D3DXMatrixTranslation(&moving, -19.4f, 8.3f, -51.3f);
	result = RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}

	//��ǰ��
	D3DXMatrixTranslation(&moving, 21.3f, 8.3f, -51.3f);
	result = fanzhuang * RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}
}


//
//��Ⱦ�ӵ�������ģ��
//
//pos�ǻ�ȡ��ǰ�ӵ�λ��
//�ӵ������ű�����0.01�������������ű�����0.1
void RenderBullet(IDirect3DDevice9* device, D3DXMATRIX trans, float scale,D3DXVECTOR3* pos)
{
	D3DXMATRIX moving, scaling, result, Ry;
	D3DXMatrixTranslation(&moving, 0.0f, 0.0f, 8.0f);
	D3DXMatrixScaling(&scaling, scale, scale, scale);
	D3DXMatrixRotationY(&Ry, -D3DX_PI*0.46f);

	result = Ry*scaling*moving*trans;

	device->SetTransform(D3DTS_WORLD, &result);
	pos->x = result._41;
	pos->y = result._42;
	pos->z = result._43;

	for (int i = 0; i < BulletMtrl.size(); i++)
	{
		device->SetMaterial(&d3d::RED_MTRL);
		BulletMesh->DrawSubset(i);
	}
}

//
//���ø���ģ�͵������
//
bool SetBoundingBox(d3d::BOUNDINGBOX *car_box, d3d::BOUNDINGBOX *bullet_box,d3d::BOUNDINGBOX *missile_box)
{
	d3d::BOUNDINGBOX temp;

	D3DXVECTOR3* firstPosition = 0;

	CarMesh->LockVertexBuffer(0, (void**)&firstPosition);
	D3DXComputeBoundingBox(firstPosition, CarMesh->GetNumVertices(), D3DXGetFVFVertexSize(CarMesh->GetFVF()), &(temp.vMin), &(temp.vMax));
	CarMesh->UnlockVertexBuffer();
	car_box->vMin = temp.vMin*0.15f;
	car_box->vMax = temp.vMax*0.15f;

	BulletMesh->LockVertexBuffer(0, (void**)&firstPosition);
	D3DXComputeBoundingBox(firstPosition, BulletMesh->GetNumVertices(), D3DXGetFVFVertexSize(BulletMesh->GetFVF()), &(temp.vMin), &(temp.vMax));
	BulletMesh->UnlockVertexBuffer();

	//�ӵ��������
	bullet_box->vMin = temp.vMin*0.01f;
	bullet_box->vMax = temp.vMax*0.01f;
	//�����������
	missile_box->vMin = temp.vMin*0.1f;
	missile_box->vMax = temp.vMax*0.1f;

	return true;
}


//
//��ײ����
//
bool CollisionTest(D3DXVECTOR3 vMin1, D3DXVECTOR3 vMax1, D3DXVECTOR3 vMin2, D3DXVECTOR3 vMax2)
{
	if (vMax1.x < vMin2.x || vMin1.x > vMax2.x) return false;   // x����
	if (vMax1.y < vMin2.y || vMin1.y > vMax2.y) return false;   // y����
	if (vMax1.z < vMin2.z || vMin1.z > vMax2.z) return false;   // z����
	return true;    // ������ײ
}