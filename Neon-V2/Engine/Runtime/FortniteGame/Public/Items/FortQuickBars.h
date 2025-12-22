#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"
#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"

struct FQuickBarSlot final
{
public:
	TArray<struct FGuid>                          Items;                                             // 0x0000(0x0010)(ZeroConstructor, NativeAccessSpecifierPrivate)
	bool                                          bEnabled;                                          // 0x0010(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	uint8                                         Pad_11[0x7];                                       // 0x0011(0x0007)(Fixing Struct Size After Last Property [ Dumper-8 ])
};

struct FQuickBarData final
{
public:
	TArray<struct FQuickBarSlotData>              QuickbarSlots;                                     // 0x0000(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
};

struct FQuickBar final
{
public:
	int32                                         CurrentFocusedSlot;                                // 0x0000(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         PreviousFocusedSlot;                               // 0x0004(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         SecondaryFocusedSlot;                              // 0x0008(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_C[0x4];                                        // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-8 ])
	TArray<struct FQuickBarSlot>                  Slots;                                             // 0x0010(0x0010)(ZeroConstructor, NativeAccessSpecifierPrivate)
	struct FQuickBarData                          DataDefinition;                                    // 0x0020(0x0010)(NativeAccessSpecifierPrivate)
	TSet<class UFortItemDefinition*>              EquippedItemDefinitions;                           // 0x0030(0x0050)(ZeroConstructor, Transient, RepSkip, NativeAccessSpecifierPrivate)
	TArray<int32>                                 SharedVisibleSlotIndicesWhenUsingGamepad;          // 0x0080(0x0010)(ZeroConstructor, Transient, RepSkip, NativeAccessSpecifierPrivate)
};

class AFortQuickBars : public AActor
{
public:
	DEFINE_MEMBER(FQuickBar, AFortQuickBars, PrimaryQuickBar);
	DEFINE_MEMBER(FQuickBar, AFortQuickBars, SecondaryQuickBar);
	
	void OnRep_PrimaryQuickBar() {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortQuickBars", "OnRep_PrimaryQuickBar").Func;
		if (Func) this->ProcessEvent(Func, nullptr);
	}

	void OnRep_SecondaryQuickBar() {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortQuickBars", "OnRep_SecondaryQuickBar").Func;
		if (Func) this->ProcessEvent(Func, nullptr);
	}

	void ServerAddItemInternal(FGuid Item, EFortQuickBars InQuickBar, int32 Slot) {
		static SDK::UFunction* Func = nullptr;
		if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortQuickBars", "ServerAddItemInternal").Func;

		struct 
		{
		public:
			struct FGuid                                  Item;                                              // 0x0000(0x0010)(Parm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			EFortQuickBars                                InQuickBar;                                        // 0x0010(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			uint8                                         Pad_11[0x3];                                       // 0x0011(0x0003)(Fixing Size After Last Property [ Dumper-8 ])
			int32                                         Slot;                                              // 0x0014(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		} FortQuickBars_ServerAddItemInternal{};

		FortQuickBars_ServerAddItemInternal.Item = Item;
		FortQuickBars_ServerAddItemInternal.Slot = Slot;
		FortQuickBars_ServerAddItemInternal.InQuickBar = InQuickBar;
		
		if (Func) this->ProcessEvent(Func, &FortQuickBars_ServerAddItemInternal);
	}
	
public:
	DECLARE_DEFAULT_OBJECT(AFortQuickBars)
	DECLARE_STATIC_CLASS(AFortQuickBars)
};
