#include "TestAutomation.h"

#define LOCTEXT_NAMESPACE "FTestAutomationModule"

DEFINE_LOG_CATEGORY(LogTestAutomation);

void FTestAutomationModule::StartupModule()
{

}

void FTestAutomationModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTestAutomationModule, TestAutomation)