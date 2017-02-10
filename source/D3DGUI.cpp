#include<d3d9.h>
#include<d3dx9.h>
#include"D3DGUI.h"



//-----------------------------------------------------------------------------
// Desc: ���캯��
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
// Name�� D3DGUIClass::CreateTextFont( )
// Desc:	 ���崴������
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


	// ����D3DXCreateTextFont������������
	D3DXCreateFont(d3dDevice, size, 0, 0, 1000, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontName,
		&pFonts[totalFontNum]);

	if (!pFonts[totalFontNum]) return false;

	// ��������ID��ID������
	if (fontID) *fontID = totalFontNum;
	totalFontNum++;

	return true;
}


//-----------------------------------------------------------------------------
// Name�� D3DGUIClass::AddBackground( )
// Desc:	�������汳��ͼ�Ļ��溯��
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddBackground(char *fileName)
{
	if (!fileName) return false;

	Background._type = UGP_GUI_Background;

	//���ļ���������ͼƬ 
	if (D3DXCreateTextureFromFile(d3dDevice, fileName, &Background.Background) != D3D_OK)
		return false;

	// ��ȡ���ڿ�Ⱥ͸߶ȣ��Ա��Ժ�ı�����������
	float w = (float)windowWidth;
	float h = (float)windowHeight;

	GUIVERTEX obj[] =
	{
		{ w, 0, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0.0f },
		{ w, h, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 1.0f },
		{ 0, 0, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0.0f },
		{ 0, h, 0.0f, 1, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 1.0f },
	};

	// �������㻺��
	if (FAILED(d3dDevice->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_DEFAULT, &BackgroundBuffer, NULL))) return false;

	// ��䶥�㻺��
	void *ptr;
	if (FAILED(BackgroundBuffer->Lock(0, sizeof(obj), (void**)&ptr, 0))) return false;
	memcpy(ptr, obj, sizeof(obj));
	BackgroundBuffer->Unlock();

	isBackgroundUsed = true;

	return true;
}


//-----------------------------------------------------------------------------
// Name�� D3DGUIClass::AddStaticText( )
// Desc:	������̬�ı��ؼ�
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddStaticText(int id, char *text, float x, float y, unsigned long color, int fontID)
{
	if (!text || fontID < 0 || fontID >= totalFontNum) return false;

	//����Ĵ������ڴ���һ���ؼ�
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

	// ���������Ҫ�ľ�̬�ı���Ϣ
	pControls[totalControlNum]._type = UGP_GUI_STATICTEXT;
	pControls[totalControlNum]._id = id;
	pControls[totalControlNum]._color = color;
	pControls[totalControlNum]._xPos = x;
	pControls[totalControlNum]._yPos = y;
	pControls[totalControlNum]._listID = fontID;

	// �����ı����� 
	int len = strlen(text);
	pControls[totalControlNum]._text = new char[len + 1];
	if (!pControls[totalControlNum]._text) return false;
	strcpy(pControls[totalControlNum]._text, text);
	//m_pControls[m_nTotalControlNum].m_text[len] = '\0';

	//���������ļ���
	totalControlNum++;

	return true;
}


//-----------------------------------------------------------------------------
// Name�� D3DGUIClass::AddButton( )
// Desc:	������ť�ؼ��ĺ���
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

	// ���ļ���������
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

	//��ȡһ��ͼ�εĿ�Ⱥ͸߶� 
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

	//�������㻺��
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

	//��䶥�㻺��
	void *ptr;

	if (FAILED(pVertexBuffer[totalBufferNum]->Lock(0, sizeof(obj), (void**)&ptr, 0)))
	{
		return false;
	}

	memcpy(ptr, obj, sizeof(obj));
	pVertexBuffer[totalBufferNum]->Unlock();

	// ����
	totalBufferNum++;

	// ����
	totalControlNum++;

	return true;
}

