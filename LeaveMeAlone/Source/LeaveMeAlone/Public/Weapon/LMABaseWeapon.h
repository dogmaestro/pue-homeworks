// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;
class ULMAWeaponComponent;
class USoundWave;
class UNiagaraSystem;


DECLARE_MULTICAST_DELEGATE(FOnEmptyClipSignature);

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()

public:

	ALMABaseWeapon();

	void Fire();

	void ChangeClip();

	FOnEmptyClipSignature OnEmptyClip;

	bool IsCurrentClipFull() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.1f;

	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }
	bool shooting = false;


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	void Shoot();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocket = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon DefaultAmmoWeapon{30, 0, true};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	USoundWave* ShootWave;

	void DecrementBullets();
	bool IsCurrentClipEmpty() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* TraceEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FString TraceName = "Tracer";

private:
	FAmmoWeapon CurrentAmmoWeapon;
	FTimerHandle FireTimerHandle;
	void SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd);

};
