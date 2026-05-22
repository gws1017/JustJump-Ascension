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

	void SetMem1DC(HDC* dc) { mem1dc = dc; }

public:

	//땅바닥과 플레이어 충돌체크 1이면 부닥침
	bool CollP2W(PLAYER player, SPtr<Obstacle> object);
	//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
	void AdjustPlayer(PLAYER& player, MAP& m, int& ocount, HINSTANCE g_hinst, Sound& sound);

	//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
	int InitObject(int mapnum, HINSTANCE g_hinst);
	//카메라 무빙워크
	void AdjustCamera(CAMERA& camera, PLAYER player);

	void DrawObjects();

	void IndexChange(const int obj_t);

	void ResetObstacle();

	std::vector<SPtr<Obstacle>> GetObjects() { return m_obstacles; }

	//void RegisterObjects();
private:

	HDC* mem1dc;
	std::vector<SPtr<Obstacle>> m_obstacles;
};

namespace AnimPeriod {
	constexpr int kType0 = 10;
	constexpr int kType4 = 8;
	constexpr int kType103 = 30;
	constexpr int kGear = 5;
	constexpr int kPortal = 20;
}