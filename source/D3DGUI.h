#ifndef __D3DGUIH__
#define __D3DGUIH__

#include <d3dx9.h>

// 所支持的控件类型宏
#define UGP_GUI_STATICTEXT		1
#define UGP_GUI_BUTTON			2
#define UGP_GUI_Background		3

// 鼠标按键状态宏
#define UGP_BUTTON_UP			1
#define UGP_BUTTON_OVER		    2
#define UGP_BUTTON_DOWN		    3


// 菜单页面的宏定义
#define GUI_MAIN_SCREEN         1
#define GUI_START_SCREEN        2
#define GUI_LOAD_SCREEN		 	3
#define GUI_OPTION_SCREEN       4
#define GUI_ABOUT_SCREEN        5
#define GUI_HELP_SCREEN         6
#define GUI_CONTINUE_SCREEN     7

// 设置一些GUI中用到的控件ID
#define STATIC_TEXT_ID			1
#define BUTTON_START_ID         2
#define BUTTON_LOAD_ID          3
#define BUTTON_OPTION_ID        4
#define BUTTON_QUIT_ID          5
#define BUTTON_CONTINUE_ID      6
#define BUTTION_HELP_ID         7
#define BUTTON_BACK_ID          8
#define BUTTON_ABOUT_ID         9



// FVF灵活顶点类型的结构体
struct GUIVERTEX
{
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//控件属性结构体
struct GUICONTROL
{
	//操作类型，ID和颜色
	int _type;              //控件类型
	int _id;                //控件ID
	unsigned long _color;   //控件颜色
	int _listID;            //如果是文字的话，这个变量就表示它使用的字体，否则就表示顶点缓存
	float _xPos, _yPos;     //控件的起始位置
	float _width, _height;  // 控件的宽度和高度
	char *_text;            // 文字内容
	LPDIRECT3DTEXTURE9 Background;                // 控件背景的填充图像
	LPDIRECT3DTEXTURE9 upTex, downTex, overTex;   // 存放按钮弹起，按下和鼠标经过时的3张纹理图
};



class D3DGUIClass
{

private:
	LPDIRECT3DDEVICE9 d3dDevice;             //D3D设备对象	
	LPD3DXFONT *pFonts;                      //D3D字体对象
	GUICONTROL *pControls;                   //控件对象
	LPDIRECT3DVERTEXBUFFER9 *pVertexBuffer;  //顶点缓存对象指针
	GUICONTROL Background;                   //背景图对象
	LPDIRECT3DVERTEXBUFFER9 BackgroundBuffer;//背景图缓冲区对象

	bool isBackgroundUsed;    //一个标识，用于标识是否已经用了背景
	int totalFontNum;		  //字体数目计数器
	int totalControlNum;	  //控件数目计数器
	int totalBufferNum;	      //缓冲区数目计数器

	int windowWidth;	     //窗口宽度
	int windowHeight;	     //窗口高度



public:
	D3DGUIClass(LPDIRECT3DDEVICE9 device, int w, int h);
	~D3DGUIClass() { ClearUp(); }

	LPDIRECT3DDEVICE9 GetD3dDevice() { return d3dDevice; }                       //返回D3D设备对象的函数
	GUICONTROL *GetBackground() { return &Background; }                          //返回背景的函数
	LPDIRECT3DVERTEXBUFFER9 GetBackgroundBuffer() { return BackgroundBuffer; }   //返回背景缓冲区对象的函数


	int GetTotalFontNum() { return totalFontNum; }        //返回所有字体数目的函数
	int GetTotalControlNum() { return totalControlNum; }  //返回所有控件数目的函数
	int GetTotalBufferNum() { return totalBufferNum; }    //返回总的缓冲区数目的函数
	int GetWindowWidth() { return windowWidth; }          //返回窗口宽度的函数
	int GetWindowHeight() { return windowHeight; }        //返回窗口高度的函数
	bool IsBackgroundUsed() { return isBackgroundUsed; }  //返回背景是否在使用的bool值的函数
	void SetWindowSize(int w, int h) { windowWidth = w; windowHeight = h; }  //设置窗口宽度和高度的函数

	LPD3DXFONT GetFont(int id)  //返回字体ID函数
	{
		if (id < 0 || id >= totalFontNum) return NULL;
		return pFonts[id];
	}

	GUICONTROL *GetGUIControl(int id)  //返回GUI控件ID函数
	{
		if (id < 0 || id >= totalControlNum) return NULL;
		return &pControls[id];
	}

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(int id) //返回顶点缓存ID函数
	{
		if (id < 0 || id >= totalBufferNum) return NULL;
		return pVertexBuffer[id];
	}


	bool CreateTextFont(char *fontName, int size, int *fontID);  //字体创建函数
	bool AddBackground(char *fileName);  //GUI背景添加函数
	bool AddStaticText(int id, char *text, float x, float y, unsigned long color, int fontID); //添加静态文本函数
	bool AddButton(int id, float x, float y, char *up, char *over, char *down); //添加按钮函数
	void ClearUp(); //资源清理函数


};

void ProcessGUI(D3DGUIClass *gui, bool LMBDown, int mouseX, int mouseY,int _currentGUI,
	void(*funcPtr)(int id, int state,int currentGUI));  //回调函数

#endif // __D3DGUIH__