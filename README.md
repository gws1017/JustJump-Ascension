# JustJump - Ascension

**Windows API 기반 2D 플랫포머 게임**

---

## 프로젝트 소개

기존 2D 플랫포머 게임 Just Jump (2020)을 기반으로,  
**코드의 구조 개선 및 시스템화 작업을 통해 게임의 주요 흐름과 시스템을 체계적으로 정리하고 관리 가능한 구조로 업그레이드**를 목표로 하는 프로젝트입니다.

하드코딩 제거, 루프 구조 분리, 애니메이션/사운드 시스템의 모듈화 등을 통해,  
단순한 기능 구현을 넘어서 게임 개발자로서의 설계 능력과 구조적 사고력을 보여주는 것을 목표로 리팩토링하였습니다.

---

## 사용 기술

- **언어:** C++
- **API:** Windows API
- **사운드:** FMOD API

---

## 주요 개선 사항

### 완료

- **윈도우 생성 및 프로시저 함수 App 클래스화**
  - App 클래스를 생성하여 윈도우 초기화/메시지 처리/종료 기능을 관리
  - Win Main에 App을 상속받는 JustJumpApp 클래스 객체를 생성하며 이를 통해 실행 흐름 제어
- **언리얼 코딩 표준을 참고하여 변수명, 함수명 등 수정**
- **FMOD API 최신 버전 적용**
  - FMOD 최신 API 규격에 맞춰 일부 사운드 함수 및 채널 관리 로직 교체
- **QueryPerformanceCounter를 이용한 고해상도 타이머 도입**
  - 델타 타임 기반으로 게임 내 시간 흐름을 안정적으로 관리할 수 있도록 타이머 시스템 구축
  - 애니메이션 기존 obj_t(프레임/스텝 카운터) 기반 → `Update(float dt)` 가상 함수 아키텍처로 전환, 오브젝트별 누적 타이머로 캡슐화
- **폴더 구조 리팩토링**
  - 기능별로 character/, system/, core/, map/ 등으로 소스 분리
- **Getter/Setter 정리 및 헤더 최적화**
  - Public 멤버 함수는 헤더 상단에, Private 멤버 변수는 하단에 배치하여 가독성 향상
  - 헤더 전방 선언
- **입력 시스템 분리**
  - `InputManager` 클래스로 키/마우스 상태 추적 및 `EInputAction` 액션 매핑 계층 도입
- **UI 컴포넌트 분리**
  - `TitleMenuUI`, `DeadScreenUI` 클래스로 마우스 히트테스트 및 렌더링 로직을 GameWorld에서 분리
- **게임 루프 구조 분리**
  - Update, Render, Input 로직을 Win32 메시지 루프에서 분리 → 유지보수성과 확장성 향상
- **하드코딩 제거 및 상수화**
  - 매직 넘버 제거, constexpr 및 enum class 사용으로 가독성 및 안정성 강화
  - UI 버튼 좌표를 `UiHitbox` 네임스페이스의 `constexpr` 구조체 테이블로 이전
  - 전역 변수 제거
- **스마트 포인터 전면 적용**
  - unique_ptr , shared_ptr 등 사용
  - `HBITMAP`은 `BmpPtr`, FMOD `System`/`Sound`는 `FmodSystemPtr`/`FmodSoundPtr`로 커스텀 삭제자 기반 RAII 적용
- **사운드 시스템 리팩토링**
  - `SoundManager` 싱글턴으로 전역 접근, `EBgm`/`EEffect` enum class로 재생 인덱스를 이름 기반으로 전환
- **오브젝트 배치 방식 개선**
  - 기존 텍스트 기반 배치(txt, `x y w h type`) → JSON으로 전환. NuGet `nlohmann.json` 패키지로 파싱
  - 타입을 숫자 대신 `"Platform"`, `"GearRow"` 같은 문자열로 명시해 가독성 확보
  - 인게임 맵 에디터 추가
- **클래스 상속 구조 재정립**
  - `EObstacleType` 13종 전부를 `Obstacle` 서브클래스로 분리, 타입 switch였던 `DrawObj`/`Update`/`IndexChange`를 가상 함수로 전환
  - `CollP2W`의 충돌 히트박스 분기도 `GetHitboxKind()` 가상 함수로 통합

---



## 스크린샷

GIF 2025-04-26 오후 9-32-26
GIF 2025-04-26 오후 9-40-23
GIF 2025-04-26 오후 9-43-11
GIF 2025-04-26 오후 9-54-21

---



## License

본 프로젝트는 개인 포트폴리오 및 학습 목적으로 제작되었습니다.

게임 내 사용된 모든 이미지, 사운드 및 기타 리소스의 저작권은 ㈜넥슨코리아(Nexon Korea Corp.)에 있습니다.

본 프로젝트는 상업적 용도로 사용되지 않았으며, 원 저작권자의 요청 시 언제든지 삭제될 수 있습니다.