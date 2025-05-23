// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/LMAWeaponComponent.h"
#include "Player/LMADefaultCharacter.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::SpawnWeapon()
{
	if (GetWorld())
	{
		Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);

		if (Weapon)
		{
			const auto Character = Cast<ACharacter>(GetOwner());
			if (Character)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
				Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocket);
			}
			Weapon->OnEmptyClip.AddUObject(this, &ULMAWeaponComponent::OnCurrentClipEmpty);
		}
	}
}

void ULMAWeaponComponent::Fire()
{
	if (Weapon->IsValidLowLevel() && !AnimReloading)
	{
		Weapon->Fire();
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ULMAWeaponComponent::Fire, Weapon->FireRate);
	}
}

void ULMAWeaponComponent::Reload()
{
	if (!CanReaload())
		return;
	Weapon->ChangeClip();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}

void ULMAWeaponComponent::StopFire()
{
	if (Weapon->IsValidLowLevel())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
		return true;
	}
	return false;
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyRealoadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyRealoadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReaload() const
{
	return !AnimReloading && !Weapon->IsCurrentClipFull();
}

void ULMAWeaponComponent::OnCurrentClipEmpty()
{
	StopFire();
	Reload();
}
