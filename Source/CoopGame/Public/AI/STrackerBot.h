// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class UStaticMeshComponent;
class UNavigationPath;
class USHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class USoundCue;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* OwningHealthComp,
						  float Health,
						  float HealthDelta,
						  const class UDamageType* DamageType,
						  class AController* InstigatedBy,
						  AActor* DamageCauser);

	void SelfDestruct();

    void DamageSelf();

	FVector GetNextPathPoint();

	// Next point in navigation path
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	// Dynamic on material  to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;

    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
    float SelfDamageInterval;

	FTimerHandle TimerHandle_SelfDamage;

	bool bStartedSelfDestruction;

    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* SelfDestructSound;
    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* ExplosionSound;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    void OnCheckNearbyBots();
	
	void RefreshPath();

    float PowerLevel;

	FTimerHandle TimerHandle_RefreshPath;

};
