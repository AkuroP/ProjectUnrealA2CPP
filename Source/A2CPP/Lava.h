// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lava.generated.h"

UCLASS()
class A2CPP_API ALava : public AActor
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    ALava();
    UPROPERTY(EditAnywhere)
    int velocity;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* MyComp;

    

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
