#include <cstdlib>
#include "pSystem.h"

using namespace psys;
const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

//
//����ϵͳ��Ĭ�Ϲ��캯��
//
PSystem::PSystem()
{
	_device = 0;
	_vb = 0;
	_tex = 0;
}

//
//����ϵͳ����������
//
PSystem::~PSystem()
{
	d3d::Release<IDirect3DVertexBuffer9*>(_vb);
	d3d::Release<IDirect3DTexture9*>(_tex);
}

//
//��ʼ������ϵͳ
//
bool PSystem::init(IDirect3DDevice9* device, char* texFileName)
{

	_device = device; // �����豸ָ��

	HRESULT hr = 0;

	hr = device->CreateVertexBuffer(
		_vbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT, // ʹ��D3DPOOL_MANAGED�Ͳ���ʹ��D3DUSAGE_DYNAMIC 
		&_vb,
		0);

	if (FAILED(hr))
	{
		::MessageBox(0, "CreateVertexBuffer() - FAILED", "PSystem", 0);
		return false;
	}

	hr = D3DXCreateTextureFromFile(
		device,
		texFileName,
		&_tex);

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXCreateTextureFromFile() - FAILED", "PSystem", 0);
		return false;
	}

	return true;
}

//
//��������
//
void PSystem::reset()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		resetParticle(&(*i));
	}
}

//
//�������
//
void PSystem::addParticle()
{
	Attribute attribute;

	resetParticle(&attribute);

	_particles.push_back(attribute);
}

//
//��Ⱦǰ��������Ⱦ״̬�������ƹ⡢�ںϷ�ʽ��
//
void PSystem::preRender()
{
	//�ƹ�
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.0f));

	//�������ӵ���Ծ���
	_device->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));

	//����Ϊalphaͨ��
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//�ںϷ�ʽ����
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//
//��Ⱦ��
//
void PSystem::postRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

//
//��Ⱦʱ��
//
void PSystem::render()
{
	if (!_particles.empty())
	{
		//
		// ������Ⱦ״̬
		//
		preRender();

		//��������
		_device->SetTexture(0, _tex);
		_device->SetFVF(Particle::FVF);
		_device->SetStreamSource(0, _vb, 0, sizeof(Particle));

		//
		// ÿ����Ⱦһ������
		//
		//����ڻ���������󣬾ʹӻ���������㿪ʼ
		if (_vbOffset >= _vbSize)
			_vbOffset = 0;

		Particle* v = 0;

		_vb->Lock(
			_vbOffset    * sizeof(Particle),
			_vbBatchSize * sizeof(Particle),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		// ��ʾ��������
		std::list<Attribute>::iterator i;
		for (i = _particles.begin(); i != _particles.end(); i++)
		{
			if (i->_isAlive)
			{
				//��һ���������Ӹ��Ƶ���һ����Ƭ����
				v->_position = i->_position;
				v->_color = (D3DCOLOR)i->_color;
				v++;       //��һ������;

				numParticlesInBatch++; //����Ƭ�εĸ���

				if (numParticlesInBatch == _vbBatchSize)
				{
					_vb->Unlock();

					_device->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);

					_vbOffset += _vbBatchSize;

					if (_vbOffset >= _vbSize)
						_vbOffset = 0;

					_vb->Lock(
						_vbOffset    * sizeof(Particle),
						_vbBatchSize * sizeof(Particle),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; 
				}
			}
		}

		_vb->Unlock();

		if (numParticlesInBatch)
		{
			_device->DrawPrimitive(
				D3DPT_POINTLIST,
				_vbOffset,
				numParticlesInBatch);
		}

		_vbOffset += _vbBatchSize;

		postRender();
	}
}

//
//�ж�����ϵͳ�Ƿ�Ϊ��
//
bool PSystem::isEmpty()
{
	return _particles.empty();
}

//
//�������������б��ж������Ƿ�����
//
bool PSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		if (i->_isAlive)
			return false;
	}
	return true;
}

//
//�Ƴ�����������
//
void PSystem::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = _particles.begin();

	while (i != _particles.end())
	{
		if (i->_isAlive == false)
		{
			i = _particles.erase(i);
		}
		else
		{
			i++; 
		}
	}
}



//------------------------------------------------------------------------------
// ѩ��ϵͳ
//------------------------------------------------------------------------------

Snow::Snow(d3d::BoundingBox* boundingBox, int numParticles)
{
	_boundingBox = *boundingBox;
	_size = 0.25f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Snow::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	d3d::GetRandomVector(
		&attribute->_position,
		&_boundingBox._min,
		&_boundingBox._max);

	attribute->_position.y = _boundingBox._max.y;

	attribute->_velocity.x = d3d::GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->_velocity.y = d3d::GetRandomFloat(0.0f, 1.0f) * -10.0f;
	attribute->_velocity.z = 0.0f;

	attribute->_color = d3d::WHITE;
}

void Snow::update(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;

		if (_boundingBox.isPointInside(i->_position) == false)
		{
			resetParticle(&(*i));
		}
	}
}



//------------------------------------------------------------------------------
// �̻�ϵͳ
//------------------------------------------------------------------------------
Firework::Firework(D3DXVECTOR3* origin, int numParticles)
{
	_origin = *origin;
	_size = 0.9f;
	_vbSize = 512;
	_vbOffset = 0;
	_vbBatchSize = 128;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Firework::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;
	attribute->_position = _origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	d3d::GetRandomVector(
		&attribute->_velocity,
		&min,
		&max);

	// �淶���ٶ�����
	D3DXVec3Normalize(
		&attribute->_velocity,
		&attribute->_velocity);

	attribute->_velocity *= 100.0f;

	attribute->_color = D3DXCOLOR(
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		1.0f);

	attribute->_age = 0.0f;
	attribute->_lifeTime = 2.0f; //����ʱ������Ϊ2��
}

void Firework::update(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		//ֻ�������������
		if (i->_isAlive)
		{
			i->_position += i->_velocity * timeDelta;

			i->_age += timeDelta;

			if (i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;
		}
	}
}

void Firework::preRender()
{
	PSystem::preRender();

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ֻ����д��Ȼ���
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Firework::postRender()
{
	PSystem::postRender();

	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}



