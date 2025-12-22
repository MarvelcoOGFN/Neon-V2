#pragma once

#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Math/Vector.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"

enum class ETraceTypeQuery : uint8
{
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	ETraceTypeQuery_MAX = 33,
};

enum class EDrawDebugTrace : uint8
{
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4,
};

struct FLinearColor final
{
public:
	float R; // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float G; // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float B; // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float A; // 0x000C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

	static FLinearColor MakeColor(float R, float G, float B, float A = 1.0f)
	{
		FLinearColor Color;
		Color.R = R;
		Color.G = G;
		Color.B = B;
		Color.A = A;
		return Color;
	}

	static FLinearColor Red() { return MakeColor(1.f, 0.f, 0.f, 1.f); }
	static FLinearColor Green() { return MakeColor(0.f, 1.f, 0.f, 1.f); }
};

class UKismetSystemLibrary : public UObject
{
public:
	static inline bool LineTraceSingle(class UObject *WorldContextObject,
									   const struct FVector &Start, const struct FVector &End,
									   ETraceTypeQuery TraceChannel, bool bTraceComplex,
									   const TArray<class AActor *> &ActorsToIgnore, EDrawDebugTrace DrawDebugType,
									   struct FHitResult *OutHit, bool bIgnoreSelf,
									   const struct FLinearColor &TraceColor,
									   const struct FLinearColor &TraceHitColor, float DrawTime)
	{
		static class UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetSystemLibrary", "LineTraceSingle");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return false;

		struct
		{
		public:
			class UObject *WorldContextObject;	   // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			struct FVector Start;				   // 0x0008(0x000C)(ConstParm, Parm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			struct FVector End;					   // 0x0014(0x000C)(ConstParm, Parm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			ETraceTypeQuery TraceChannel;		   // 0x0020(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			bool bTraceComplex;					   // 0x0021(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			uint8 Pad_22[0x6];					   // 0x0022(0x0006)(Fixing Size After Last Property [ Dumper-7 ])
			TArray<class AActor *> ActorsToIgnore; // 0x0028(0x0010)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, NativeAccessSpecifierPublic)
			EDrawDebugTrace DrawDebugType;		   // 0x0038(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			uint8 Pad_39[0x7];					   // 0x0039(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
			struct FHitResult OutHit;			   // 0x0040(0x0088)(Parm, OutParm, IsPlainOldData, NoDestructor, ContainsInstancedReference, NativeAccessSpecifierPublic)
			bool bIgnoreSelf;					   // 0x00C8(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			uint8 Pad_C9[0x3];					   // 0x00C9(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
			struct FLinearColor TraceColor;		   // 0x00CC(0x0010)(Parm, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			struct FLinearColor TraceHitColor;	   // 0x00DC(0x0010)(Parm, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			float DrawTime;						   // 0x00EC(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			bool ReturnValue;					   // 0x00F0(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			uint8 Pad_F1[0x7];					   // 0x00F1(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
		} Params{};

		Params.WorldContextObject = WorldContextObject;
		Params.Start = std::move(Start);
		Params.End = std::move(End);
		Params.TraceChannel = TraceChannel;
		Params.bTraceComplex = bTraceComplex;
		Params.ActorsToIgnore = std::move(ActorsToIgnore);
		Params.DrawDebugType = DrawDebugType;
		Params.bIgnoreSelf = bIgnoreSelf;
		Params.TraceColor = std::move(TraceColor);
		Params.TraceHitColor = std::move(TraceHitColor);
		Params.DrawTime = DrawTime;

		GetDefaultObj()->ProcessEvent(Func, &Params);

		if (OutHit != nullptr)
			*OutHit = std::move(Params.OutHit);

		return Params.ReturnValue;
	}

public:
	DECLARE_DEFAULT_OBJECT(UKismetSystemLibrary);
	DECLARE_STATIC_CLASS(UKismetSystemLibrary);
};