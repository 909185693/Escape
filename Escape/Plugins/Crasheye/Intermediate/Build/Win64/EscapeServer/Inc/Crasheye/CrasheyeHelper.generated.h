// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef CRASHEYE_CrasheyeHelper_generated_h
#error "CrasheyeHelper.generated.h already included, missing '#pragma once' in CrasheyeHelper.h"
#endif
#define CRASHEYE_CrasheyeHelper_generated_h

#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execCrasheyeTestCrash) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeTestCrash(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCrasheyeLeaveBreadcrumb) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Breadcrumb); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeLeaveBreadcrumb(Z_Param_Breadcrumb); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCrasheyeSetAppVersion) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Version); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeSetAppVersion(Z_Param_Version); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCrasheyeSetUserIdentifier) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_UserIdentifier); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeSetUserIdentifier(Z_Param_UserIdentifier); \
		P_NATIVE_END; \
	}


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execCrasheyeTestCrash) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeTestCrash(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCrasheyeLeaveBreadcrumb) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Breadcrumb); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeLeaveBreadcrumb(Z_Param_Breadcrumb); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCrasheyeSetAppVersion) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Version); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeSetAppVersion(Z_Param_Version); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCrasheyeSetUserIdentifier) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_UserIdentifier); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UCrasheyeHelper::CrasheyeSetUserIdentifier(Z_Param_UserIdentifier); \
		P_NATIVE_END; \
	}


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCrasheyeHelper(); \
	friend struct Z_Construct_UClass_UCrasheyeHelper_Statics; \
public: \
	DECLARE_CLASS(UCrasheyeHelper, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Crasheye"), NO_API) \
	DECLARE_SERIALIZER(UCrasheyeHelper)


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_INCLASS \
private: \
	static void StaticRegisterNativesUCrasheyeHelper(); \
	friend struct Z_Construct_UClass_UCrasheyeHelper_Statics; \
public: \
	DECLARE_CLASS(UCrasheyeHelper, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Crasheye"), NO_API) \
	DECLARE_SERIALIZER(UCrasheyeHelper)


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCrasheyeHelper(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCrasheyeHelper) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCrasheyeHelper); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCrasheyeHelper); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UCrasheyeHelper(UCrasheyeHelper&&); \
	NO_API UCrasheyeHelper(const UCrasheyeHelper&); \
public:


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCrasheyeHelper(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UCrasheyeHelper(UCrasheyeHelper&&); \
	NO_API UCrasheyeHelper(const UCrasheyeHelper&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCrasheyeHelper); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCrasheyeHelper); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCrasheyeHelper)


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_PRIVATE_PROPERTY_OFFSET
#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_11_PROLOG
#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_PRIVATE_PROPERTY_OFFSET \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_RPC_WRAPPERS \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_INCLASS \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_PRIVATE_PROPERTY_OFFSET \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_INCLASS_NO_PURE_DECLS \
	Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h_14_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class CrasheyeHelper."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Escape_Plugins_Crasheye_Source_Crasheye_Public_CrasheyeHelper_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
