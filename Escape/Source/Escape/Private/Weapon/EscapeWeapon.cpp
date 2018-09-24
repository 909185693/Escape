// Copyright 2018 by January. All Rights Reserved.

#include "EscapeWeapon.h"


AEscapeWeapon::AEscapeWeapon(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh->AlwaysLoadOnClient = true;
	Mesh->AlwaysLoadOnServer = true;
	Mesh->bOwnerNoSee = false;
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCanEverAffectNavigation(false);
	SetRootComponent(Mesh);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	CapsuleComponent->InitCapsuleSize(10.f, 84.f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->bCheckAsyncSceneOnMove = false;
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	CapsuleComponent->SetupAttachment(Mesh);

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;
}

void AEscapeWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CapsuleComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, CapsuleSolt);
}

void AEscapeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsNetMode(NM_DedicatedServer))
	{
		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	}
	else
	{
		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	}
}

void AEscapeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CapsuleComponent != nullptr)
	{
		CapsuleComponent->UpdateComponentToWorld();
	}
}

void AEscapeWeapon::SetMasterPoseComponent(USkinnedMeshComponent* NewMasterBoneComponent, bool bForceUpdate)
{
	Mesh->SetMasterPoseComponent(NewMasterBoneComponent, bForceUpdate);
}

