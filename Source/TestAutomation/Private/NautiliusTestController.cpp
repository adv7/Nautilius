// Fill out your copyright notice in the Description page of Project Settings.


#include "NautiliusTestController.h"
#include "ProfilingDebugging/CsvProfiler.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Async/Async.h"

void UNautiliusTestController::OnPostMapChange(UWorld* World)
{
    UE_LOG(LogGauntlet, Display, TEXT("NautiliusTestController started"));

    FTimerHandle dummy;
    GetWorld()->GetTimerManager().SetTimer(dummy, this, &UNautiliusTestController::StartTesting, SpinUpTime, false);
}

void UNautiliusTestController::StartTesting()
{
    //TODO: this is where you put your custom game code that should be run before profiling starts

    StartProfiling();
}

void UNautiliusTestController::StartProfiling()
{
    FCsvProfiler::Get()->BeginCapture();

    // set a timer for when profiling should end
    FTimerHandle dummy;
    GetWorld()->GetTimerManager().SetTimer(dummy, this, &UNautiliusTestController::StopProfiling, ProfilingTime, false);
}

void UNautiliusTestController::StopProfiling()
{
    UE_LOG(LogGauntlet, Display, TEXT("Stopping the profiler"));

    TSharedFuture<FString> future = FCsvProfiler::Get()->EndCapture();

    // launch an async task that polls the Future for completion
    // will in turn launch a task on the game thread once the CSV file is saved to disk
    AsyncTask(ENamedThreads::AnyThread, [this, future]()
        {
            while (!future.IsReady())
                FPlatformProcess::SleepNoStats(0);

            AsyncTask(ENamedThreads::GameThread, [this]()
                {
                    StopTesting();
                }
            );
        }
    );
}

void UNautiliusTestController::OnTick(float DeltaTime)
{
    //TODO: this is where you can put stuff that should happen on tick
}

void UNautiliusTestController::StopTesting()
{
    UE_LOG(LogGauntlet, Display, TEXT("YourGameGauntletController stopped"));
    EndTest(0);
}