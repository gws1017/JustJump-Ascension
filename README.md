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
  - Win Main에 App을 상속받는 JustJumpApp 클래스 객체를 생성하여 실행 흐름 제어
- **FMOD API 최신 버전 적용**
  - FMOD 최신 API 규격에 맞춰 일부 사운드 함수 및 채널 관리 로직 교체
- **QueryPerformanceCounter를 이용한 고해상도 타이머 도입**
  - 델타 타임 기반으로 게임 내 시간 흐름을 안정적으로 관리할 수 있도록 타이머 시스템 구축
- **폴더 구조 리팩토링**
  - 기능별로 character/, system/, core/, map/ 등으로 소스 분리
- **Getter/Setter 정리 및 헤더 최적화**
  - Public 멤버 함수는 헤더 상단에, Private 멤버 변수는 하단에 배치하여 가독성 향상
- **헤더 전방 선언 적용**
- **입력 시스템 분리**
  - `InputManager` 클래스로 키/마우스 상태 추적 및 `EInputAction` 액션 매핑 계층 도입
- **하드코딩 제거 및 상수화**
  - 매직 넘버 제거, constexpr 및 enum class 사용으로 가독성 및 안정성 강화
  - UI 버튼 좌표를 `UiHitbox` 네임스페이스의 `constexpr` 구조체 테이블로 이전
- **애니메이션/이동 dt 기반화**
  - 기존 obj_t(프레임/스텝 카운터) 기반 → `Update(float dt)` 가상 함수 아키텍처로 전환, 오브젝트별 누적 타이머로 캡슐화
- **UI 컴포넌트 분리**
  - `TitleMenuUI`, `DeadScreenUI` 클래스로 마우스 히트테스트 및 렌더링 로직을 GameWorld에서 분리

### 진행중
- **언리얼 코딩 표준을 참고하여 변수명, 함수명 등 수정**
- **게임 루프 구조 분리**
  - Update, Render, Input 로직을 Win32 메시지 루프에서 분리 → 유지보수성과 확장성 향상
- **스마트 포인터/캡슐화 적용**
  - unique_ptr, shared_ptr 등을 사용하여 전역 변수 제거 및 메모리 안전성 강화
  - App 계층(`m_game_world`, `m_input_manager`, `m_timer`, `m_window`)과 `ObjectManager::m_obstacles`에는 적용 완료
  - `HBITMAP`(Map, TitleMenuUI, DeadScreenUI, PLAYER 등)과 FMOD 리소스는 아직 raw 핸들 + 수동 Load/Unload 패턴


### 진행 예정

- **사운드 시스템 리팩토링**
  - FMOD 사용 → Sound 클래스를 관리하는 싱글턴 매니저 클래스를 추가하여 전역으로 사용
- **오브젝트 배치 방식 개선**
  - 기존 텍스트 기반 배치(txt) → JSON 형식으로 구조화 예정 (맵에 따라 객체 로드)
- **클래스 상속 구조 재정립 예정**
  - Obstacle 클래스, Player 클래스, Camera 클래스를 중심으로 상속/다형성 설계 강화
- **GDI/FMOD 리소스 RAII 래퍼 도입**
  - `HBITMAP` 등 raw 핸들을 커스텀 삭제자 기반 스마트 포인터로 감싸 수동 Load/Unload 짝 맞춤 실수 방지
---

## 스크린샷

![GIF 2025-04-26 오후 9-32-26](https://github.com/user-attachments/assets/c162a9e4-5400-4f66-b0d3-ebadfe8fce4d)
![GIF 2025-04-26 오후 9-40-23](https://github.com/user-attachments/assets/19def80c-1704-4c1a-afc5-24e5b712d0a3)
![GIF 2025-04-26 오후 9-43-11](https://github.com/user-attachments/assets/775b891c-a213-437c-926d-2afe32073bdb)
![GIF 2025-04-26 오후 9-54-21](https://github.com/user-attachments/assets/c3b46748-4225-4e28-9c66-298a80748c6f)

---

## License

본 프로젝트는 개인 포트폴리오 및 학습 목적으로 제작되었습니다.

게임 내 사용된 모든 이미지, 사운드 및 기타 리소스의 저작권은 ㈜넥슨코리아(Nexon Korea Corp.)에 있습니다.

본 프로젝트는 상업적 용도로 사용되지 않았으며, 원 저작권자의 요청 시 언제든지 삭제될 수 있습니다.


## 리팩토링 진행 기록 (추가용)

### 최근 반영 내용

- **GameWorld 구조 정리 (월드 오케스트레이션 유지)**
  - `GameWorld::Update(dt)`를 월드 단위 흐름 제어 중심으로 유지
  - 내부 로직을 `UpdateGameplay(dt)`, `UpdateFadeAndCamera`로 분리해 가독성과 유지보수성 개선
- **Render 단계 분리**
  - 렌더 진입점은 `Render()`로 유지하고, 내부를 `RenderScene`, `RenderStartMenu`, `RenderInGameUI`로 분리
  - 기능 단위 분리로 렌더 파이프라인 파악이 쉬워짐
- **UI 컴포넌트 분리**
  - 마우스 hit-test와 렌더링을 `IsInRect` 계열 멤버 함수 대신 `TitleMenuUI`, `DeadScreenUI` 클래스로 위임
  - `GameWorld`는 상태 판단 후 위임만 수행, 중첩 if 블록과 버튼 상태 변수를 GameWorld에서 제거
- **임시 실험 구조 정리**
  - 과도한 컨텍스트/외부 위임(`GameWorldContext`, `m_update`, `m_render`) 방식은 현재 프로젝트 단계에서 복잡도 증가로 판단하여 제거
  - `GameWorld` 중심 구조를 유지한 채, 내부 함수 분리 방식으로 정리

### 다음 작업 계획

- **UI 렌더링 통합**
  - `RenderStartMenu`, `RenderInGameUI`를 상태 기반 `RenderUI`로 통합 검토
- **GDI/FMOD 리소스 RAII 래퍼 도입**
  - `HBITMAP` 등 raw 핸들을 커스텀 삭제자 기반 스마트 포인터로 감싸 수동 Load/Unload 짝 맞춤 실수 방지