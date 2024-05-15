#include "MyImage.h"

UMyImage::UMyImage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    static FStructProperty* Property = nullptr;
    if (!Property && HasAnyFlags(RF_ClassDefaultObject))
    {
        UClass* const Class = GetClass();
        // property 的名称是 MyStruct，类型是 FMyStruct
        Property = new FStructProperty(Class, TEXT("MyStruct"), RF_Public | RF_MarkAsNative);
        Property->SetMetaData(TEXT("Category"), TEXT("Custom Data"));
        Property->SetPropertyFlags(EPropertyFlags::CPF_Edit);
        Property->SetPropertyFlags(EPropertyFlags::CPF_BlueprintVisible);
        Property->Struct = FMyStruct::StaticStruct();
        Class->AddCppProperty(Property);
        Class->Bind();
        Class->StaticLink(true);
    }
}
