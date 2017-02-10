#include<d3d9.h>
#include<d3dx9.h>
#include"D3DGUI.h"



//-----------------------------------------------------------------------------
// Desc: 构造函数
//-----------------------------------------------------------------------------
D3DGUIClass::D3DGUIClass(LPDIRECT3DDEVICE9 device, int w, int h)
{
	pFonts = NULL;
	pControls = NULL;
	pVertexBuffer = NULL;

	totalFontNum = totalControlNum = totalBufferNum = 0;
	windowWidth = windowHeight = 0;
	isBackgroundUsed = false;

	d3dDevice = device;
	windowWidth = w; windowHeight = h;

	memset(&Background, 0, sizeof(GUICONTROL));
}



//-----------------------------------------------------------------------------
// Name： D3DGUIClass::CreateTextFont( )
// Desc:	 字体创建函数
//-----------------------------------------------------------------------------
bool D3DGUIClass::CreateTextFont(char *fontName, int size, int *fontID)
{
	if (!d3dDevice) return false;

	if (!pFonts)
	{
		pFonts = new LPD3DXFONT[1];
		if (!pFonts) return false;
	}
	else
	{
		LPD3DXFONT *temp;
		temp = new LPD3DXFONT[totalFontNum + 1];
		if (!temp) return false;

		memcpy(temp, pFonts,
			sizeof(LPD3DXFONT)* totalFontNum);

		delete[] pFonts;
		pFonts = temp;
	}


	// 调用D3DXCreateTextFont函数创建字体
	D3DXCreateFont(d3dDevice, size, 0, 0, 1000, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontName,
		&pFonts[totalFontNum]);

	if (!pFonts[totalFontNum]) return false;

	// 存下字体ID和ID计数。
	if (fontID) *fontID = totalFontNum;
	totalFontNum++;

	return true;
}


//-----------------------------------------------------------------------------
// Name： D3DGUIClass::AddBackground( )
// Desc:	创建保存背景图的缓存函数
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddBackground(char *fileName)
{
	if (!fileName) return false;

	Background._type = UGP_GUI_Background;

	//从文件载入纹理图片 
	if (D3DXCreateTextureFromFile(d3dDevice, fileName, &Background.Background) != D3D_OK)
		return false;

	// 获取窗口宽度和高度，以便稍后的背景铺满窗口
	float w = (float)windowWidth;
	float h = (float)windowHeight;

	GUIVERTEX obj[] =
	{
		{ w, 0, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },
		{ w, h, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },
		{ 0, 0, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },
		{ 0, h, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f },
	};

	// 创建顶点缓存
	if (FAILED(d3dDevice->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_DEFAULT, &BackgroundBuffer, NULL))) return false;

	// 填充顶点缓存
	void *ptr;
	if (FAILED(BackgroundBuffer->Lock(0, sizeof(obj), (void**)&ptr, 0))) return false;
	memcpy(ptr, obj, sizeof(obj));
	BackgroundBuffer->Unlock();

	isBackgroundUsed = true;

	return true;
}


//-----------------------------------------------------------------------------
// Name： D3DGUIClass::AddStaticText( )
// Desc:	创建静态文本控件
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddStaticText(int id, char *text, float x, float y, unsigned long color, int fontID)
{
	if (!text || fontID < 0 || fontID >= totalFontNum) return false;

	//下面的代码用于创建一个控件
	if (!pControls)
	{
		pControls = new GUICONTROL[1];
		if (!pControls) return false;
		memset(&pControls[0], 0, sizeof(GUICONTROL));
	}
	else
	{
		GUICONTROL *temp;
		temp = new GUICONTROL[totalControlNum + 1];
		if (!temp) return false;
		memset(temp, 0, sizeof(GUICONTROL)*
			(totalControlNum + 1));

		memcpy(temp,  pControls,
			sizeof(GUICONTROL)* totalControlNum);

		delete[] pControls;
		pControls = temp;
	}

	// 填充我们需要的静态文本信息
	pControls[totalControlNum]._type = UGP_GUI_STATICTEXT;
	pControls[totalControlNum]._id = id;
	pControls[totalControlNum]._color = color;
	pControls[totalControlNum]._xPos = x;
	pControls[totalControlNum]._yPos = y;
	pControls[totalControlNum]._listID = fontID;

	// 复制文本数据 
	int len = strlen(text);
	pControls[totalControlNum]._text = new char[len + 1];
	if (!pControls[totalControlNum]._text) return false;
	strcpy(pControls[totalControlNum]._text, text);
	//m_pControls[m_nTotalControlNum].m_text[len] = '\0';

	//增量总数的计算
	totalControlNum++;

	return true;
}


