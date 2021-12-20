// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeWallLoading.generated.h"

UCLASS()
class A2CPP_API AMazeWallLoading : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeWallLoading();

	UPROPERTY(EditAnywhere)
	FName loadLevelName;

	UPROPERTY(EditAnywhere)
	FName unloadLevelName;

	UPROPERTY(EditAnywhere)
	bool Load;

	UPROPERTY(EditAnywhere)
	bool Unload;
protected:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* triggerBox;

	UPROPERTY()
	class AA2CPPCharacter* player;

	UPROPERTY()
	FLatentActionInfo LatentInfo;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
