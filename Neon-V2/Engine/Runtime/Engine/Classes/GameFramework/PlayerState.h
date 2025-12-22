#pragma once
#include "pch.h"

#include "Actor.h"

struct PlayerData
{
    std::string AccountID;
};

inline std::unordered_map<class APlayerState*, PlayerData> PlayerMap;

class APlayerState : public AActor
{
public:
    DEFINE_MEMBER(FUniqueNetIdRepl, APlayerState, UniqueId);
    FString GetPlayerName() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("PlayerState", "GetPlayerName").Func;

        struct
        {
        public:
            class FString                                 ReturnValue;                                       // 0x0000(0x0010)(Parm, OutParm, ZeroConstructor, ReturnParm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } PlayerState_GetPlayerName{};
        
        if (Func) this->ProcessEvent(Func, &PlayerState_GetPlayerName);

        return PlayerState_GetPlayerName.ReturnValue;
    }

    inline std::string GetAccountID()
    {
        auto it = PlayerMap.find(this);
        if (it != PlayerMap.end())
        {
            return it->second.AccountID;
        }
        
        uint8* ReplicationBytesData =GetUniqueId().ReplicationBytes.GetData();
        uint8 AIDLen = *(uint8*)(ReplicationBytesData + 1);
        std::stringstream ss;

        for (int i = 0; i < AIDLen; i++)
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)*(uint8*)(ReplicationBytesData + 2 + i);

        PlayerData data;
        data.AccountID = ss.str();
        PlayerMap[this] = data;
        
        return data.AccountID;
    }
public:
    DECLARE_STATIC_CLASS(APlayerState);
    DECLARE_DEFAULT_OBJECT(APlayerState);
};
