// Copyright 2018 by January. All Rights Reserved.

#include "EscapeHUD_Game.h"
#include "EscapeCharacter.h"
#include "Kismet/GameplayStatics.h"


static FName HealthName(TEXT("HealthBar"));

AEscapeHUD_Game::AEscapeHUD_Game(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if !UE_SERVER 
#if UE_BUILD_DEVELOPMENT
	if (!IsNetMode(NM_DedicatedServer))
	{
#endif
		static ConstructorHelpers::FObjectFinder<UFont> FontObj(TEXT("/Game/Font/XMFF.XMFF"));
		DamageInfoFont = FontObj.Object;

		DamageFontRenderInfo.bEnableShadow = true;

		static ConstructorHelpers::FObjectFinder<UTexture2D> HealthTextureFinder(TEXT("/Game/UI/HUD/Health"));
		HealthTexture = HealthTextureFinder.Object;

		HealthBarBg = UCanvas::MakeIcon(HealthTexture, 0, 0, 64, 6);
		HealthBar = UCanvas::MakeIcon(HealthTexture, 0, 6, 64, 6);
		HealthIcon = UCanvas::MakeIcon(HealthTexture, 0, 12, 64, 6);
#if UE_BUILD_DEVELOPMENT
	}
#endif
#endif
}

void AEscapeHUD_Game::DrawHUD()
{
	Super::DrawHUD();

	DrawHealth();
	DrawDamageInfo();
}

void AEscapeHUD_Game::AddDamageInfo(float NewDamage, FVector NewHitLocation, bool bIsCrit)
{
	FDamageInfo NewDamageInfo;
	NewDamageInfo.Damage = NewDamage;
	NewDamageInfo.MakeTime = GetWorld()->TimeSeconds;
	NewDamageInfo.HitLocation = NewHitLocation;
	NewDamageInfo.bIsCrit = bIsCrit;

	DamageInfos.Add(NewDamageInfo);
}

void AEscapeHUD_Game::MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{
	if (Icon.Texture)
	{
		const float Width = Icon.Texture->GetSurfaceWidth();
		const float Height = Icon.Texture->GetSurfaceHeight();
		UV0 = FVector2D(U / Width, V / Height);
		UV1 = UV0 + FVector2D(UL / Width, VL / Height);
	}
}

void AEscapeHUD_Game::DrawHealth()
{
	if (HealthBar.Texture)
	{
		const float ScaleUI = 1.f;

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			AEscapeCharacter* Pawn = Cast<AEscapeCharacter>(*It);
			APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
			if (PlayerController && Pawn && Pawn->IsAlive() && Pawn->WasRecentlyRendered())
			{
				FVector ActorLocation = Pawn->GetMesh()->GetSocketLocation(HealthName);
				FVector2D ScreenPostion = FVector2D::ZeroVector;
				UGameplayStatics::ProjectWorldToScreen(PlayerController, ActorLocation, ScreenPostion);

				FVector2D DrawPostion(ScreenPostion.X - HealthBarBg.UL / 2, ScreenPostion.Y - HealthBarBg.VL / 2);

				Canvas->SetDrawColor(FColor::White);

				Canvas->DrawIcon(HealthBarBg, DrawPostion.X, DrawPostion.Y, ScaleUI);
				const float HealthAmount = FMath::Min(1.f, (float)Pawn->GetHealth() / (float)Pawn->GetMaxHealth());

				FCanvasTileItem TileItem(FVector2D(DrawPostion.X, DrawPostion.Y), HealthBar.Texture->Resource,
					FVector2D(HealthBar.UL * HealthAmount * ScaleUI, HealthBar.VL * ScaleUI), FLinearColor::White);
				MakeUV(HealthBar, TileItem.UV0, TileItem.UV1, HealthBar.U, HealthBar.V, HealthBar.UL * HealthAmount, HealthBar.VL);
				TileItem.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(TileItem);
			}
		}

	}
}

void AEscapeHUD_Game::DrawDamageInfo()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController != nullptr)
	{
		TArray<FDamageInfo>::TConstIterator It(DamageInfos);
		for (int32 Index = 0; It; ++Index, ++It)
		{
			const FDamageInfo& DamageInfo = *It;

			FVector2D ScreenPostion = FVector2D::ZeroVector;
			UGameplayStatics::ProjectWorldToScreen(PlayerController, DamageInfo.HitLocation, ScreenPostion);

			const float PastTime = GetWorld()->TimeSeconds - DamageInfo.MakeTime;

			FVector2D Offset = FVector2D::ZeroVector;
			if (PastTime < 0.4f)
			{
				Offset = FVector2D((PastTime / 0.4f) * 20.f, -(PastTime / 0.4f) * 100.f);
			}
			else if (PastTime >= 0.4f)
			{
				Offset = FVector2D(20.f, -100.f);
			}

			if (DamageInfoFont != nullptr)
			{
				if (DamageInfo.bIsCrit)
				{
					DamageInfoFont->LegacyFontSize = 24;

					Canvas->SetDrawColor(FColor::Red);
					Canvas->DrawText(DamageInfoFont, FString::Printf(TEXT("暴击 %d"), DamageInfo.Damage), ScreenPostion.X + Offset.X, ScreenPostion.Y + Offset.Y, 1.f, 1.f, DamageFontRenderInfo);
				}
				else
				{
					DamageInfoFont->LegacyFontSize = 20;

					Canvas->SetDrawColor(FColor::Yellow);
					Canvas->DrawText(DamageInfoFont, FString::Printf(TEXT("%d"), DamageInfo.Damage), ScreenPostion.X + Offset.X, ScreenPostion.Y + Offset.Y, 1.f, 1.f, DamageFontRenderInfo);
				}
			}

			if (PastTime > 0.5f)
			{
				DamageInfos.RemoveAt(Index);
			}
		}
	}
}

