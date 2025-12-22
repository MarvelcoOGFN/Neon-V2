#pragma once
#include "pch.h"

#include "FortPawn.h"
#include "Engine/Runtime/Engine/Core/Public/Misc/Guid.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"

struct FFortPickupRequestInfo final
{
public:
	struct FGuid SwapWithItem;	  // 0x0000(0x0010)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float FlyTime;				  // 0x0010(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector Direction;	  // 0x0014(0x000C)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 bPlayPickupSound : 1;	  // 0x0020(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8 bIsAutoPickup : 1;	  // 0x0020(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8 bUseRequestedSwap : 1;  // 0x0020(0x0001)(BitIndex: 0x02, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8 bTrySwapWithWeapon : 1; // 0x0020(0x0001)(BitIndex: 0x03, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8 Pad_21[0x3];			  // 0x0021(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FTimerHandle
{
	FTimerHandle()
		: Handle(0)
	{
	}

	bool IsValid() const
	{
		return Handle != 0;
	}

	void Invalidate()
	{
		Handle = 0;
	}

	bool operator==(const FTimerHandle &Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FTimerHandle &Other) const
	{
		return Handle != Other.Handle;
	}

	static const uint32 IndexBits = 24;
	static const uint32 SerialNumberBits = 40;

	static_assert(IndexBits + SerialNumberBits == 64, "The space for the timer index and serial number should total 64 bits");

	static const int32 MaxIndex = (int32)1 << IndexBits;
	static const uint64 MaxSerialNumber = (uint64)1 << SerialNumberBits;

	void SetIndexAndSerialNumber(int32 Index, uint64 SerialNumber)
	{
		Handle = (SerialNumber << IndexBits) | (uint64)(uint32)Index;
	}

	FORCEINLINE int32 GetIndex() const
	{
		return (int32)(Handle & (uint64)(MaxIndex - 1));
	}

	FORCEINLINE uint64 GetSerialNumber() const
	{
		return Handle >> IndexBits;
	}

	uint64 Handle;
};

struct FZiplinePawnState final
{
public:
	TWeakObjectPtr<class AFortAthenaZiplineBase> Zipline;			  // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TWeakObjectPtr<class AFortAthenaZiplineBase> PreviousZipline;	  // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TWeakObjectPtr<class UPrimitiveComponent> InteractComponent;	  // 0x0010(0x0008)(ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool bIsZiplining;												  // 0x0018(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool bJumped;													  // 0x0019(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool bReachedEnd;												  // 0x001A(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_1B[0x1];												  // 0x001B(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
	int32 AuthoritativeValue;										  // 0x001C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector SocketOffset;									  // 0x0020(0x0018)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float TimeZipliningBegan;										  // 0x0038(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float TimeZipliningEndedFromJump;								  // 0x003C(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FTimerHandle RemoveZiplineFromIgnoreWhenMovingTimerHandle; // 0x0040(0x0008)(RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_48[0x18];												  // 0x0048(0x0018)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class AFortPlayerPawn : public AFortPawn
{
public:
	DEFINE_BOOL(AFortPlayerPawn, bIsRespawning)
	DEFINE_PTR(struct FFortAthenaLoadout, AFortPlayerPawn, CosmeticLoadout);
	DEFINE_MEMBER(TArray<class AFortPickup *>, AFortPlayerPawn, IncomingPickups);

public:
	DECLARE_STATIC_CLASS(AFortPlayerPawn)
	DECLARE_DEFAULT_OBJECT(AFortPlayerPawn)
public:
	static inline void (*CompletePickupAnimationOG)(AFortPickup *Pickup);
	static void CompletePickupAnimation(AFortPickup *Pickup);
	void ServerHandlePickup(FFrame &Stack);
	void ServerHandlePickupInfo(FFrame &Stack);
	void OnCapsuleBeginOverlap(FFrame &Stack);
	void ServerSendZiplineState(FFrame &Stack);
	void BeginSkydiving(bool NewValue)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerPawn", "BeginSkydiving");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return;

		struct
		{
			bool bStartSkydiving;
		} BeginSkydivingParams{NewValue};

		this->ProcessEvent(Func, &BeginSkydivingParams);
	}

	void OnRep_CosmeticLoadout()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerPawn", "OnRep_CosmeticLoadout");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return;

		this->ProcessEvent(Func, nullptr);
	}
};
