using UnrealBuildTool;
using System.IO;

public class SkyStudio : ModuleRules
{
    public SkyStudio(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemSteam",
            "HTTP",          // ✅ HTTP 요청 모듈
            "Json",          // ✅ (선택) 나중에 JSON도 쓸 거면 같이 추가
            "JsonUtilities"  // ✅ (선택) JSON 직렬화용
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Steam SDK v157
            string SteamPath = Path.Combine(
                EngineDirectory,
                "Binaries", "ThirdParty", "Steamworks", "Steamv157", "Win64"
            );

            string SteamDLL = Path.Combine(SteamPath, "steam_api64.dll");
            string SteamLIB = Path.Combine(SteamPath, "steam_api64.lib");

            PublicAdditionalLibraries.Add(SteamLIB);
            RuntimeDependencies.Add(SteamDLL, StagedFileType.NonUFS);
        }
    }
}
