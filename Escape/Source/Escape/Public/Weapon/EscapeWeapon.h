// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeWeapon.generated.h"


UCLASS()
class ESCAPE_API AEscapeWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

	/**
	 * Called when an instance of this class is placed (in editor) or spawned.
	 * @param	Transform			The transform the actor was constructed at.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Overridable native event for when play begins for this actor. */
	virtual void BeginPlay() override;

	/**
	 *	Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	Note that Tick is disabled by default, and you will need to check PrimaryActorTick.bCanEverTick is set to true to enable it.
	 *
	 *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float DeltaTime) override;

	/** Returns Capsule subobject **/
	class UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	/** Returns Mesh subobject **/
	class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	/**
	 * Set MasterPoseComponent for this component
	 *
	 * @param NewMasterBoneComponent New MasterPoseComponent
	 */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetMasterPoseComponent(USkinnedMeshComponent* NewMasterBoneComponent, bool bForceUpdate = false);

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	FName CapsuleSolt;

private:
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
};
