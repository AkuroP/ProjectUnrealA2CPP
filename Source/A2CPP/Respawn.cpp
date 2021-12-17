// Fill out your copyright notice in the Description page of Project Settings.


#include "Respawn.h"
#include "A2CPPCharacter.h"
#include "GameFramework/GameModeBase.h"
// Sets default values
ARespawn::ARespawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ARespawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARespawn::RespawnPlayer(AA2CPPCharacter* &player)
{
	GetWorld()->GetAuthGameMode()->RestartPlayer(player->GetController());
	player->SetActorLocation(this->GetActorLocation());
	player->playerLife = player->playerMaxLife;
}