//-----------------------------------------------------------------------------
// Name�� D3DGUIClass::ClearUp( )
// Desc:	�����Ż��ͷ���Դ�ĺ���
//-----------------------------------------------------------------------------
void D3DGUIClass::ClearUp()
{
	//�����Ż����ͷ����е���Դ
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
// Name��ProcessGUI
// Desc: ȫ�ֵĺ�������װ��Ⱦ����GUIϵͳ��ͬ����Ϊ�ؼ����ûص�����
//-----------------------------------------------------------------------------
void ProcessGUI(D3DGUIClass *gui, bool LMBDown, int mouseX, int mouseY,int _currentGUI, void(*funcPtr)(int id, int state,int currentGUI))
{
	if (!gui) return;

	LPDIRECT3DDEVICE9 device = gui->GetD3dDevice();
	if (!device) return;

	// ���Ʊ���
	GUICONTROL *Background = gui->GetBackground();
	LPDIRECT3DVERTEXBUFFER9 bdBuffer = gui->GetBackgroundBuffer();

	//�Ѿ��������Ķ����ſ��Ի���
	if (gui->IsBackgroundUsed() && Background && bdBuffer)
	{
		device->SetTexture(0, Background->Background);
		device->SetStreamSource(0, bdBuffer, 0, sizeof(GUIVERTEX));
		device->SetFVF(D3DFVF_GUI);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		device->SetTexture(0, NULL);
	}

	//������ʾ�ı��Ķ���
	LPD3DXFONT pFont = NULL;
	RECT fontPosition = { 0, 0, (long)gui->GetWindowWidth(),
		(long)gui->GetWindowHeight() };

	// ����һ�����㻺��������ڰ�ť����Ⱦ
	LPDIRECT3DVERTEXBUFFER9 pBuffer = NULL;
	int status = UGP_BUTTON_UP;

	// һ��ѭ�������ڸ��ֿؼ�����Ⱦ
	for (int i = 0; i < gui->GetTotalControlNum(); i++)
	{
		// ��ȡ��ǰ�ؼ�
		GUICONTROL *pControl = gui->GetGUIControl(i);
		if (!pControl) continue;

		// ���ݲ�ͬ����������ͬ�Ĳ���
		switch (pControl->_type)
		{
		case UGP_GUI_STATICTEXT:
			// ��������»�ȡ�������
			pFont = gui->GetFont(pControl->_listID);
			if (!pFont) continue;

			// ��������λ��
			fontPosition.left = pControl->_xPos;
			fontPosition.top = pControl->_yPos;

			// ��ʾ����
			pFont->DrawText(NULL, pControl->_text, -1, &fontPosition,
				DT_LEFT, pControl->_color);
			break;

		case UGP_GUI_BUTTON:
			status = UGP_BUTTON_UP;

			//��ȡ��ť����Ӧ�Ķ��㻺�����
			pBuffer = gui->GetVertexBuffer(pControl->_listID);
			if (!pBuffer) continue;

			// ���������alpha͸��ѡ��
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//�������Ƿ���ͣ���ߵ���˰�ť
			if (mouseX > pControl->_xPos && mouseX < pControl->_xPos + pControl->_width &&
				mouseY > pControl->_yPos && mouseY < pControl->_yPos + pControl->_height)
			{
				if (LMBDown) status = UGP_BUTTON_DOWN;
				else status = UGP_BUTTON_OVER;
			}

			//���ݲ�ͬ�����Ͱ�ť֮���״̬��׼����ͬ������ͼ 
			if (status == UGP_BUTTON_UP) device->SetTexture(0, pControl->upTex);
			if (status == UGP_BUTTON_OVER) device->SetTexture(0, pControl->overTex);
			if (status == UGP_BUTTON_DOWN) device->SetTexture(0, pControl->downTex);

			// ��ʼ��Ⱦ��ť
			device->SetStreamSource(0, pBuffer, 0, sizeof(GUIVERTEX));
			device->SetFVF(D3DFVF_GUI);
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// �ر�alpha���.
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;
		}

		//���ûص���������ؼ���Ϣ
		if (funcPtr) funcPtr(pControl->_id, status,_currentGUI);
	}
}