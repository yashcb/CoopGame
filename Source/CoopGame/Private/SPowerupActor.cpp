// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
    PowerupInterval = 0.0f;
    TotalNrOfTicks = 0;

	SetReplicates(true);
	bIsPowerupActive = false;
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupActor::OnTickPowerup()
{
    TickProcessed++;

    OnPowerupTicked();
    if (TickProcessed >= TotalNrOfTicks)
    {
        OnExpired();
		bIsPowerupActive = false;
		OnRep_PowerupActive();

        GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
    }
}

void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void ASPowerupActor::ActivatePowerup(AActor* ActiveFor)
{
    OnActivated(ActiveFor);
	bIsPowerupActive = true;
	OnRep_PowerupActive();

    if (PowerupInterval > 0.0f)
    {
        GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval,
                                        true);
    }
    else{  OnTickPowerup(); }
}

void  ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}