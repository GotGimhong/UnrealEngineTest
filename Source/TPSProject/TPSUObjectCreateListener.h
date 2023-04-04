#pragma once

#include "UObject/UObjectArray.h"

namespace TPS
{
    // 用于监听 NotifyUObjectCreated 函数的调用
    class FUObjectCreateListener : public FUObjectArray::FUObjectCreateListener
    {
    public:
        FUObjectCreateListener(const FUObjectCreateListener& Other) = delete;
        FUObjectCreateListener();

        ~FUObjectCreateListener();

        virtual void OnUObjectArrayShutdown() override;

        virtual void NotifyUObjectCreated(const UObjectBase* ObjectBase, int32 Index) override;

    private:
        bool bListened;
    };
}
