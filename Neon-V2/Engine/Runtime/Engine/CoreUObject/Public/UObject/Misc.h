#pragma once
#include "pch.h"

template<typename T>
static inline std::vector<T*> TGetObjectsOfClass(UClass* Class = T::StaticClass())
{
    std::vector<T*> ArrayOfObjects;
    for (int i = 0; i < GUObjectArray.GetObjectArrayNum(); i++)
    {
        SDK::FUObjectItem *IndexedObject =
                         SDK::GUObjectArray.IndexToObject( i );
        if ( !IndexedObject )
            continue;
        UObject *Object = static_cast<UObject*>(IndexedObject->Object);
        if ( !Object )
            continue;

        if (Object)
        {
            if (Object->IsA(Class))
            {
                if (Object->GetFName().ToString().ToString().contains("Default"))
                    continue;

                if (Object->GetFName().ToString().ToString().contains("Test"))
                    continue;
					
                ArrayOfObjects.push_back(Cast<T>((UObject*)Object));
            }
        }
    }

    return ArrayOfObjects;
}

static inline std::vector<UObject*> GetObjectsOfClass(UClass* Class = UObject::StaticClass())
{
    std::vector<UObject*> ArrayOfObjects;
    for (int i = 0; i < GUObjectArray.GetObjectArrayNum(); i++)
    {
        SDK::FUObjectItem *IndexedObject =
                         SDK::GUObjectArray.IndexToObject( i );
        if ( !IndexedObject )
            continue;
        UObject *Object = static_cast<UObject*>(IndexedObject->Object);
        if ( !Object )
            continue;

        if (Object)
        {
            if (Object->IsA(Class))
            {
                ArrayOfObjects.push_back((UObject*)Object);
            }
        }
    }

    return ArrayOfObjects;
}