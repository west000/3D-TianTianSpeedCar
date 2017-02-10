#include"SkyBox.h"

//
//���캯��
//
SkyBoxClass::SkyBoxClass(IDirect3DDevice9 *pDevice)
{
	skyBoxVertex = NULL;
	device = pDevice;
	for (int i = 0; i < 5; i++)
		skyTex[i] = NULL;
	length = 0.0f;
}

//
//��������
//
SkyBoxClass::~SkyBoxClass()
{
	d3d::Release<IDirect3DVertexBuffer9*>(skyBoxVertex);
	for (int i = 0; i < 5; i++)
	{
		d3d::Release<IDirect3DTexture9*>(skyTex[i]);
	}
}

//
//��ʼ����պ��Ӻ���
//
bool SkyBoxClass::InitSkyBox(float len)
{
	length = len;

	device->CreateVertexBuffer(
		30 * sizeof(SkyBoxVertex),
		0,
		D3DFVF_SKYBOX,
		D3DPOOL_MANAGED,
		&skyBoxVertex,
		0);

	//׼����䶥������
	SkyBoxVertex *pVertices;
	//2������
	skyBoxVertex->Lock(0, 0, (void**)&pVertices, 0);

	//ǰ���ĸ���
	pVertices[0] = SkyBoxVertex{ -length / 2, 0.0f, length / 2, 0.0f, 1.0f };
	pVertices[1] = SkyBoxVertex{ -length / 2, length / 2, length / 2, 0.0f, 0.0f };
	pVertices[2] = SkyBoxVertex{ length / 2, length / 2, length / 2, 1.0f, 0.0f };
	pVertices[3] = SkyBoxVertex{ -length / 2, 0.0f, length / 2, 0.0f, 1.0f };
	pVertices[4] = SkyBoxVertex{ length / 2, length / 2, length / 2, 1.0f, 0.0f };
	pVertices[5] = SkyBoxVertex{ length / 2, 0.0f, length / 2, 1.0f, 1.0f };


	//�����ĸ�����
	pVertices[6] = SkyBoxVertex{ length / 2, 0.0f, -length / 2, 0.0f, 1.0f };
	pVertices[7] = SkyBoxVertex{ length / 2, length / 2, -length / 2, 0.0f, 0.0f };
	pVertices[8] = SkyBoxVertex{ -length / 2, length / 2, -length / 2, 1.0f, 0.0f };
	pVertices[9] = SkyBoxVertex{ length / 2, 0.0f, -length / 2, 0.0f, 1.0f };
	pVertices[10] = SkyBoxVertex{ -length / 2, length / 2, -length / 2, 1.0f, 0.0f };
	pVertices[11] = SkyBoxVertex{ -length / 2, 0.0f, -length / 2, 1.0f, 1.0f };


	//������ĸ�����  
	pVertices[12] = SkyBoxVertex{ -length / 2, 0.0f, -length / 2, 0.0f, 1.0f };
	pVertices[13] = SkyBoxVertex{ -length / 2, length / 2, -length / 2, 0.0f, 0.0f };
	pVertices[14] = SkyBoxVertex{ -length / 2, length / 2, length / 2, 1.0f, 0.0f };
	pVertices[15] = SkyBoxVertex{ -length / 2, 0.0f, -length / 2, 0.0f, 1.0f };
	pVertices[16] = SkyBoxVertex{ -length / 2, length / 2, length / 2, 1.0f, 0.0f };
	pVertices[17] = SkyBoxVertex{ -length / 2, 0.0f, length / 2, 1.0f, 1.0f };


	//������ĸ�����  
	pVertices[18] = SkyBoxVertex{ length / 2, 0.0f, length / 2, 0.0f, 1.0f };
	pVertices[19] = SkyBoxVertex{ length / 2, length / 2, length / 2, 0.0f, 0.0f };
	pVertices[20] = SkyBoxVertex{ length / 2, length / 2, -length / 2, 1.0f, 0.0f };
	pVertices[21] = SkyBoxVertex{ length / 2, 0.0f, length / 2, 0.0f, 1.0f };
	pVertices[22] = SkyBoxVertex{ length / 2, length / 2, -length / 2, 1.0f, 0.0f };
	pVertices[23] = SkyBoxVertex{ length / 2, 0.0f, -length / 2, 1.0f, 1.0f };

	//������ĸ�����  
	pVertices[24] = SkyBoxVertex{ -length / 2, length / 2, length / 2, 0.0f, 1.0f };
	pVertices[25] = SkyBoxVertex{ -length / 2, length / 2, -length / 2, 0.0f, 0.0f };
	pVertices[26] = SkyBoxVertex{ length / 2, length / 2, -length / 2, 1.0f, 0.0f };
	pVertices[27] = SkyBoxVertex{ -length / 2, length / 2, length / 2, 0.0f, 1.0f };
	pVertices[28] = SkyBoxVertex{ length / 2, length / 2, -length / 2, 1.0f, 0.0f };
	pVertices[29] = SkyBoxVertex{ length / 2, length / 2, length / 2, 1.0f, 1.0f };

	//3������
	skyBoxVertex->Unlock();

	//Ҳ����memcpy�����ж��㸴�Ƶ����㻺����
	//memcpy(pVertices,vertices,sizeof(vertices));

	return true;
}

//
//�������������ͼ
//
bool SkyBoxClass::LoadShyTextureFromFile(char *front, char *back, char *left, char *right, char *top)
{
	D3DXCreateTextureFromFile(device, front, &skyTex[0]);
	D3DXCreateTextureFromFile(device, back, &skyTex[1]);
	D3DXCreateTextureFromFile(device, left, &skyTex[2]);
	D3DXCreateTextureFromFile(device, right, &skyTex[3]);
	D3DXCreateTextureFromFile(device, top, &skyTex[4]);
	return true;
}

//
//��պе���Ⱦ
//
void SkyBoxClass::RenderSkyBox(D3DXMATRIX *matWorld)
{
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//��λѰַģʽ
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	device->SetTransform(D3DTS_WORLD, matWorld);
	device->SetStreamSource(0, skyBoxVertex, 0, sizeof(SkyBoxVertex));
	device->SetFVF(D3DFVF_SKYBOX);
	for (int i = 0; i < 5; i++)
	{
		device->SetTexture(0, skyTex[i]);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, i * 6, 2);
	}
}
