// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeWallLoading.h"
#include "Components/BoxComponent.h"
#include "A2CPPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMazeWallLoading::AMazeWallLoading()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("loadBox"));
	triggerBox->SetGenerateOverlapEvents(true);
	RootComponent = triggerBox;
}

// Called when the game starts or when spawned
void AMazeWallLoading::BeginPlay()
{
	Super::BeginPlay();
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMazeWallLoading::OnBeginOverlap);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AMazeWallLoading::OnEndOverlap);
}

// Called every frame
void AMazeWallLoading::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeWallLoading::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<AA2CPPCharacter>(OtherActor);
	if (player == nullptr)
	{
		return;
	}
	
	if (Load)
	{
		UGameplayStatics::LoadStreamLevel(this, loadLevelName, true, false, LatentInfo);
	}

}

void AMazeWallLoading::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	player = Cast<AA2CPPCharacter>(OtherActor);
	if (player == NULL)
	{
		return;
	}
	if (Unload)
	{
		UGameplayStatics::UnloadStreamLevel(this, unloadLevelName, LatentInfo, false);
	}
}

