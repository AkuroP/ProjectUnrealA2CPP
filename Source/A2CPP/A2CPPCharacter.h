// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "A2CPPCharacter.generated.h"

UCLASS(config=Game)
class AA2CPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AA2CPPCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY()
	bool isPicked;

	UPROPERTY()
	AActor* pickedActor;

	UPROPERTY(EditAnywhere)
	int playerMaxLife;
	
	UPROPERTY(VisibleAnywhere)
	int playerLife;

	UPROPERTY(EditAnywhere, Category = Speed)
	float walkSpeed;
	
	UPROPERTY(EditAnywhere, Category = Speed)
	float runSpeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASprayBullet> bullet;

	UPROPERTY()
	int jumpCount;

	UPROPERTY(EditAnywhere)
	int maxJumpCount;

	UPROPERTY()
	float jumpHeight;
	
	UPROPERTY(EditAnywhere)
	class ARespawn* Respawn;

	/*UPROPERTY()
	FTransform* bulletTransform;*/

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Grab function
	UFUNCTION()
	void Interact();

	//fire paintball function
	UFUNCTION()
	void Fire();
	
	UFUNCTION()
	void OnStartRun();

	UFUNCTION()
	void OnStopRun();

	void BeginPlay() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void Landed(const FHitResult& Hit) override;
public:
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void PlayerHurt(int damage);
	UFUNCTION()
	void PlayerHeal(int heal);

	UFUNCTION()
	void MultipleJump();

	UFUNCTION()
	void Crouching();

	UFUNCTION()
	void Uncrouching();
};

