#pragma once
#include "pch.h"

class UKismetMathLibrary final : public UObject
{
private:
    struct KismetMathLibrary_RandomBoolWithWeight
    {
    public:
        float Weight;
        bool ReturnValue;
    };

public:
    static bool RandomBoolWithWeight(float Weight)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "RandomBoolWithWeight");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        struct KismetMathLibrary_RandomBoolWithWeight final
        {
        public:
            float Weight;
            bool ReturnValue;
        } KismetMathLibrary_RandomBoolWithWeight{Weight};

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &KismetMathLibrary_RandomBoolWithWeight);

        return KismetMathLibrary_RandomBoolWithWeight.ReturnValue;
    }

    static FRotator FindLookAtRotation(const struct FVector &Start, const struct FVector &Target)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "FindLookAtRotation");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0;

        struct KismetMathLibrary_RandomIntegerInRange final
        {
        public:
            struct FVector Start;
            struct FVector Target;
            FRotator ReturnValue;
        };
        KismetMathLibrary_RandomIntegerInRange Params;
        Params.Start = std::move(Start);
        Params.Target = std::move(Target);

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static int32 RandomIntegerInRange(int32 min_0, int32 max_0)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "RandomIntegerInRange");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0;

        struct KismetMathLibrary_RandomIntegerInRange final
        {
        public:
            int32 min_0;
            int32 max_0;
            int32 ReturnValue;
        };
        KismetMathLibrary_RandomIntegerInRange Params;
        Params.min_0 = min_0;
        Params.max_0 = max_0;

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static FVector GetDirectionUnitVector(const struct FVector &From, const struct FVector &To)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "GetDirectionUnitVector");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return FVector();

        struct KismetMathLibrary_GetDirectionUnitVector final
        {
        public:
            struct FVector From;        // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector To;          // 0x000C(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector ReturnValue; // 0x0018(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        KismetMathLibrary_GetDirectionUnitVector Params;
        Params.From = std::move(From);
        Params.To = std::move(To);

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static FVector Normal(const struct FVector &A, float Tolerance)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "Normal");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return FVector();

        struct KismetMathLibrary_GetDirectionUnitVector final
        {
        public:
            struct FVector A;           // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            float Tolerance;            // 0x000C(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector ReturnValue; // 0x0018(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        KismetMathLibrary_GetDirectionUnitVector Params;
        Params.A = std::move(A);
        Params.Tolerance = std::move(Tolerance);

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static float Vector_Distance(const struct FVector &From, const struct FVector &To)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "Vector_Distance");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0;

        struct KismetMathLibrary_GetDirectionUnitVector final
        {
        public:
            struct FVector From; // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector To;   // 0x000C(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            float ReturnValue;   // 0x0018(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        KismetMathLibrary_GetDirectionUnitVector Params;
        Params.From = std::move(From);
        Params.To = std::move(To);

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static float RandomFloat()
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "RandomFloat");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0;

        struct KismetMathLibrary_GetDirectionUnitVector final
        {
        public:
            float ReturnValue; // 0x0018(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        KismetMathLibrary_GetDirectionUnitVector Params;

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static float Vector_DistanceSquared(const struct FVector &From, const struct FVector &To)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "Vector_DistanceSquared");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0;

        struct KismetMathLibrary_GetDirectionUnitVector final
        {
        public:
            struct FVector From; // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector To;   // 0x000C(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            float ReturnValue;   // 0x0018(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        KismetMathLibrary_GetDirectionUnitVector Params;
        Params.From = std::move(From);
        Params.To = std::move(To);

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static FVector NegateVector(const struct FVector &A)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "NegateVector");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return FVector();

        struct KismetMathLibrary_NegateVector final
        {
        public:
            struct FVector A;           // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector ReturnValue; // 0x000C(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        KismetMathLibrary_NegateVector Params;
        Params.A = std::move(A);

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    static float RandomFloatInRange(int32 min_0, int32 max_0)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetMathLibrary", "RandomFloatInRange");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0.f;

        struct KismetMathLibrary_RandomFloatInRange final
        {
        public:
            float min_0;
            float max_0;
            float ReturnValue;
        };
        KismetMathLibrary_RandomFloatInRange Params;
        Params.min_0 = min_0;
        Params.max_0 = max_0;

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("KismetMathLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

public:
    DECLARE_STATIC_CLASS(UKismetMathLibrary);
    DECLARE_DEFAULT_OBJECT(UKismetMathLibrary);
};