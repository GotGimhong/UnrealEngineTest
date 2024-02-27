#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UButton;

UCLASS()
class MYPROJECT_API UMyUserWidget : public UUserWidget
{
    GENERATED_UCLASS_BODY()

protected:
    /** 用于测试的控件，要求包含事件监听 */
    UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
    UButton* CloseButton;

public:
    virtual void OnWidgetRebuilt() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
    /** CloseButton 控件 OnClicked 事件的回调 */
    UFUNCTION()
    void OnCloseButtonClicked();

    /** 获取 CloseButton 控件的 Template 版本 */
    UButton* GetTemplateCloseButton() const;
};
