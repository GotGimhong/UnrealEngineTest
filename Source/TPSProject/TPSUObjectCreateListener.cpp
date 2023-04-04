#include "TPSUObjectCreateListener.h"
#include "TPSCharacter.h"

namespace TPS
{
    FUObjectCreateListener::FUObjectCreateListener()
        : bListened(true)
    {
        GUObjectArray.AddUObjectCreateListener(this);
    }

    FUObjectCreateListener::~FUObjectCreateListener()
    {
        if (bListened)
        {
            GUObjectArray.RemoveUObjectCreateListener(this);
            bListened = false;
        }
    }

    void FUObjectCreateListener::OnUObjectArrayShutdown()
    {
    #if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 23)
        const bool bEngineExit = IsEngineExitRequested();
    #else
        const bool bEngineExit = GIsRequestingExit;
    #endif
        if (bListened && bEngineExit)
        {
            GUObjectArray.RemoveUObjectCreateListener(this);
            bListened = false;
        }
    }

    void FUObjectCreateListener::NotifyUObjectCreated(const UObjectBase* const ObjectBase, const int32 Index)
    {
        const UObject* const Object = static_cast<const UObject*>(ObjectBase);
        if (Object->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
        {
            return;
        }

        UClass* const Class = Object->GetClass();
        if (Class == nullptr || !Class->IsChildOf<ATPSCharacter>())
        {
            return;
        }

        UObject* const Cdo = Class->GetDefaultObject();
        // 使用任意的蓝图函数来测试，这里选用了 GetTransform
        UFunction* const Function = Class->FindFunctionByName(TEXT("GetTransform"));
        if (Cdo == nullptr || Function == nullptr)
        {
            return;
        }

        FTransform Transform;
        // 可能会引起断言失败的问题，此时 FUObjectThreadContext::Get().IsRoutingPostLoad == true
        if (FUObjectThreadContext::Get().IsRoutingPostLoad)
        {
            UE_LOG(LogTemp, Error, TEXT("%s: Post load is still running, character is %s"), ANSI_TO_TCHAR(__FUNCTION__), *Object->GetName());
        }
        UE_LOG(LogTemp, Display, TEXT("%s calls ProcessEvent on %s"), *Cdo->GetName(), *Function->GetName());
        Cdo->ProcessEvent(Function, &Transform);
    }
}
