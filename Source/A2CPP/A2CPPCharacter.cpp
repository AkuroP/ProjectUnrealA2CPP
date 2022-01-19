// Copyright Epic Games, Inc. All Rights Reserved.

#include "A2CPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "DrawDebugHelpers.h"
#include "SprayBullet.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Respawn.h"


//////////////////////////////////////////////////////////////////////////
// AA2CPPCharacter

AA2CPPCharacter::AA2CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	walkSpeed = 600.f;
	runSpeed = 900.f;
	playerMaxLife = 100.f;
	playerLife = 100.f;
	jumpHeight = 500.f;
}

void AA2CPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	jumpCount = maxJumpCount;
}

void AA2CPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA2CPPCharacter::Respawn(AA2CPPCharacter* player)
{
	respawn->RespawnPlayer(player);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AA2CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AA2CPPCharacter::MultipleJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AA2CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AA2CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AA2CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AA2CPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AA2CPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AA2CPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AA2CPPCharacter::OnResetVR);

	// Custom key bindings
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AA2CPPCharacter::Interact);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AA2CPPCharacter::Fire);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AA2CPPCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AA2CPPCharacter::OnStopRun);
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AA2CPPCharacter::Crouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AA2CPPCharacter::Uncrouching);

}


void AA2CPPCharacter::OnResetVR()
{
	// If A2CPP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in A2CPP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AA2CPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AA2CPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AA2CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AA2CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AA2CPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AA2CPPCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AA2CPPCharacter::Crouching()
{
	Crouch();
}

void AA2CPPCharacter::Uncrouching()
{
	UnCrouch();
}

void AA2CPPCharacter::Interact()
{
	//Ce que retourne le hit
	FHitResult OutHit;
	FHitResult pickedObject;
	//Debut du Trace
	FVector TraceStartLocation = this->GetActorLocation();
	//Distance du Trace
	FVector TraceFowardVector = FollowCamera->GetForwardVector() + this->GetActorForwardVector();
	//Fin du Tracce
	FVector TraceEndLocation = ((TraceFowardVector * 1000.f) + TraceStartLocation);
	//Parametre du trace, ici ignore le player
	FCollisionQueryParams TraceParameters;
	TraceParameters.AddIgnoredActor(this);
	//Ligne qui detecte des objets
	DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Red, false);

	//Au premier objet detecte
	if (GetWorld()->LineTraceSingleByChannel(OutHit, TraceStartLocation, TraceEndLocation, ECollisionChannel::ECC_PhysicsBody, TraceParameters))
	{
		//Si le joueur n'a pas grab, grab l'objet
		if (!isPicked)
		{
			//Si l'objet detecte as une collision et qu'il est movable
			if (OutHit.bBlockingHit && OutHit.GetComponent()->Mobility == EComponentMobility::Movable)
			{
				//TEST / LOG
				//GLog->Log(OutHit.Actor->GetActorLabel());
				//FAttachmentTransformRules AttachTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
				//OutHit.Actor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				//OutHit.Actor->SetActorTransform(this->GetTransform());
				//GLog->Log("Picked");
				
				//Enleve la physique de l'objet
				OutHit.Component->SetSimulatePhysics(false);
				//Attach l'objet au joueur
				OutHit.Actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				//Deplace l'objet vers le haut
				OutHit.Actor->SetActorLocation(OutHit.Actor->GetActorLocation() + FVector(0.f, 0.f, 80.f));
				pickedActor = OutHit.GetActor();
				isPicked = true;
			}

		}
		//Sinon drop l'objet
		else
		{
			GLog->Log(OutHit.Actor->GetActorLabel());
			UPrimitiveComponent* yes = Cast<UPrimitiveComponent>(pickedActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			yes->SetSimulatePhysics(true);
			pickedActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			//GLog->Log("Released");
			isPicked = false;
		}

	}
}

void AA2CPPCharacter::Fire()
{
	//GLog->Log("FIRE !");
	//Debut du Trace
	UWorld* playerLocation = this->GetWorld();
	//Direction de la balle
	FVector bulletForwardVector = this->GetRootComponent()->GetComponentLocation() + (this->GetActorForwardVector() * 110);
	
	playerLocation->SpawnActor<ASprayBullet>(bullet, bulletForwardVector, this->GetActorRotation());
}

void AA2CPPCharacter::OnStartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void AA2CPPCharacter::OnStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}



void AA2CPPCharacter::PlayerHurt(float damage)
{
	playerLife -= damage;
}

void AA2CPPCharacter::PlayerHeal(float heal)
{
	playerLife += heal;
}

void AA2CPPCharacter::PlayerLevelUp(int level)
{
	playerLevel += level;
}

void AA2CPPCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	jumpCount = maxJumpCount;
}

void AA2CPPCharacter::MultipleJump()
{
	if (jumpCount > 0)
	{
		ACharacter::LaunchCharacter(FVector(0.f, 0.f, jumpHeight), false, true);
		jumpCount--;
	}
}