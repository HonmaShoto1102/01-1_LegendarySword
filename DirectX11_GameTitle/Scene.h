#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "gameObject.h"

class CScene
{
protected:
	std::list<CGameObject*> m_GameObject[3];//STL�̃��X�g�\��<-�z��

public:
	CScene() {}
	virtual ~CScene() {}

	virtual void Init() = 0;//�������z�֐�


	template <typename T>//�e���v���[�g�֐�
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T>//�e���v���[�g�֐�
	T* GetGameObject(int Layer)
	{
		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//�^�𒲂ׂ�
			{
				return (T*)object;
			}
		}
		return NULL;
	}


	template <typename T>//�e���v���[�g�֐�
	std::vector<T*> GatGameObjects(int Layer)
	{
		std::vector<T*> objects;//STL�̔z��
		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}


	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}

			m_GameObject[i].clear();//���X�g�̃N���A
		}

	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			//�|�����t�B�Y���A���Ԑ��A���l��	(�C���X�^���X�̊֐����Ăяo�����)
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}

			m_GameObject[i].remove_if([](CGameObject* object) {return object->Destroy(); });
			//�����_��
		}
	}



	virtual void Draw_Object()
	{
		//for(int i=0;i<2;i++)
		for (CGameObject* object : m_GameObject[1])
		{
			object->Draw();
		}
	}

	virtual void Draw_UI()
	{
		for (CGameObject* object : m_GameObject[2])
		{
			object->Draw();
		}
	}

};