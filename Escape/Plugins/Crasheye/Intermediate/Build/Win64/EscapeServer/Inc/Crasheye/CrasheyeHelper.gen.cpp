// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Crasheye/Private/CrasheyePrivatePCH.h"
#include "Crasheye/Public/CrasheyeHelper.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCrasheyeHelper() {}
// Cross Module References
	CRASHEYE_API UClass* Z_Construct_UClass_UCrasheyeHelper_NoRegister();
	CRASHEYE_API UClass* Z_Construct_UClass_UCrasheyeHelper();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	UPackage* Z_Construct_UPackage__Script_Crasheye();
	CRASHEYE_API UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb();
	CRASHEYE_API UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion();
	CRASHEYE_API UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier();
	CRASHEYE_API UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash();
// End Cross Module References
	void UCrasheyeHelper::StaticRegisterNativesUCrasheyeHelper()
	{
		UClass* Class = UCrasheyeHelper::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CrasheyeLeaveBreadcrumb", &UCrasheyeHelper::execCrasheyeLeaveBreadcrumb },
			{ "CrasheyeSetAppVersion", &UCrasheyeHelper::execCrasheyeSetAppVersion },
			{ "CrasheyeSetUserIdentifier", &UCrasheyeHelper::execCrasheyeSetUserIdentifier },
			{ "CrasheyeTestCrash", &UCrasheyeHelper::execCrasheyeTestCrash },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics
	{
		struct CrasheyeHelper_eventCrasheyeLeaveBreadcrumb_Parms
		{
			FString Breadcrumb;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Breadcrumb;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::NewProp_Breadcrumb = { UE4CodeGen_Private::EPropertyClass::Str, "Breadcrumb", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(CrasheyeHelper_eventCrasheyeLeaveBreadcrumb_Parms, Breadcrumb), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::NewProp_Breadcrumb,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::Function_MetaDataParams[] = {
		{ "Category", "Crasheye" },
		{ "ModuleRelativePath", "Public/CrasheyeHelper.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCrasheyeHelper, "CrasheyeLeaveBreadcrumb", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04022401, sizeof(CrasheyeHelper_eventCrasheyeLeaveBreadcrumb_Parms), Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::PropPointers), 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics
	{
		struct CrasheyeHelper_eventCrasheyeSetAppVersion_Parms
		{
			FString Version;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Version;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::NewProp_Version = { UE4CodeGen_Private::EPropertyClass::Str, "Version", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(CrasheyeHelper_eventCrasheyeSetAppVersion_Parms, Version), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::NewProp_Version,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::Function_MetaDataParams[] = {
		{ "Category", "Crasheye" },
		{ "ModuleRelativePath", "Public/CrasheyeHelper.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCrasheyeHelper, "CrasheyeSetAppVersion", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04022401, sizeof(CrasheyeHelper_eventCrasheyeSetAppVersion_Parms), Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::PropPointers), 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics
	{
		struct CrasheyeHelper_eventCrasheyeSetUserIdentifier_Parms
		{
			FString UserIdentifier;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_UserIdentifier;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::NewProp_UserIdentifier = { UE4CodeGen_Private::EPropertyClass::Str, "UserIdentifier", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000000080, 1, nullptr, STRUCT_OFFSET(CrasheyeHelper_eventCrasheyeSetUserIdentifier_Parms, UserIdentifier), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::NewProp_UserIdentifier,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::Function_MetaDataParams[] = {
		{ "Category", "Crasheye" },
		{ "ModuleRelativePath", "Public/CrasheyeHelper.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCrasheyeHelper, "CrasheyeSetUserIdentifier", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04022401, sizeof(CrasheyeHelper_eventCrasheyeSetUserIdentifier_Parms), Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::PropPointers), 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash_Statics::Function_MetaDataParams[] = {
		{ "Category", "Crasheye" },
		{ "ModuleRelativePath", "Public/CrasheyeHelper.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCrasheyeHelper, "CrasheyeTestCrash", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04022401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UCrasheyeHelper_NoRegister()
	{
		return UCrasheyeHelper::StaticClass();
	}
	struct Z_Construct_UClass_UCrasheyeHelper_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCrasheyeHelper_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_Crasheye,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UCrasheyeHelper_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UCrasheyeHelper_CrasheyeLeaveBreadcrumb, "CrasheyeLeaveBreadcrumb" }, // 2175867612
		{ &Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetAppVersion, "CrasheyeSetAppVersion" }, // 3989676366
		{ &Z_Construct_UFunction_UCrasheyeHelper_CrasheyeSetUserIdentifier, "CrasheyeSetUserIdentifier" }, // 4259721923
		{ &Z_Construct_UFunction_UCrasheyeHelper_CrasheyeTestCrash, "CrasheyeTestCrash" }, // 1435328603
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCrasheyeHelper_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "CrasheyeHelper.h" },
		{ "ModuleRelativePath", "Public/CrasheyeHelper.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCrasheyeHelper_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCrasheyeHelper>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UCrasheyeHelper_Statics::ClassParams = {
		&UCrasheyeHelper::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x001000A0u,
		FuncInfo, ARRAY_COUNT(FuncInfo),
		nullptr, 0,
		nullptr,
		&StaticCppClassTypeInfo,
		nullptr, 0,
		METADATA_PARAMS(Z_Construct_UClass_UCrasheyeHelper_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UCrasheyeHelper_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCrasheyeHelper()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UCrasheyeHelper_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UCrasheyeHelper, 2986794872);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UCrasheyeHelper(Z_Construct_UClass_UCrasheyeHelper, &UCrasheyeHelper::StaticClass, TEXT("/Script/Crasheye"), TEXT("UCrasheyeHelper"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCrasheyeHelper);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
