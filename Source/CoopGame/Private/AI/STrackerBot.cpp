// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "SCharacter.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
    MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCanEverAffectNavigation(false);

	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	ExplosionRadius = 200;
	ExplosionDamage = 40;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SelfDamageInterval = 0.25f;
}


// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Role == ROLE_Authority && !bExploded)
    {
        float DistanceTarget = (GetActorLocation() - NextPathPoint).Size();

        if (DistanceTarget <= RequiredDistanceToTarget)
        {
            NextPathPoint = GetNextPathPoint();
            DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
        }
        else {
            // Keep moving towards next target
            FVector ForceDirection = NextPathPoint - GetActorLocation();
            ForceDirection.Normalize();
            ForceDirection *= MovementForce;

            MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
            DrawDebugDirectionalArrow(GetWorld(),
                                      GetActorLocation(),
                                      GetActorLocation() + ForceDirection,
                                      32, FColor::Yellow,
                                      false, 0.0f, 0, 1.0f);
        }
        DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
    }
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
    {
        FVector NextPathPoint = GetNextPathPoint();
    }

    // Every second checking for nearby bots and updating power-level
    FTimerHandle TimeHandle_CheckPowerLevel;
	GetWorldTimerManager().SetTimer(TimeHandle_CheckPowerLevel, this, &ASTrackerBot::OnCheckNearbyBots, 1.0f, true);
}

FVector ASTrackerBot::GetNextPathPoint()
{
	// Hack, to get player location
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this,GetActorLocation(), PlayerPawn);

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		// Return next point in the path
		return NavPath->PathPoints[1];
	}
	// Failed to find path
	return GetActorLocation();
}

void ASTrackerBot::SelfDestruct()
{
    if (bExploded)
    { return; }
    bExploded = true;

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

    UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

    MeshComp->SetVisibility(false, true);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (Role == ROLE_Authority)
    {
        TArray<AActor *> IgnoredActors;
        IgnoredActors.Add(this);

        // Increases damage based on power level
        float ActualDamage = ExplosionDamage + (ExplosionDamage * PowerLevel);

        UGameplayStatics::ApplyRadialDamage(this, ActualDamage, GetActorLocation(), ExplosionRadius, nullptr,
                                            IgnoredActors, this, GetInstigatorController(),
                                            true);
        DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
        // Delete actor immediately
        SetLifeSpan(2.0f);
    }
}


void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
                                    const class UDamageType* DamageType, class AController* InstigatedBy,
                                    AActor* DamageCauser)
{
    // Explode on hitpoints == 0

    if (MatInst == nullptr)
    {
        MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
    }
    if (MatInst)
    {
        MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
    }
    if (Health <= 0.0f)
    {  SelfDestruct(); }

    UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName())
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (!bStartedSelfDestruction && !bExploded)
    {
        ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
        if (PlayerPawn)
        {
            // We overlapped with player!
            if (Role == ROLE_Authority)
            {
                GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf,
                                                SelfDamageInterval, true, 0.0f);
            }
            bStartedSelfDestruction = true;
            UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
        }
    }
}

void ASTrackerBot::DamageSelf()
{
    UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::OnCheckNearbyBots()
{
    // Radius to check nearby bots
    const float Radius = 600;

    // Create temporary collision shape for overlaps
    FCollisionShape CollShape;
    CollShape.SetSphere(Radius);

    FCollisionObjectQueryParams QueryParams;
    QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
    QueryParams.AddObjectTypesToQuery(ECC_Pawn);

    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::White, false, 1.0f);

    int32 NoOfBots = 0;
    for (FOverlapResult Result : Overlaps)
    {
        // Check if we overlapped with other trackerbots
        ASTrackerBot* Bot = Cast<ASTrackerBot>(Result.GetActor());
        // Ignore |this| bot
        if (Bot && Bot != this)
        {  NoOfBots++; }
    }

    const int32 MaxPowerLevel = 4;
    // Clamp between min = 0 & max = 4
    PowerLevel = FMath::Clamp(NoOfBots, 0, MaxPowerLevel);

    // Update material color
    if (MatInst == nullptr)
    {
        MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
    }
    if (MatInst)
    {
        float Alpha = PowerLevel /(float) MaxPowerLevel;
        MatInst->SetScalarParameterValue("PowerLevelAlpha", Alpha);
    }
    // Draw string on bot location
    DrawDebugString(GetWorld(), FVector(0, 0, 0), FString::FromInt(PowerLevel), this, FColor::White, 1.0f, true);
}
