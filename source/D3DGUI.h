#ifndef __D3DGUIH__
#define __D3DGUIH__

#include <d3dx9.h>

// ��֧�ֵĿؼ����ͺ�
#define UGP_GUI_STATICTEXT		1
#define UGP_GUI_BUTTON			2
#define UGP_GUI_Background		3

// ��갴��״̬��
#define UGP_BUTTON_UP			1
#define UGP_BUTTON_OVER		    2
#define UGP_BUTTON_DOWN		    3


// �˵�ҳ��ĺ궨��
#define GUI_MAIN_SCREEN         1
#define GUI_START_SCREEN        2
#define GUI_LOAD_SCREEN		 	3
#define GUI_OPTION_SCREEN       4
#define GUI_ABOUT_SCREEN        5
#define GUI_HELP_SCREEN         6
#define GUI_CONTINUE_SCREEN     7

// ����һЩGUI���õ��Ŀؼ�ID
#define STATIC_TEXT_ID			1
#define BUTTON_START_ID         2
#define BUTTON_LOAD_ID          3
#define BUTTON_OPTION_ID        4
#define BUTTON_QUIT_ID          5
#define BUTTON_CONTINUE_ID      6
#define BUTTION_HELP_ID         7
#define BUTTON_BACK_ID          8
#define BUTTON_ABOUT_ID         9



// FVF�������͵Ľṹ��
struct GUIVERTEX
{
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//�ؼ����Խṹ��
struct GUICONTROL
{
	//�������ͣ�ID����ɫ
	int _type;              //�ؼ�����
	int _id;                //�ؼ�ID
	unsigned long _color;   //�ؼ���ɫ
	int _listID;            //��������ֵĻ�����������ͱ�ʾ��ʹ�õ����壬����ͱ�ʾ���㻺��
	float _xPos, _yPos;     //�ؼ�����ʼλ��
	float _width, _height;  // �ؼ��Ŀ�Ⱥ͸߶�
	char *_text;            // ��������
	LPDIRECT3DTEXTURE9 Background;                // �ؼ����������ͼ��
	LPDIRECT3DTEXTURE9 upTex, downTex, overTex;   // ��Ű�ť���𣬰��º���꾭��ʱ��3������ͼ
};



class D3DGUIClass
{

private:
	LPDIRECT3DDEVICE9 d3dDevice;             //D3D�豸����	
	LPD3DXFONT *pFonts;                      //D3D�������
	GUICONTROL *pControls;                   //�ؼ�����
	LPDIRECT3DVERTEXBUFFER9 *pVertexBuffer;  //���㻺�����ָ��
	GUICONTROL Background;                   //����ͼ����
	LPDIRECT3DVERTEXBUFFER9 BackgroundBuffer;//����ͼ����������

	bool isBackgroundUsed;    //һ����ʶ�����ڱ�ʶ�Ƿ��Ѿ����˱���
	int totalFontNum;		  //������Ŀ������
	int totalControlNum;	  //�ؼ���Ŀ������
	int totalBufferNum;	      //��������Ŀ������

	int windowWidth;	     //���ڿ��
	int windowHeight;	     //���ڸ߶�



public:
	D3DGUIClass(LPDIRECT3DDEVICE9 device, int w, int h);
	~D3DGUIClass() { ClearUp(); }

	LPDIRECT3DDEVICE9 GetD3dDevice() { return d3dDevice; }                       //����D3D�豸����ĺ���
	GUICONTROL *GetBackground() { return &Background; }                          //���ر����ĺ���
	LPDIRECT3DVERTEXBUFFER9 GetBackgroundBuffer() { return BackgroundBuffer; }   //���ر�������������ĺ���


	int GetTotalFontNum() { return totalFontNum; }        //��������������Ŀ�ĺ���
	int GetTotalControlNum() { return totalControlNum; }  //�������пؼ���Ŀ�ĺ���
	int GetTotalBufferNum() { return totalBufferNum; }    //�����ܵĻ�������Ŀ�ĺ���
	int GetWindowWidth() { return windowWidth; }          //���ش��ڿ�ȵĺ���
	int GetWindowHeight() { return windowHeight; }        //���ش��ڸ߶ȵĺ���
	bool IsBackgroundUsed() { return isBackgroundUsed; }  //���ر����Ƿ���ʹ�õ�boolֵ�ĺ���
	void SetWindowSize(int w, int h) { windowWidth = w; windowHeight = h; }  //���ô��ڿ�Ⱥ͸߶ȵĺ���

	LPD3DXFONT GetFont(int id)  //��������ID����
	{
		if (id < 0 || id >= totalFontNum) return NULL;
		return pFonts[id];
	}

	GUICONTROL *GetGUIControl(int id)  //����GUI�ؼ�ID����
	{
		if (id < 0 || id >= totalControlNum) return NULL;
		return &pControls[id];
	}

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(int id) //���ض��㻺��ID����
	{
		if (id < 0 || id >= totalBufferNum) return NULL;
		return pVertexBuffer[id];
	}


	bool CreateTextFont(char *fontName, int size, int *fontID);  //���崴������
	bool AddBackground(char *fileName);  //GUI������Ӻ���
	bool AddStaticText(int id, char *text, float x, float y, unsigned long color, int fontID); //��Ӿ�̬�ı�����
	bool AddButton(int id, float x, float y, char *up, char *over, char *down); //��Ӱ�ť����
	void ClearUp(); //��Դ������


};

void ProcessGUI(D3DGUIClass *gui, bool LMBDown, int mouseX, int mouseY,int _currentGUI,
	void(*funcPtr)(int id, int state,int currentGUI));  //�ص�����

#endif // __D3DGUIH__