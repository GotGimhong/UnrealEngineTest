#pragma once

#include "Components/Image.h"
#include "MyImage.generated.h"

/** 测试用的蓝图结构体 */
USTRUCT(BlueprintType)
struct MYPROJECT_API FMyStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSoftObjectPath Path;
};

/** 测试用的蓝图类型 */
UCLASS()
class MYPROJECT_API UMyImage : public UImage
{
	GENERATED_UCLASS_BODY()
};
