// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "EscapeItem.generated.h"

class UEscapeGameplayAbility;

/** Base class for all items, do not blueprint directly */
UCLASS(Abstract, BlueprintType)
class ESCAPE_API UEscapeItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Constructor */
	UEscapeItem()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
	{}

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	/** Icon to display */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	//FSlateBrush ItemIcon;

	/** Price in game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Price;

	/** Maximum number of instances that can be in inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxCount;

	/** Maximum level this item can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxLevel;

	/** Ability to grant if this item is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UEscapeGameplayAbility> GrantedAbility;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};


