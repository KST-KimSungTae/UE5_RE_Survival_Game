#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZuoBlueprintFunctionLibrary.generated.h"


UCLASS()
class SKYSTUDIO_API UZuoBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /** 간단한 헬로우 테스트 */
    UFUNCTION(BlueprintCallable, Category = "ZUO")
    static void PrintHelloZuoLab();

    /** Steam 로그인 기능 */
    UFUNCTION(BlueprintCallable, Category = "ZUO")
    static bool LoginToSteam();

    UFUNCTION(BlueprintCallable, Category = "Zuo")
    static void SendGameDataToServer(
        FString URL,
        FString Nickname,
        int32 Item1, int32 Item2, int32 Item3, int32 Item4, int32 Item5,
        int32 Item6, int32 Item7, int32 Item8, int32 Item9, int32 Item10,
        int32 Lv, bool bIsWin, float PlayTime, int32 Kill, int32 Mode
    );

};
