// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CppBaseActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CPPBASE_API ACppBaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACppBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SinMovement();

	UFUNCTION(BlueprintCallable)
	FVector GetInitialLocation();

	UFUNCTION(BlueprintCallable)
	void SetInitialLocation(FVector location);

	UPROPERTY(EditInstanceOnly)
	float Amplitude = 1.0;

	UPROPERTY(EditInstanceOnly)
	float Frequency = 2.0;

	UPROPERTY(VisibleAnywhere)
	FVector InitialLocation;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
};