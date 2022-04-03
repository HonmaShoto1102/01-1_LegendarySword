#pragma once

#include "Scene.h"
#include "ImGUImanager.h"


class CGame :public CScene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	CImGUI* m_pGUI;
	

};