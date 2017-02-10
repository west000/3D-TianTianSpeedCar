#ifndef __pSystemH__
#define __pSystemH__

#include "D3DUtility.h"
#include "Camera.h"
#include <list>

namespace psys
{
	struct Particle
	{
		D3DXVECTOR3 _position;
		D3DCOLOR    _color;
		static const DWORD FVF;
	};

	struct Attribute
	{
		Attribute()
		{
			_lifeTime = 0.0f;
			_age = 0.0f;
			_isAlive = true;
		}

		D3DXVECTOR3 _position;	   //点精灵的位置
		D3DXVECTOR3 _velocity;     //点精灵的速度
		D3DXVECTOR3 _acceleration; //点精灵的加速度
		float       _lifeTime;     //点精灵距离消亡的时间  
		float       _age;          //点精灵当前的年龄 
		D3DXCOLOR   _color;        //当前点精灵的颜色   
		D3DXCOLOR   _colorFade;    //点精灵颜色随时间的改变
		bool        _isAlive;      //点精灵是否存活
	};


	class PSystem
	{
	public:
		PSystem();
		virtual ~PSystem();

		//初始化点精灵
		virtual bool init(IDirect3DDevice9* device, char* texFileName);
		//重设系统中每个粒子的属性
		virtual void reset();

		//重置点精灵
		virtual void resetParticle(Attribute* attribute) = 0;
		//添加点精灵
		virtual void addParticle();
		//更新点精灵
		virtual void update(float timeDelta) = 0;
		
		//渲染前：设置渲染状态
		virtual void preRender();
		//渲染时
		virtual void render();
		//渲染后
		virtual void postRender();

		bool isEmpty();
		bool isDead();

	protected:
		virtual void removeDeadParticles();  //移除死亡的点精灵

	protected:
		IDirect3DDevice9*       _device;
		D3DXVECTOR3             _origin;     //产生点精灵的源头
		d3d::BoundingBox        _boundingBox;//点精灵的范围
		float                   _emitRate;   //加入新点精灵的频率
		float                   _size;       //点精灵的尺寸
		IDirect3DTexture9*      _tex;        //点精灵的纹理
		IDirect3DVertexBuffer9* _vb;         //点精灵的顶点缓存区
		std::list<Attribute>    _particles;  //点精灵的属性列表
		int                     _maxParticles; //点精灵的最大数目


		DWORD _vbSize;      // 顶点缓存区的大小
		DWORD _vbOffset;    // 顶点缓存的偏移量  
		DWORD _vbBatchSize; // 要锁定从 _vbOffset 开始的顶点数目
		
	};

	//雪花系统
	class Snow : public PSystem
	{
	public:
		Snow(d3d::BoundingBox* boundingBox, int numParticles);
		void resetParticle(Attribute* attribute);
		void update(float timeDelta);
	};

	//烟花系统
	class Firework : public PSystem
	{
	public:
		Firework(D3DXVECTOR3* origin, int numParticles);
		void resetParticle(Attribute* attribute);
		void update(float timeDelta);
		void preRender();
		void postRender();
	};
}

#endif // __pSystemH__