#pragma once
#include "Scene.h"
enum class INGAME_LAYER {
   
    DEFAULT = 0,//GetLayer(0)->SetName(L"Default");
   BLUE=3,//GetLayer(3)->SetName(L"Blue");
   RED=4,//GetLayer(4)->SetName(L"Red");
  COVER=5, //GetLayer(5)->SetName(L"Cover");
   //GetLayer(6)->SetName(L"Arrow");
   //GetLayer(7)->SetName(L"Terrain");
   //GetLayer(8)->SetName(L"Tile");
  OBSTACLE=9, //GetLayer(9)->SetName(L"Obstacle");
  BOX=10,
   //GetLayer(30)->SetName(L"UI");
   //GetLayer(31)->SetName(L"Tool");


};


class CInGameScene :
    public CScene
{
private:
    CSound* m_pSceneBGM;
public:
    virtual void Init();
    ~CInGameScene();
};