//-----------------------------------------------------------------------------
// Name： D3DGUIClass::AddButton( )
// Desc:	创建按钮控件的函数
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddButton(int id, float x, float y, char *up, char *over, char *down)
{
	if (!up || !over || !down) return false;

	if (!pControls)
	{
		pControls = new GUICONTROL[1];
		if (!pControls) return false;
		memset(&pControls[0], 0, sizeof(GUICONTROL));
	}
	else
	{
		GUICONTROL *temp;
		temp = new GUICONTROL[totalControlNum + 1];
		if (!temp) return false;
		memset(temp, 0, sizeof(GUICONTROL)*
			(totalControlNum + 1));

		memcpy(temp, pControls,
			sizeof(GUICONTROL)* totalControlNum);

		delete[] pControls;
		pControls = temp;
	}

	// Set all the data needed to render/process a button.
	pControls[totalControlNum]._type = UGP_GUI_BUTTON;
	pControls[totalControlNum]._id = id;
	pControls[totalControlNum]._xPos = x;
	pControls[totalControlNum]._yPos = y;
	pControls[totalControlNum]._listID = totalBufferNum;

	// 从文件加载纹理
	if (D3DXCreateTextureFromFile(d3dDevice, up, &pControls[totalControlNum].upTex) != D3D_OK)
	{
		return false;
	}

	if (D3DXCreateTextureFromFile(d3dDevice, over, &pControls[totalControlNum].overTex) != D3D_OK)
	{
		return false;
	}

	if (D3DXCreateTextureFromFile(d3dDevice, down, &pControls[totalControlNum].downTex) != D3D_OK)
	{
		return false;
	}

	unsigned long white = D3DCOLOR_XRGB(255, 255, 255);

	//获取一下图形的宽度和高度 
	D3DSURFACE_DESC desc;
	pControls[totalControlNum].upTex->GetLevelDesc(0, &desc);

	float w = (float)desc.Width;
	float h = (float)desc.Height;

	pControls[totalControlNum]._width = w;
	pControls[totalControlNum]._height = h;

	GUIVERTEX obj[] =
	{
		{ w + x, 0 + y, 0.0f, 1, white, 1.0f, 0.0f },
		{ w + x, h + y, 0.0f, 1, white, 1.0f, 1.0f },
		{ 0 + x, 0 + y, 0.0f, 1, white, 0.0f, 0.0f },
		{ 0 + x, h + y, 0.0f, 1, white, 0.0f, 1.0f },
	};

	//创建顶点缓存
	if (!pVertexBuffer)
	{
		pVertexBuffer = new LPDIRECT3DVERTEXBUFFER9[1];
		if (!pVertexBuffer) return false;
	}
	else
	{
		LPDIRECT3DVERTEXBUFFER9 *temp;
		temp = new LPDIRECT3DVERTEXBUFFER9[totalBufferNum + 1];
		if (!temp) return false;

		memcpy(temp, pVertexBuffer,
			sizeof(LPDIRECT3DVERTEXBUFFER9)* totalBufferNum);

		delete[] pVertexBuffer;
		pVertexBuffer = temp;
	}

	if (FAILED(d3dDevice->CreateVertexBuffer(sizeof(obj), 0,
		D3DFVF_GUI, D3DPOOL_DEFAULT, &pVertexBuffer[totalBufferNum], NULL)))
	{
		return false;
	}

	//填充顶点缓存
	void *ptr;

	if (FAILED(pVertexBuffer[totalBufferNum]->Lock(0, sizeof(obj), (void**)&ptr, 0)))
	{
		return false;
	}

	memcpy(ptr, obj, sizeof(obj));
	pVertexBuffer[totalBufferNum]->Unlock();

	// 自增
	totalBufferNum++;

	// 自增
	totalControlNum++;

	return true;
}

