#pragma once

class PLAYER;
class Obstacle;
class MAP;
class CAMERA;
class Object;

using namespace std;


class ObjectManager
{
public:
	ObjectManager() = default;
	virtual ~ObjectManager() = default;

	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

public:

	//땅바닥과 플레이어 충돌체크 1이면 부닥침
	bool CollP2W(PLAYER player, Obstacle object);
	//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
	void AdjustPlayer(PLAYER& player, Obstacle* object, MAP& m, int& ocount, HINSTANCE g_hinst, Sound& sound);

	//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
	int InitObject(Obstacle* obj, int mapnum, HINSTANCE g_hinst);
	//카메라 무빙워크
	void AdjustCamera(CAMERA& camera, PLAYER player);

	//void RegisterObjects();
private:

	std::vector<SPtr<Object>> m_objects;
};