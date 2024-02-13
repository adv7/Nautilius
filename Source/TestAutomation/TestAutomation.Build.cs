using UnrealBuildTool;

public class TestAutomation : ModuleRules
{
    public TestAutomation(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[] 
            { 
                "Core", 
                "CoreUObject", 
                "Engine" 
            }
        );
    }
}