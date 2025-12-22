#pragma once
#include "pch.h"

#include "UObjectGlobals.h"
#include "Engine/Plugins/Kismet/Public/KismetStringLibrary.h"

class FWeakObjectPtr
{
public:
    int32                                         ObjectIndex;                                       // 0x0000(0x0004)(NOT AUTO-GENERATED PROPERTY)
    int32                                         ObjectSerialNumber;                                // 0x0004(0x0004)(NOT AUTO-GENERATED PROPERTY)

    FWeakObjectPtr(int32 Index = 0, int32 SerialNumber = 0)
    : ObjectIndex(Index), ObjectSerialNumber(SerialNumber)
    {}

    FWeakObjectPtr(UObjectBase* Object) {
        ObjectIndex = Object->GetUniqueID();
        ObjectSerialNumber = GUObjectArray.IndexToObject(ObjectIndex)->SerialNumber;
    }
	
public:
    class UObject* Get() const
    {
        return (UObject*)GUObjectArray.IndexToObject(ObjectIndex)->Object;
    }
    class UObject* operator->() const;
    bool operator==(const FWeakObjectPtr& Other) const;
    bool operator!=(const FWeakObjectPtr& Other) const;
    bool operator==(const class UObject* Other) const;
    bool operator!=(const class UObject* Other) const;
};

template<typename UEType>
class TWeakObjectPtr : public FWeakObjectPtr
{
public:
    TWeakObjectPtr(int32 Index = 0, int32 SerialNumber = 0)
    : FWeakObjectPtr(Index, SerialNumber)
    {}

    TWeakObjectPtr(UEType* Obj)
        : FWeakObjectPtr(Obj) 
    {}
	
    UEType* Get() const
    {
        return static_cast<UEType*>(FWeakObjectPtr::Get());
    }

    UEType* operator->() const
    {
        return static_cast<UEType*>(FWeakObjectPtr::Get());
    }
};

template<class TObjectID>
struct TPersistentObjectPtr
{
public:
    /** Once the object has been noticed to be loaded, this is set to the object weak pointer **/
    mutable FWeakObjectPtr	WeakPtr;
    /** Compared to CurrentAnnotationTag and if they are not equal, a guid search will be performed **/
    mutable int			TagAtLastTest;
    /** Guid for the object this pointer points to or will point to. **/
    TObjectID				ObjectID;
};

struct FSoftObjectPath
{
public:
    /** Asset path, patch to a top level object in a package. This is /package/path.assetname */
    SDK::FName AssetPathName;

    /** Optional FString for subobject within an asset. This is the sub path after the : */
    SDK::FString SubPathString;
};

struct FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
public:
};

template<class T = SDK::UObject>
struct TSoftObjectPtr
{
public:
    FSoftObjectPtr SoftObjectPtr;

    bool IsValid()
    {
        return SoftObjectPtr.ObjectID.AssetPathName.GetComparisonIndex();
    }

    T* Get(UClass* ClassToLoad = nullptr, bool bTryToLoad = false)
    {
        if (SoftObjectPtr.ObjectID.AssetPathName.GetComparisonIndex() <= 0)
            return nullptr;

        if (bTryToLoad && ClassToLoad)
        {
            return StaticLoadObject<T>(
                UKismetStringLibrary::Conv_NameToString(SoftObjectPtr.ObjectID.AssetPathName).ToString(), 
                ClassToLoad
            );
        }
    
        if (bTryToLoad)
        {
            return StaticLoadObject<T>(UKismetStringLibrary::Conv_NameToString(SoftObjectPtr.ObjectID.AssetPathName).ToString());
        }

        return StaticFindObject<T>(SoftObjectPtr.ObjectID.AssetPathName.ToString().ToString());
    }

    T* NewGet()
    {
        if (this->SoftObjectPtr.WeakPtr.ObjectIndex != -1)
        {
            SDK::FUObjectItem* ObjectItem = GUObjectArray.IndexToObject(this->SoftObjectPtr.WeakPtr.ObjectIndex);
            if (ObjectItem && ObjectItem->Object)
            {
                SDK::UObject* UObj = static_cast<SDK::UObject*>(ObjectItem->Object);
                return Cast<T>(UObj);
            }
        }

        std::string AssetName = UKismetStringLibrary::Conv_NameToString(this->SoftObjectPtr.ObjectID.AssetPathName).ToString();
    
        for (int32 i = 0; i < GUObjectArray.GetObjectArrayNum(); ++i)
        {
            FUObjectItem* Object = GUObjectArray.IndexToObject(i);
            if (!Object)
                continue;
            
            if (Object->Object->GetFName().ToString().ToString().contains(AssetName))
            {
                SDK::UObject* UObj = static_cast<SDK::UObject*>(Object->Object);
                T* CastedObject = Cast<T>(UObj);
                if (CastedObject)
                    return CastedObject;
            }
        }

        return StaticLoadObject<T>(UKismetStringLibrary::Conv_NameToString(SoftObjectPtr.ObjectID.AssetPathName).ToString());
    }
};

