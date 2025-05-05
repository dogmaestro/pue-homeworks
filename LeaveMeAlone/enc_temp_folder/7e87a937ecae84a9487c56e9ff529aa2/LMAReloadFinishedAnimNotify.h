// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Delegates/DelegateCombinations.h"
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LMAReloadFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyReloadFinishedSignature, USkeletalMeshComponent*);

UCLASS()
class LEAVEMEALONE_API ULMAReloadFinishedAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnNotifyReloadFinishedSignature OnNotifyReloadFinished;
};