// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SprayBullet.generated.h"


UCLASS()
class A2CPP_API ASprayBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprayBullet();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;
	
	UPROPERTY()
	UStaticMesh* Asset;

	UPROPERTY(VisibleAnywhere, Category = ProjectileSettings)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = ProjectileSettings)
	FVector velocity = FVector(3000.f, 0.f, 0.f);

	UPROPERTY(VisibleAnywhere, Category = ProjectileSettings)
	float initialSpeed = 5000.f;

	UPROPERTY(VisibleAnywhere, Category = ProjectileSettings)
	float maxSpeed = 30000.f;

	UPROPERTY(EditAnywhere, Category = DecalParameters)
	UMaterialInterface* DecalMaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = DecalParameters)
	FVector DecalSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(VisibleAnywhere, Category = DecalParameters)
	float LifeSpan = 3.f;
	
	UPROPERTY()
	FRotator decalRotator;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	UMaterialInstance* primitiveComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