template<class T = SDK::UClass>
struct TSoftClassPtr
{
public:
   FSoftObjectPtr SoftObjectPtr;

   TSoftClassPtr() = default;
   
   TSoftClassPtr(UClass* InClass)
   {
       if (InClass)
       {
           SoftObjectPtr.ObjectID.AssetPathName = InClass->GetFName();
       }
   }

   bool IsValid()
   {
       return SoftObjectPtr.ObjectID.AssetPathName;
   }

   T* Get(UClass* ClassToLoad = nullptr, bool bTryToLoad = false)
   {
       if (SoftObjectPtr.ObjectID.AssetPathName.GetComparisonIndex() <= 0)
           return nullptr;

       if (bTryToLoad && ClassToLoad)
       {
           return StaticLoadObject<T>(
               UKismetStringLibrary::Conv_NameToString(SoftObjectPtr.ObjectID.AssetPathName).ToString(), 
               ClassToLoad
           );
       }
   
       if (bTryToLoad)
       {
           return StaticLoadObject<T>(UKismetStringLibrary::Conv_NameToString(SoftObjectPtr.ObjectID.AssetPathName).ToString());
       }

       return StaticFindObject<T>(SoftObjectPtr.ObjectID.AssetPathName.ToString().ToString());
   }

   T* NewGet()
   {
       if (this->SoftObjectPtr.WeakPtr.ObjectIndex != -1)
       {
           SDK::FUObjectItem* ObjectItem = GUObjectArray.IndexToObject(this->SoftObjectPtr.WeakPtr.ObjectIndex);
           if (ObjectItem && ObjectItem->Object)
           {
               SDK::UObject* UObj = static_cast<SDK::UObject*>(ObjectItem->Object);
               return Cast<T>(UObj);
           }
       }

       std::string AssetName = UKismetStringLibrary::Conv_NameToString(this->SoftObjectPtr.ObjectID.AssetPathName).ToString();
   
       for (int32 i = 0; i < GUObjectArray.GetObjectArrayNum(); ++i)
       {
           FUObjectItem* Object = GUObjectArray.IndexToObject(i);
           if (!Object)
               continue;
           
           if (Object->Object->GetFName().ToString().ToString().contains(AssetName))
           {
               SDK::UObject* UObj = static_cast<SDK::UObject*>(Object->Object);
               T* CastedObject = Cast<T>(UObj);
               if (CastedObject)
                   return CastedObject;
           }
       }

       return StaticLoadObject<T>(UKismetStringLibrary::Conv_NameToString(SoftObjectPtr.ObjectID.AssetPathName).ToString());
   }

   static TSoftClassPtr<T> FromClass(UClass* InClass)
   {
       TSoftClassPtr<T> Result;
       if (InClass)
       {
           Result.SoftObjectPtr.ObjectID.AssetPathName = InClass->GetFName();
       }
       return Result;
   }
};

template<typename ClassType>
class TSubclassOf
{
    class UClass* ClassPtr;

public:
    TSubclassOf() = default;

    inline TSubclassOf(UClass* Class)
        : ClassPtr(Class)
    {
    }

    inline UClass* Get()
    {
        return ClassPtr;
    }

    inline operator UClass* () const
    {
        return ClassPtr;
    }

    template<typename Target, typename = std::enable_if<std::is_base_of_v<Target, ClassType>, bool>::type>
    inline operator TSubclassOf<Target>() const
    {
        return ClassPtr;
    }

    inline UClass* operator->()
    {
        return ClassPtr;
    }

    inline TSubclassOf& operator=(UClass* Class)
    {
        ClassPtr = Class;

        return *this;
    }

    inline bool operator==(const TSubclassOf& Other) const
    {
        return ClassPtr == Other.ClassPtr;
    }

    inline bool operator!=(const TSubclassOf& Other) const
    {
        return ClassPtr != Other.ClassPtr;
    }

    inline bool operator==(UClass* Other) const
    {
        return ClassPtr == Other;
    }

    inline bool operator!=(UClass* Other) const
    {
        return ClassPtr != Other;
    }
};
