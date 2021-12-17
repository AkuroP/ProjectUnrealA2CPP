// Fill out your copyright notice in the Description page of Project Settings.


#include "HealOrHit.h"
#include "A2CPPCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHealOrHit::AHealOrHit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//box trigger
	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("triggerBox"));
	triggerBox->SetGenerateOverlapEvents(true);
	//visuel
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AHealOrHit::BeginPlay()
{
	Super::BeginPlay();
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHealOrHit::OnBeginOverlap);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AHealOrHit::OnEndOverlap);

}

// Called every frame
void AHealOrHit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (triggered)
	{
		//GLog->Log(FString::FromInt(tickCount));
		tickCount += 1;
		if (isHeal)
		{
			if (tickCount >= 60)
			{
				player->PlayerHeal(1);
				//GLog->Log("RESET TIMER");
				GLog->Log(FString::FromInt(player->playerLife));
				tickCount = 0;
			}
		}
		else
		{
			if (tickCount >= 60)
			{
				player->PlayerHurt(1);
				tickCount = 0;
				//GLog->Log("RESET TIMER");
				GLog->Log(FString::FromInt(player->playerLife));

			}
		}
	}
}

void AHealOrHit::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<AA2CPPCharacter>(OtherActor);
	
	if (player == nullptr)
	{
		return;
	}
	else
	{
		triggered = true;
	}
}

void AHealOrHit::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GLog->Log("END");
	triggered = false;
	tickCount = 0;
}

