// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LMABaseWeapon.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponMesh);
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon.Bullets = DefaultAmmoWeapon.Bullets;
}

void ALMABaseWeapon::Fire()
{
	Shoot();
}

void ALMABaseWeapon::Shoot()
{
	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocket);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	FVector TracerEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TracerEnd = HitResult.ImpactPoint;
	}
	SpawnTrace(TraceStart, TracerEnd);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);
	DecrementBullets();
}

void ALMABaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = DefaultAmmoWeapon.Bullets;
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;

	if (IsCurrentClipEmpty())
	{
		OnEmptyClip.Broadcast();
		ChangeClip();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == DefaultAmmoWeapon.Bullets;
}

void ALMABaseWeapon::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
	if (TraceFX)
	{
		TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
	}
}
