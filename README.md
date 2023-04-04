# 测试工程：关于 UObject 创建和异步加载后处理（PostLoad）的问题

## 引擎版本

5\.1\.1


## 工程说明

工程蓝本是 [UnLua 2.1.4](https://github.com/Tencent/UnLua/releases/tag/v2.1.4) 中的示例工程 TPSProject，改动如下：

+ 删除了不需要的内容，包括 UnLuaIntelliSense，UnLuaIntelliSenseBP 和 UnLuaTestSuite 这三个模块，教程和文档

+ 增加了 `FUObjectCreateListener` 类，用于监听引擎 `FUObjectArray::FUObjectCreateListener` 类的 `NotifyUObjectCreated` 接口；请见 /Source/TPSProject 目录下的 TPSUObjectCreateListener\.h 和 TPSUObjectCreateListener\.cpp

+ 增加了 `UTPSGameInstance` 类，用于维护一个 `FUObjectCreateListener` 类的实例，以及提供 `ClientTravel` 接口，实现异步切换关卡；请见 /Source/TPSProject 目录下的 TPSGameInstance\.h 和 TPSGameInstance\.cpp

+ 增加了启动关卡的游戏模式类，继承自 `AGameModeBase` 类，在调用 `ReceiveBeginPlay` 接口时通过 `UTPSGameInstance::ClientTravel` 接口切换至 MainMap 关卡；请见 /Content/Core/Blueprints/BP_GameLaunch

+ 增加了启动关卡，使用 `BP_GameLaunch` 类作为游戏模式类；请见 /Content/Core/Maps/LaunchMap

+ 在项目设置中，将 *编辑器开始地图（Editor Startup Map）* 和 *游戏默认地图（Game Default Map）* 设置为 `LaunchMap`，将 *游戏实例类（Game Instance Clas）* 设置为 `UTPSGameInstance`


## 问题复现方式

多次测试后发现，**只有满足以下两个条件，才能相对稳定地复现该问题**：

1. 使用 [源码版本的引擎](https://github.com/EpicGames/UnrealEngine/tree/5.1)

    尝试过使用公版引擎，即通过 Epic Games Launcher 下载的引擎，未能复现问题。

2. 使用 **Debug Editor** 的方式编译引擎和工程

    尝试过使用 Development Editor，Test 和 Shipping 的方式编译引擎和工程，都未能复现问题。

满足以上两个条件后，问题的复现步骤如下：

1. 启动引擎编辑器；如果当前已经打开了引擎编辑器，甚至在 PIE 模式下运行过游戏，建议关闭引擎编辑器后再重新打开

2. 以 PIE 或者独立进程的方式运行游戏

3. 在从 `LaunchMap` 切换至 `MainMap` 时，就会出现错误（崩溃），错误是 `UObject::ProcessEvent` 函数中的断言失败，请见 ScriptCore\.cpp

    ```cpp
    void UObject::ProcessEvent(UFunction* Function, void* Parms)
    {
	    checkf(!IsUnreachable(),TEXT("%s  Function: '%s'"), *GetFullName(), *Function->GetPathName());
	    checkf(!FUObjectThreadContext::Get().IsRoutingPostLoad, TEXT("Cannot call UnrealScript (%s - %s) while PostLoading objects"), *GetFullName(), *Function->GetFullName()); // 这里的断言失败
        // ...
    }
    ```


## 初步分析

出现问题时的调用栈如下：

1. `UObject::ProcessEvent`，ScriptCore\.cpp，第1966行

2. `FUObjectCreateListener::NotifyUObjectCreated`，TPSUObjectCreateListener\.cpp，第64行

根据 `UObject::ProcessEvent` 中的断言可知，引擎在创建 UObject 的时候正在执行异步加载的后处理。
