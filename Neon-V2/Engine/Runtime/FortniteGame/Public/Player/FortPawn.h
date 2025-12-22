#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Character.h"
#include "Engine/Runtime/Engine/Core/Public/Misc/Guid.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Weapons/FortWeapon.h"
#include "Engine/Runtime/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTag.h"

class AFortPawn : public ACharacter
{
public:
	bool PickUpActor(AActor *PickupTarget, class UFortDecoItemDefinition *PlacementDecoItemDefinition)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPawn", "PickUpActor");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return false;

		struct
		{
			AActor *PickupTarget;
			class UFortDecoItemDefinition *PlacementDecoItemDefinition;
			bool ReturnValue;
		} AFortPawn_PickUpActor_Params{PickupTarget, PlacementDecoItemDefinition};

		this->ProcessEvent(Func, &AFortPawn_PickUpActor_Params);

		return AFortPawn_PickUpActor_Params.ReturnValue;
	}

	AFortWeapon *EquipWeaponDefinition(UObject *WeaponData, const FGuid &ItemEntryGuid)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPawn", "EquipWeaponDefinition");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return nullptr;

		struct FortPawn_EquipWeaponDefinition final
		{
		public:
			const UObject *WeaponData;
			FGuid ItemEntryGuid;
			AFortWeapon *ReturnValue;
		};
		FortPawn_EquipWeaponDefinition Params;
		Params.WeaponData = WeaponData;
		Params.ItemEntryGuid = ItemEntryGuid;

		this->ProcessEvent(Func, &Params);

		return Params.ReturnValue;
	}

	AFortWeapon *EquipWeaponDefinition(UObject *WeaponData, const FGuid &ItemEntryGuid, const struct FGuid &TrackerGuid)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPawn", "EquipWeaponDefinition");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return nullptr;

		struct FortPawn_EquipWeaponDefinition final
		{
		public:
			class UObject *WeaponData;		// 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			struct FGuid ItemEntryGuid;		// 0x0008(0x0010)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			struct FGuid TrackerGuid;		// 0x0018(0x0010)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			class AFortWeapon *ReturnValue; // 0x0028(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		};
		FortPawn_EquipWeaponDefinition Params;
		Params.WeaponData = WeaponData;
		Params.ItemEntryGuid = ItemEntryGuid;
		Params.TrackerGuid = TrackerGuid;

		this->ProcessEvent(Func, &Params);

		return Params.ReturnValue;
	}

	struct FAthenaBatchedDamageGameplayCues_Shared final
	{
	public:
		struct FVector_NetQuantize10 Location;			  // 0x0000(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
		struct FVector_NetQuantizeNormal Normal;		  // 0x000C(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
		float Magnitude;								  // 0x0018(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bWeaponActivate;							  // 0x001C(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsFatal;									  // 0x001D(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsCritical;								  // 0x001E(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsShield;									  // 0x001F(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsShieldDestroyed;						  // 0x0020(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsShieldApplied;							  // 0x0021(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsBallistic;								  // 0x0022(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8 Pad_23[0x1];								  // 0x0023(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
		struct FVector_NetQuantize10 NonPlayerLocation;	  // 0x0024(0x000C)(RepSkip, NoDestructor, NativeAccessSpecifierPublic)
		struct FVector_NetQuantizeNormal NonPlayerNormal; // 0x0030(0x000C)(RepSkip, NoDestructor, NativeAccessSpecifierPublic)
		float NonPlayerMagnitude;						  // 0x003C(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool NonPlayerbIsFatal;							  // 0x0040(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool NonPlayerbIsCritical;						  // 0x0041(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsValid;									  // 0x0042(0x0001)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8 Pad_43[0x1];								  // 0x0043(0x0001)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct FAthenaBatchedDamageGameplayCues_NonShared final
	{
	public:
		class AActor *HitActor;			 // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class AActor *NonPlayerHitActor; // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct FAthenaBatchedDamageGameplayCues final
	{
	public:
		class AActor *HitActor;							  // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector_NetQuantize10 Location;			  // 0x0008(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
		struct FVector_NetQuantizeNormal Normal;		  // 0x0014(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
		float Magnitude;								  // 0x0020(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bWeaponActivate;							  // 0x0024(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsFatal;									  // 0x0025(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsCritical;								  // 0x0026(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsShield;									  // 0x0027(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsShieldDestroyed;						  // 0x0028(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8 Pad_29[0x7];								  // 0x0029(0x0007)(Fixing Size After Last Property [ Dumper-8 ])
		class AActor *NonPlayerHitActor;				  // 0x0030(0x0008)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector_NetQuantize10 NonPlayerLocation;	  // 0x0038(0x000C)(RepSkip, NoDestructor, NativeAccessSpecifierPublic)
		struct FVector_NetQuantizeNormal NonPlayerNormal; // 0x0044(0x000C)(RepSkip, NoDestructor, NativeAccessSpecifierPublic)
		float NonPlayerMagnitude;						  // 0x0050(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool NonPlayerbIsFatal;							  // 0x0054(0x0001)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool NonPlayerbIsCritical;						  // 0x0055(0x0001)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool bIsValid;									  // 0x0056(0x0001)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8 Pad_57[0x1];								  // 0x0057(0x0001)(Fixing Struct Size After Last Property [ Dumper-8 ])
	};

	DefineUOriginal(void, MovingEmoteStopped, AFortPawn *, FFrame &Stack);
	DefineUOriginal(void, NetMulticast_Athena_BatchedDamageCues, AFortPawn *, FFrame &Stack);
public:

	void ApplySiphon(int SiphonAmount, int MaxHealth, int MaxShield)
	{
		if (!this) return;

		int Health = this->GetHealth();
		int Shield = this->GetShield();

		if (Health > MaxHealth) Health = MaxHealth;
		if (Shield > MaxShield) Shield = MaxShield;

		if (Health == MaxHealth)
		{
			if (Shield + SiphonAmount > MaxShield)
				Shield = MaxShield;
			else
				Shield += SiphonAmount;
		}
		else if (Health + SiphonAmount > MaxHealth)
		{
			int Overflow = (Health + SiphonAmount) - MaxHealth;
			Health = MaxHealth;

			if (Shield + Overflow > MaxShield)
				Shield = MaxShield;
			else
				Shield += Overflow;
		}
		else
		{
			Health += SiphonAmount;
		}

		this->SetHealth(Health);
		this->SetShield(Shield);
	}
	float GetHealth()
	{
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "GetHealth").Func;
		if (!Func) return 0;
		struct { float ret; } Params{  };
		this->ProcessEvent(Func, &Params);
		return Params.ret;
	}
	float GetShield()
	{
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "GetShield").Func;
		if (!Func) return 0;
		struct { float ret; } Params{  };
		this->ProcessEvent(Func, &Params);
		return Params.ret;
	}
	void SetHealth(float val) {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "SetHealth").Func;
		if (!Func) return;
		struct { float val; } Params{ val };
		this->ProcessEvent(Func, &Params);
	}
	void SetShield(float val) {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "SetShield").Func;
		if (!Func) return;
		struct { float val; } Params{ val };
		this->ProcessEvent(Func, &Params);
	}
	float GetMaxHealth()
	{
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "GetMaxHealth").Func;
		if (!Func) return 0;
		struct { float ret; } Params{  };
		this->ProcessEvent(Func, &Params);
		return Params.ret;
	}
	float GetMaxShield()
	{
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "GetMaxShield").Func;
		if (!Func) return 0;
		struct { float ret; } Params{  };
		this->ProcessEvent(Func, &Params);
		return Params.ret;
	}
	void SetMaxHealth(float val) {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "SetMaxHealth").Func;
		if (!Func) return;
		struct { float val; } Params{ val };
		this->ProcessEvent(Func, &Params);
	}
	
	void SetMaxShield(float val) {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPawn", "SetMaxShield").Func;
		if (!Func) return;
		struct { float val; } Params{ val };
		this->ProcessEvent(Func, &Params);
	}


public:
	DEFINE_BOOL(AFortPawn, bMovingEmote);
	DEFINE_BOOL(AFortPawn, bMovingEmoteForwardOnly);
	DEFINE_BOOL(AFortPawn, bMovingEmoteFollowingOnly);
	DEFINE_MEMBER(float, AFortPawn, EmoteWalkSpeed);
	DEFINE_PTR(AFortWeapon, AFortPawn, CurrentWeapon);
	DECLARE_STATIC_CLASS(AFortPawn)
	DECLARE_DEFAULT_OBJECT(AFortPawn)
};
