#pragma once

//map/manifest.json 기반 레벨 진행 정보. 맵을 추가/삭제해도 재컴파일 없이
//manifest.json의 gameplayMapCount만 바꾸면 반영된다.
namespace LevelConfig
{
	//게임플레이 맵 개수 (manifest.json의 "gameplayMapCount"). 파일이 없으면 기본값 3
	int GetGameplayMapCount();
	//클리어 화면의 맵 번호 (GameplayMin + GetGameplayMapCount())
	int GetClearMapNumber();
	//편집 모드: 개수가 atLeast보다 작으면 늘리고 manifest.json에 즉시 저장
	void EnsureGameplayMapCount(int atLeast);
}
