// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/EscapeHUD.h"
#include "EscapeHUD_Game.generated.h"


USTRUCT()
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Damage;

	UPROPERTY()
	FVector HitLocation;

	UPROPERTY()
	float MakeTime;

	UPROPERTY()
	uint32 bIsCrit : 1;
};

UCLASS()
class ESCAPE_API AEscapeHUD_Game : public AEscapeHUD
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	void AddDamageInfo(float NewDamage, FVector NewHitLocation, bool bIsCrit);

protected:
	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);

	/** Texture for Health. */
	UPROPERTY()
	UTexture2D* HealthTexture;

	/** Health bar background icon. */
	UPROPERTY()
	FCanvasIcon HealthBarBg;

	/** Health bar icon. */
	UPROPERTY()
	FCanvasIcon HealthBar;

	/** Health icon on the health bar. */
	UPROPERTY()
	FCanvasIcon HealthIcon;

	UPROPERTY(EditDefaultsOnly, Category = Font)
	class UFont* DamageInfoFont;

	UPROPERTY(EditDefaultsOnly, Category = Font)
	struct FFontRenderInfo DamageFontRenderInfo;

	void DrawHealth();
	
	TArray<FDamageInfo> DamageInfos;

	void DrawDamageInfo();
	
};
