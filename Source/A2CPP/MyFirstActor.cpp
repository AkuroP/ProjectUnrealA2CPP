// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstActor.h"
#include <A2CPP/A2CPPCharacter.h>

// Sets default values
AMyFirstActor::AMyFirstActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//RootComponent = SceneComponent;
	RootComponent = MeshComponent;
	//MeshComponent->SetupAttachment(SceneComponent);

	LetterCount = 10;
	count = 0;

	Object = NewObject<UObject>();
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
	Super::BeginPlay();
	PrintLetterFunction();
	MeshComponent->OnComponentHit.AddDynamic(this, &AMyFirstActor::OnHit);
}

// Called every frame
void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*GLog->Log(FString::FromInt(count));
	count += 1;*/
}

void AMyFirstActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	AA2CPPCharacter* Character = Cast<AA2CPPCharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	Character->LaunchCharacter(FVector(0, 0, 1000), false, true);
	GLog->Log("HITTED");
}

void AMyFirstActor::PrintLetterFunction()
{
	GLog->Log("START" + FString::FromInt(LetterCount));
}


