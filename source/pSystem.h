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

		D3DXVECTOR3 _position;	   //�㾫���λ��
		D3DXVECTOR3 _velocity;     //�㾫����ٶ�
		D3DXVECTOR3 _acceleration; //�㾫��ļ��ٶ�
		float       _lifeTime;     //�㾫�����������ʱ��  
		float       _age;          //�㾫�鵱ǰ������ 
		D3DXCOLOR   _color;        //��ǰ�㾫�����ɫ   
		D3DXCOLOR   _colorFade;    //�㾫����ɫ��ʱ��ĸı�
		bool        _isAlive;      //�㾫���Ƿ���
	};


	class PSystem
	{
	public:
		PSystem();
		virtual ~PSystem();

		//��ʼ���㾫��
		virtual bool init(IDirect3DDevice9* device, char* texFileName);
		//����ϵͳ��ÿ�����ӵ�����
		virtual void reset();

		//���õ㾫��
		virtual void resetParticle(Attribute* attribute) = 0;
		//��ӵ㾫��
		virtual void addParticle();
		//���µ㾫��
		virtual void update(float timeDelta) = 0;
		
		//��Ⱦǰ��������Ⱦ״̬
		virtual void preRender();
		//��Ⱦʱ
		virtual void render();
		//��Ⱦ��
		virtual void postRender();

		bool isEmpty();
		bool isDead();

	protected:
		virtual void removeDeadParticles();  //�Ƴ������ĵ㾫��

	protected:
		IDirect3DDevice9*       _device;
		D3DXVECTOR3             _origin;     //�����㾫���Դͷ
		d3d::BoundingBox        _boundingBox;//�㾫��ķ�Χ
		float                   _emitRate;   //�����µ㾫���Ƶ��
		float                   _size;       //�㾫��ĳߴ�
		IDirect3DTexture9*      _tex;        //�㾫�������
		IDirect3DVertexBuffer9* _vb;         //�㾫��Ķ��㻺����
		std::list<Attribute>    _particles;  //�㾫��������б�
		int                     _maxParticles; //�㾫��������Ŀ


		DWORD _vbSize;      // ���㻺�����Ĵ�С
		DWORD _vbOffset;    // ���㻺���ƫ����  
		DWORD _vbBatchSize; // Ҫ������ _vbOffset ��ʼ�Ķ�����Ŀ
		
	};

	//ѩ��ϵͳ
	class Snow : public PSystem
	{
	public:
		Snow(d3d::BoundingBox* boundingBox, int numParticles);
		void resetParticle(Attribute* attribute);
		void update(float timeDelta);
	};

	//�̻�ϵͳ
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