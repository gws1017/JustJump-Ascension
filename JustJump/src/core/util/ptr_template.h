#pragma once
#include <Windows.h>
#include <memory>
#include <fmod.hpp>

template<class T>
using UPtr = std::unique_ptr<T>;
template<class T, class ... Args>
constexpr UPtr<T> CreateUPtr(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using SPtr = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr SPtr<T> CreateSPtr(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using WPtr = std::weak_ptr<T>;

// HBITMAP 전용 RAII. Win32 API에는 .get()으로 전달한다.
// BitmapDeleter: unique_ptr 기본 삭제자(delete)는 HBITMAP엔 안 맞아서
//   DeleteObject를 대신 호출하도록 지정하는 커스텀 삭제자. 상태 없는 빈 구조체라
//   BmpPtr 크기가 raw 포인터와 동일하게 유지된다(EBO).
// GdiDetail: BitmapDeleter는 외부에서 직접 쓸 일 없는 내부 구현이라 구분해서 묶어둠.
namespace GdiDetail
{
	struct BitmapDeleter
	{
		void operator()(HBITMAP h) const noexcept { if (h) DeleteObject(h); }
	};
}

using BmpPtr = std::unique_ptr<std::remove_pointer_t<HBITMAP>, GdiDetail::BitmapDeleter>;
inline BmpPtr CreateBmpPtr(HBITMAP raw)
{
	return BmpPtr(raw);
}

// FMOD System/Sound 전용 RAII. 둘 다 delete 대신 release()로 해제해야 한다.
// FMOD::Channel은 System이 내부적으로 관리하는 일시적 핸들이라(release 없음, stop만 함)
// 스마트 포인터로 감싸지 않는다.
namespace FmodDetail
{
	struct SystemDeleter
	{
		void operator()(FMOD::System* s) const noexcept { if (s) s->release(); }
	};
	struct SoundDeleter
	{
		void operator()(FMOD::Sound* s) const noexcept { if (s) s->release(); }
	};
}

using FmodSystemPtr = std::unique_ptr<FMOD::System, FmodDetail::SystemDeleter>;
using FmodSoundPtr = std::unique_ptr<FMOD::Sound, FmodDetail::SoundDeleter>;

