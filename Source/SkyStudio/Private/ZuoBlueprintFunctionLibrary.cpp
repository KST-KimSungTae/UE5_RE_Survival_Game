#include "ZuoBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Http.h"


// 로그 출력 헬퍼
static void ZuoPrint(FString Text, FLinearColor Color = FLinearColor::White)
{
    FString TimeStamp = FDateTime::Now().ToString(TEXT("%H:%M:%S.%s"));
    FString Line = FString::Printf(TEXT("[%s] %s"), *TimeStamp, *Text);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Line);
    UKismetSystemLibrary::PrintString(nullptr, Line, true, true, Color, 8.0f);
}

/*─────────────────────────────*/
/*        헬로우 테스트         */
/*─────────────────────────────*/
void UZuoBlueprintFunctionLibrary::PrintHelloZuoLab()
{
    ZuoPrint(TEXT("👋 Hello from ZuoLab!"), FLinearColor::Green);
}

/*─────────────────────────────*/
/*     Steam 로그인 기능       */
/*─────────────────────────────*/
bool UZuoBlueprintFunctionLibrary::LoginToSteam()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);

    if (!Subsystem)
    {
        ZuoPrint(TEXT("❌ Steam Subsystem not found!"), FLinearColor::Red);
        return false;
    }

    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (!Identity.IsValid())
    {
        ZuoPrint(TEXT("❌ Steam Identity interface invalid!"), FLinearColor::Red);
        return false;
    }

    // 이미 로그인되어 있으면
    if (Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
    {
        FString Name = Identity->GetPlayerNickname(0);
        ZuoPrint(FString::Printf(TEXT("✅ Already logged in as %s"), *Name), FLinearColor::Green);
        return true;
    }

    ZuoPrint(TEXT("🔹 Attempting Steam login..."), FLinearColor::Yellow);

    // Steam은 credentials 없이 자동 로그인
    FOnlineAccountCredentials Credentials;
    Identity->OnLoginCompleteDelegates->AddLambda(
        [](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
        {
            if (bWasSuccessful)
            {
                FString Nick = UserId.ToString();
                ZuoPrint(FString::Printf(TEXT("✅ Steam Login Success! ID: %s"), *Nick), FLinearColor::Green);
            }
            else
            {
                ZuoPrint(FString::Printf(TEXT("❌ Steam Login Failed: %s"), *Error), FLinearColor::Red);
            }
        }
    );

    Identity->Login(0, Credentials);
    return true;
}



void UZuoBlueprintFunctionLibrary::SendGameDataToServer(
    FString URL,
    FString Nickname,           // player1 → nickname
    int32 Item1, int32 Item2, int32 Item3, int32 Item4, int32 Item5,
    int32 Item6, int32 Item7, int32 Item8, int32 Item9, int32 Item10, // 10개 아이템
    int32 Lv, bool bIsWin,
    float PlayTime, int32 Kill,
    int32 Mode // 0 = 싱글, 1 = 멀티
)
{
    // 👇 단순 문자열 형태로 구성 (XML 아님)
    FString Body;
    Body += "<myname>" + Nickname + "</myname>\n";
    Body += "<myId>0</myId>\n";
    Body += "<gamedata>\n";
    Body += "    <nickname>" + Nickname + "</nickname>\n";
    Body += FString::Printf(TEXT("    <item1>%d</item1>\n"), Item1);
    Body += FString::Printf(TEXT("    <item2>%d</item2>\n"), Item2);
    Body += FString::Printf(TEXT("    <item3>%d</item3>\n"), Item3);
    Body += FString::Printf(TEXT("    <item4>%d</item4>\n"), Item4);
    Body += FString::Printf(TEXT("    <item5>%d</item5>\n"), Item5);
    Body += FString::Printf(TEXT("    <item6>%d</item6>\n"), Item6);
    Body += FString::Printf(TEXT("    <item7>%d</item7>\n"), Item7);
    Body += FString::Printf(TEXT("    <item8>%d</item8>\n"), Item8);
    Body += FString::Printf(TEXT("    <item9>%d</item9>\n"), Item9);
    Body += FString::Printf(TEXT("    <item10>%d</item10>\n"), Item10);
    Body += FString::Printf(TEXT("    <lv>%d</lv>\n"), Lv);
    Body += FString::Printf(TEXT("    <isWin>%s</isWin>\n"), bIsWin ? TEXT("True") : TEXT("False"));
    Body += FString::Printf(TEXT("    <playtime>%.2f</playtime>\n"), PlayTime);
    Body += FString::Printf(TEXT("    <kill>%d</kill>\n"), Kill);
    Body += FString::Printf(TEXT("    <mode>%d</mode>\n"), Mode);
    Body += "</gamedata>\n";

    // 🔍 디버그 출력
    ZuoPrint(Body, FLinearColor::Yellow);

    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->SetURL(URL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=utf-8")); // 단순 텍스트로 전송
    Request->SetContentAsString(Body);

    Request->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess)
        {
            if (!bSuccess || !Resp.IsValid())
            {
                ZuoPrint(TEXT("❌ HTTP Request Failed!"), FLinearColor::Red);
                return;
            }

            FString Result = Resp->GetContentAsString();
            ZuoPrint(FString::Printf(TEXT("✅ Response: %s"), *Result), FLinearColor::Green);
        }
    );

    Request->ProcessRequest();
}
