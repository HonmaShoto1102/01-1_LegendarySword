#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "gameObject.h"

class CScene
{
protected:
	std::list<CGameObject*> m_GameObject[3];//STLのリスト構造<-配列

public:
	CScene() {}
	virtual ~CScene() {}

	virtual void Init() = 0;//純粋仮想関数


	template <typename T>//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T>//テンプレート関数
	T* GetGameObject(int Layer)
	{
		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//型を調べる
			{
				return (T*)object;
			}
		}
		return NULL;
	}


	template <typename T>//テンプレート関数
	std::vector<T*> GatGameObjects(int Layer)
	{
		std::vector<T*> objects;//STLの配列
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

			m_GameObject[i].clear();//リストのクリア
		}

	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			//ポリモフィズム、多態性、多様性	(インスタンスの関数が呼び出される)
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}

			m_GameObject[i].remove_if([](CGameObject* object) {return object->Destroy(); });
			//ラムダ式
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