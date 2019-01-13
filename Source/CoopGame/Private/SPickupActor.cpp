// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "SPowerupActor.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SphereComp->SetSphereRadius(75.0f);
    RootComponent = SphereComp;

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    DecalComp->DecalSize = FVector(64, 75, 75);
    DecalComp->SetupAttachment(RootComponent);

    CooldownDuration = 10.0f;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		Respawn();
	}
}

void ASPickupActor::Respawn()
{
    if (PowerupClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("POwerUpClass is nullptr in %s.Please update in blueprint"), *GetName())
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PowerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
}

void ASPickupActor::NotifyActorBeginOverlap(AActor *OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (Role == ROLE_Authority && PowerupInstance)
    {
        PowerupInstance->ActivatePowerup(OtherActor);
        PowerupInstance = nullptr;

        // Set timer for respawn
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, CooldownDuration);
    }
}