//-----------------------------------------------------------------------------
// Name： D3DGUIClass::ClearUp( )
// Desc:	清理门户释放资源的函数
//-----------------------------------------------------------------------------
void D3DGUIClass::ClearUp()
{
	//清理门户，释放所有的资源
	if (isBackgroundUsed)
	{
		if (Background.Background)
			Background.Background->Release();

		if (BackgroundBuffer)
			BackgroundBuffer->Release();
	}

	Background.Background = NULL;
	BackgroundBuffer = NULL;

	for (int i = 0; i < totalFontNum; i++)
	{
		if (pFonts[i])
		{
			pFonts[i]->Release();
			pFonts[i] = NULL;
		}
	}

	if (pFonts) delete[] pFonts;
	pFonts = NULL;
	totalFontNum = 0;

	for (int i = 0; i < totalBufferNum; i++)
	{
		if (pVertexBuffer[i])
		{
			pVertexBuffer[i]->Release();
			pVertexBuffer[i] = NULL;
		}
	}

	if (pVertexBuffer) delete[] pVertexBuffer;
	pVertexBuffer = NULL;
	totalBufferNum = 0;

	for (int i = 0; i < totalControlNum; i++)
	{
		if (pControls[i].Background)
		{
			pControls[i].Background->Release();
			pControls[i].Background = NULL;
		}

		if (pControls[i].upTex)
		{
			pControls[i].upTex->Release();
			pControls[i].upTex = NULL;
		}

		if (pControls[i].downTex)
		{
			pControls[i].downTex->Release();
			pControls[i].downTex = NULL;
		}

		if (pControls[i].overTex)
		{
			pControls[i].overTex->Release();
			pControls[i].overTex = NULL;
		}

		if (pControls[i]._text)
		{
			delete[] pControls[i]._text;
			pControls[i]._text = NULL;
		}
	}

	if (pControls) delete[] pControls;
	pControls = NULL;
	totalControlNum = 0;
}


//-----------------------------------------------------------------------------
// Name：ProcessGUI
// Desc: 全局的函数，封装渲染整个GUI系统，同样还为控件调用回调函数
//-----------------------------------------------------------------------------
void ProcessGUI(D3DGUIClass *gui, bool LMBDown, int mouseX, int mouseY,int _currentGUI, void(*funcPtr)(int id, int state,int currentGUI))
{
	if (!gui) return;

	LPDIRECT3DDEVICE9 device = gui->GetD3dDevice();
	if (!device) return;

	// 绘制背景
	GUICONTROL *Background = gui->GetBackground();
	LPDIRECT3DVERTEXBUFFER9 bdBuffer = gui->GetBackgroundBuffer();

	//已经创建出的东西才可以绘制
	if (gui->IsBackgroundUsed() && Background && bdBuffer)
	{
		device->SetTexture(0, Background->Background);
		device->SetStreamSource(0, bdBuffer, 0, sizeof(GUIVERTEX));
		device->SetFVF(D3DFVF_GUI);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		device->SetTexture(0, NULL);
	}

	//用来显示文本的对象
	LPD3DXFONT pFont = NULL;
	RECT fontPosition = { 0, 0, (long)gui->GetWindowWidth(),
		(long)gui->GetWindowHeight() };

	// 创建一个顶点缓存对象用于按钮的渲染
	LPDIRECT3DVERTEXBUFFER9 pBuffer = NULL;
	int status = UGP_BUTTON_UP;

	// 一个循环，用于各种控件的渲染
	for (int i = 0; i < gui->GetTotalControlNum(); i++)
	{
		// 获取当前控件
		GUICONTROL *pControl = gui->GetGUIControl(i);
		if (!pControl) continue;

		// 根据不同的类型做不同的操作
		switch (pControl->_type)
		{
		case UGP_GUI_STATICTEXT:
			// 这种情况下获取字体对象
			pFont = gui->GetFont(pControl->_listID);
			if (!pFont) continue;

			// 设置文字位置
			fontPosition.left = pControl->_xPos;
			fontPosition.top = pControl->_yPos;

			// 显示文字
			pFont->DrawText(NULL, pControl->_text, -1, &fontPosition,
				DT_LEFT, pControl->_color);
			break;

		case UGP_GUI_BUTTON:
			status = UGP_BUTTON_UP;

			//获取按钮所对应的顶点缓存对象
			pBuffer = gui->GetVertexBuffer(pControl->_listID);
			if (!pBuffer) continue;

			// 设置纹理的alpha透明选项
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//检查鼠标是否悬停或者点击了按钮
			if (mouseX > pControl->_xPos && mouseX < pControl->_xPos + pControl->_width &&
				mouseY > pControl->_yPos && mouseY < pControl->_yPos + pControl->_height)
			{
				if (LMBDown) status = UGP_BUTTON_DOWN;
				else status = UGP_BUTTON_OVER;
			}

			//根据不同的鼠标和按钮之间的状态来准备不同的纹理图 
			if (status == UGP_BUTTON_UP) device->SetTexture(0, pControl->upTex);
			if (status == UGP_BUTTON_OVER) device->SetTexture(0, pControl->overTex);
			if (status == UGP_BUTTON_DOWN) device->SetTexture(0, pControl->downTex);

			// 开始渲染按钮
			device->SetStreamSource(0, pBuffer, 0, sizeof(GUIVERTEX));
			device->SetFVF(D3DFVF_GUI);
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// 关闭alpha混合.
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;
		}

		//调用回调函数处理控件消息
		if (funcPtr) funcPtr(pControl->_id, status,_currentGUI);
	}
}