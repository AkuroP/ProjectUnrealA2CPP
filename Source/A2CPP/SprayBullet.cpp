// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayBullet.h"
#include "WallActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"


// Sets default values
ASprayBullet::ASprayBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Asset = Mesh.Object;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->Velocity = velocity;
	ProjectileMovement->InitialSpeed = initialSpeed;
	ProjectileMovement->MaxSpeed = maxSpeed;
	MeshComponent->SetStaticMesh(Asset);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionProfileName(FName("PhysicsActor"));
	DecalMaterialInstance = CreateDefaultSubobject<UMaterialInstance>(TEXT("Decal"));
}

// Called when the game starts or when spawned
void ASprayBullet::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASprayBullet::OnBeginOverlap);
	//primitiveComponent->SetMaterialByName(FName("SaberAlterDecal"), DecalMaterialInstance);
	//DecalComponent
}

// Called every frame
void ASprayBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASprayBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWallActor* Actor = Cast<AWallActor>(OtherActor);
	//GLog->Log(OtherActor->GetName());
	if (OtherActor->Tags.Num() != 0 && Actor != nullptr)
	{
		for (int i = 0; i < OtherActor->Tags.Num(); i++)
		{
			if (OtherActor->Tags[i] == FName("PaintWall"))
			{
				decalRotator = UKismetMathLibrary::NormalizedDeltaRotator(this->GetActorRotation(), FRotator(0.f, 90.f, 90.f));
				ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(this->GetActorLocation(), decalRotator);
				if (decal)
				{
					decal->SetDecalMaterial(DecalMaterialInstance);
					decal->GetDecal()->DecalSize = DecalSize;
					decal->SetLifeSpan(LifeSpan);
				}
				this->Destroy();
				GLog->Log("YES");
			}
			else
			{
				GLog->Log("NO");
			}
		}
	}
	else
	{
		GLog->Log("NAY");
		return;
	}
}
