#include "MyUserWidget.h"
#include "Components/Button.h"
#if WITH_EDITOR
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#endif

UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CloseButton(nullptr)
{

}

void UMyUserWidget::OnWidgetRebuilt()
{
    Super::OnWidgetRebuilt();

    // 添加一次事件监听即可
    UButton* const TemplateCloseButton = GetTemplateCloseButton();
    if (IsValid(TemplateCloseButton) && !TemplateCloseButton->OnClicked.IsAlreadyBound(this, &UMyUserWidget::OnCloseButtonClicked))
    {
        TemplateCloseButton->OnClicked.AddDynamic(this, &UMyUserWidget::OnCloseButtonClicked);
    }
}

void UMyUserWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
    UButton* const TemplateCloseButton = GetTemplateCloseButton();
    if (IsValid(TemplateCloseButton) && TemplateCloseButton->OnClicked.IsAlreadyBound(this, &UMyUserWidget::OnCloseButtonClicked))
    {
        TemplateCloseButton->OnClicked.RemoveAll(this);
    }

    Super::ReleaseSlateResources(bReleaseChildren);
}

void UMyUserWidget::OnCloseButtonClicked()
{
    UE_LOG(LogTemp, Display, TEXT("%s: %s"), ANSI_TO_TCHAR(__FUNCTION__), *GetFullName());
}

UButton* UMyUserWidget::GetTemplateCloseButton() const
{
    if (IsValid(CloseButton) && CloseButton->GetFName() != NAME_None && IsValid(GetClass()->ClassGeneratedBy))
    {
    #if WITH_EDITOR
        UWidgetBlueprint* const Blueprint = CastChecked<UWidgetBlueprint>(GetClass()->ClassGeneratedBy);
        return Cast<UButton>(Blueprint->WidgetTree->FindWidget(CloseButton->GetFName()));
    #endif
    }
    return nullptr;
}
