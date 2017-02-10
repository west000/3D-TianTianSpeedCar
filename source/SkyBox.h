#ifndef __SkyBoxH__
#define __SkyBoxH__

#include "D3DUtility.h"

struct SkyBoxVertex
{
	float x, y, z;
	float u, v;
};
#define D3DFVF_SKYBOX D3DFVF_XYZ|D3DFVF_TEX1

class SkyBoxClass
{
private:
	IDirect3DDevice9 *device;
	IDirect3DVertexBuffer9 *skyBoxVertex;  //天空盒的顶点缓存
	IDirect3DTexture9 *skyTex[5];          //天空盒的纹理贴图
	float length;                          //天空盒的边长

public:
	SkyBoxClass(IDirect3DDevice9* pDirect);
	virtual ~SkyBoxClass();

public:
	bool InitSkyBox(float len);
	bool LoadShyTextureFromFile(char *front, char *back, char *left, char *right, char *top);
	void RenderSkyBox(D3DXMATRIX *matWorld);

};

#endif //__SkyBoxH__