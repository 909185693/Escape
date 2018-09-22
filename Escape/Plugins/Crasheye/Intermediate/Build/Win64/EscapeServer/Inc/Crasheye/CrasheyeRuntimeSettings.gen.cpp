// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Crasheye/Private/CrasheyePrivatePCH.h"
#include "Crasheye/Public/CrasheyeRuntimeSettings.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCrasheyeRuntimeSettings() {}
// Cross Module References
	CRASHEYE_API UClass* Z_Construct_UClass_UCrasheyeRuntimeSettings_NoRegister();
	CRASHEYE_API UClass* Z_Construct_UClass_UCrasheyeRuntimeSettings();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_Crasheye();
// End Cross Module References
	void UCrasheyeRuntimeSettings::StaticRegisterNativesUCrasheyeRuntimeSettings()
	{
	}
	UClass* Z_Construct_UClass_UCrasheyeRuntimeSettings_NoRegister()
	{
		return UCrasheyeRuntimeSettings::StaticClass();
	}
	struct Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_IOS_AppKey_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_IOS_AppKey;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_IOS_bEnableCrasheye_MetaData[];
#endif
		static void NewProp_IOS_bEnableCrasheye_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_IOS_bEnableCrasheye;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Andoird_AppKey_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Andoird_AppKey;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Andoird_bEnableCrasheye_MetaData[];
#endif
		static void NewProp_Andoird_bEnableCrasheye_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Andoird_bEnableCrasheye;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_Crasheye,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "CrasheyeRuntimeSettings.h" },
		{ "ModuleRelativePath", "Public/CrasheyeRuntimeSettings.h" },
		{ "ToolTip", "Implements the settings for the Crasheye plugin." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_AppKey_MetaData[] = {
		{ "Category", "Crasheye|IOS" },
		{ "ModuleRelativePath", "Public/CrasheyeRuntimeSettings.h" },
		{ "ToolTip", "Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_AppKey = { UE4CodeGen_Private::EPropertyClass::Str, "IOS_AppKey", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UCrasheyeRuntimeSettings, IOS_AppKey), METADATA_PARAMS(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_AppKey_MetaData, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_AppKey_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye_MetaData[] = {
		{ "Category", "Crasheye|IOS" },
		{ "ModuleRelativePath", "Public/CrasheyeRuntimeSettings.h" },
		{ "ToolTip", "Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to" },
	};
#endif
	void Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye_SetBit(void* Obj)
	{
		((UCrasheyeRuntimeSettings*)Obj)->IOS_bEnableCrasheye = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye = { UE4CodeGen_Private::EPropertyClass::Bool, "IOS_bEnableCrasheye", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000004001, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(UCrasheyeRuntimeSettings), &Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye_SetBit, METADATA_PARAMS(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye_MetaData, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_AppKey_MetaData[] = {
		{ "Category", "Crasheye|Android" },
		{ "ModuleRelativePath", "Public/CrasheyeRuntimeSettings.h" },
		{ "ToolTip", "Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_AppKey = { UE4CodeGen_Private::EPropertyClass::Str, "Andoird_AppKey", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UCrasheyeRuntimeSettings, Andoird_AppKey), METADATA_PARAMS(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_AppKey_MetaData, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_AppKey_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye_MetaData[] = {
		{ "Category", "Crasheye|Android" },
		{ "ModuleRelativePath", "Public/CrasheyeRuntimeSettings.h" },
		{ "ToolTip", "Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to" },
	};
#endif
	void Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye_SetBit(void* Obj)
	{
		((UCrasheyeRuntimeSettings*)Obj)->Andoird_bEnableCrasheye = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye = { UE4CodeGen_Private::EPropertyClass::Bool, "Andoird_bEnableCrasheye", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000004001, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(UCrasheyeRuntimeSettings), &Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye_SetBit, METADATA_PARAMS(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye_MetaData, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_AppKey,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_IOS_bEnableCrasheye,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_AppKey,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::NewProp_Andoird_bEnableCrasheye,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCrasheyeRuntimeSettings>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::ClassParams = {
		&UCrasheyeRuntimeSettings::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x001000A6u,
		nullptr, 0,
		Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::PropPointers),
		"Engine",
		&StaticCppClassTypeInfo,
		nullptr, 0,
		METADATA_PARAMS(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCrasheyeRuntimeSettings()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UCrasheyeRuntimeSettings_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UCrasheyeRuntimeSettings, 2344116813);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UCrasheyeRuntimeSettings(Z_Construct_UClass_UCrasheyeRuntimeSettings, &UCrasheyeRuntimeSettings::StaticClass, TEXT("/Script/Crasheye"), TEXT("UCrasheyeRuntimeSettings"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCrasheyeRuntimeSettings);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
