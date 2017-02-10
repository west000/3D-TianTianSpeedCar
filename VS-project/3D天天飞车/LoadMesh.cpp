#include"LoadAndSetMesh.h"
#include"D3DUtility.h"
#include<vector>

using namespace std;

//------------------------------------------------------------------
//网格模型
//------------------------------------------------------------------
//汽车网格模型
ID3DXMesh* CarMesh = 0;
vector<D3DMATERIAL9> CarMtrl(0);
vector<IDirect3DTexture9*> CarTex(0);
//轮子网格模型
ID3DXMesh* WheelMesh = 0;
vector<D3DMATERIAL9> WheelMtrl(0);
vector<IDirect3DTexture9*> WheelTex(0);
//子弹网格模型
ID3DXMesh* BulletMesh = 0;
std::vector<D3DMATERIAL9> BulletMtrl(0);
std::vector<IDirect3DTexture9*> BulletTex(0);



//
//加载网格模型
//
bool LoadMeshModel(IDirect3DDevice9* device,char* filename,
	ID3DXMesh** mesh,vector<D3DMATERIAL9> &vecMtrl,vector<IDirect3DTexture9*> &vecTex)
{
	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;//ID3DXBuffer是一个COM对象，使用完之后必须将其释放，防止内存泄露
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	//D3DXLoadMeshFromX（）函数返回一个HRESULT类型的数据，可用FAILED（）判断创建网格是否成功
	hr = D3DXLoadMeshFromX(
		filename,	//XFile文件名
		D3DXMESH_MANAGED,	//创建标志，见SDK
		device,
		&adjBuffer,  //存放网格邻接信息的数组
		&mtrlBuffer,//存放网格材质的数组
		0,
		&numMtrls,//材质的数目
		mesh);	//返回已经填充数据的网格

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	//加载材质与纹理
	//
	//如果.X文件里面包含了材质的数据
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		//将其存放在一个材质数组中，以便绘制的时候可以根据不同的面片设置材质
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
				//如果没有纹理图片文件的路径，就将材质设置为0
				vecTex.push_back(0);
			}
		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer//因为ID3DXBuffer是一个COM对象，所以用完必须释放其内存

	//
	//优化网格，使渲染效率提高，主要通过属性表和邻接信息
	//
	hr = (*mesh)->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),	//获取未经优化的邻接数组内存块的指针
		0, 0, 0);	//第三个参数是一个数组，填充了经过优化后的网格的邻接信息
	//第四个参数是一个数组，填充网格面片的重绘信息，这里我们没有用到重绘信息后的数据，设置为0

	d3d::Release<ID3DXBuffer*>(adjBuffer); //同样需要释放其内存

	if (FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}

	return true;
}

//
//重载加载网格模型函数
//
bool LoadAllModel(IDirect3DDevice9* device)
{
	//加载三个网格模型：车子、轮子、子弹
	LoadMeshModel(device, "XFile\\car.x", &CarMesh, CarMtrl, CarTex);
	LoadMeshModel(device, "XFile\\wheel.x", &WheelMesh, WheelMtrl, WheelTex);
	LoadMeshModel(device, "XFile\\bullet.x", &BulletMesh, BulletMtrl, BulletTex);

	return true;
}

//
//清除网格的内存
//
void CleanupMesh()
{	
	d3d::Release<ID3DXMesh*>(CarMesh);
	d3d::Release<ID3DXMesh*>(WheelMesh);
	d3d::Release<ID3DXMesh*>(BulletMesh);
}

//
//渲染车辆模型
//
void RenderCar(IDirect3DDevice9* device,float timeDelta, D3DXMATRIX trans,bool glassy)
{
	device->SetTransform(D3DTS_WORLD, &trans);

	//汽车模型
	//glassy是标记是否透明，吃到穿透盒子就触发该设置
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
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //线框的渲染方式
			CarMesh->DrawSubset(i);
		}
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //画完网格线之后，重新设置为填充渲染方式
	}
		
	//--------------------------------------------------
	//轮子旋转
	//---------------------------------------------------
	D3DXMATRIX RollingX;
	static float rollangle = 0.0f;
	rollangle += timeDelta * 30;
	D3DXMatrixRotationX(&RollingX, rollangle);  //旋转轮子

	if (rollangle > 2 * D3DX_PI)
		rollangle = 0.0f;

	D3DXMATRIX moving;  //将车轮模型移动到合适的位置
	D3DXMatrixTranslation(&moving, -19.4f, 8.3f, 18.5f);

	//右后轮
	D3DXMATRIX result;
	result = RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}

	D3DXMATRIX fanzhuang;     //车轮翻转180°
	D3DXMatrixRotationY(&fanzhuang, D3DX_PI);

	//左后轮
	D3DXMatrixTranslation(&moving, 21.1f, 8.3f, 18.5f);
	result = fanzhuang*RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}

	//右前轮
	D3DXMatrixTranslation(&moving, -19.4f, 8.3f, -51.3f);
	result = RollingX * moving*trans;
	device->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < WheelMtrl.size(); ++i)
	{
		device->SetMaterial(&WheelMtrl[3]);
		device->SetTexture(0, WheelTex[3]);
		WheelMesh->DrawSubset(i);
	}

	//左前轮
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
//渲染子弹、导弹模型
//
//pos是获取当前子弹位置
//子弹的缩放倍数是0.01，而导弹的缩放倍数是0.1
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
//设置各个模型的外接体
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

	//子弹的外接体
	bullet_box->vMin = temp.vMin*0.01f;
	bullet_box->vMax = temp.vMax*0.01f;
	//导弹的外接体
	missile_box->vMin = temp.vMin*0.1f;
	missile_box->vMax = temp.vMax*0.1f;

	return true;
}


//
//碰撞测试
//
bool CollisionTest(D3DXVECTOR3 vMin1, D3DXVECTOR3 vMax1, D3DXVECTOR3 vMin2, D3DXVECTOR3 vMax2)
{
	if (vMax1.x < vMin2.x || vMin1.x > vMax2.x) return false;   // x方向
	if (vMax1.y < vMin2.y || vMin1.y > vMax2.y) return false;   // y方向
	if (vMax1.z < vMin2.z || vMin1.z > vMax2.z) return false;   // z方向
	return true;    // 产生碰撞
}