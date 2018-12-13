// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CoopGame/Public/SProjectileWeapon.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSProjectileWeapon() {}
// Cross Module References
	COOPGAME_API UClass* Z_Construct_UClass_ASProjectileWeapon_NoRegister();
	COOPGAME_API UClass* Z_Construct_UClass_ASProjectileWeapon();
	COOPGAME_API UClass* Z_Construct_UClass_ASWeapon();
	UPackage* Z_Construct_UPackage__Script_CoopGame();
// End Cross Module References
	void ASProjectileWeapon::StaticRegisterNativesASProjectileWeapon()
	{
	}
	UClass* Z_Construct_UClass_ASProjectileWeapon_NoRegister()
	{
		return ASProjectileWeapon::StaticClass();
	}
	struct Z_Construct_UClass_ASProjectileWeapon_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASProjectileWeapon_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ASWeapon,
		(UObject* (*)())Z_Construct_UPackage__Script_CoopGame,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASProjectileWeapon_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "SProjectileWeapon.h" },
		{ "ModuleRelativePath", "Public/SProjectileWeapon.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASProjectileWeapon_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASProjectileWeapon>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ASProjectileWeapon_Statics::ClassParams = {
		&ASProjectileWeapon::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x009000A0u,
		nullptr, 0,
		nullptr, 0,
		nullptr,
		&StaticCppClassTypeInfo,
		nullptr, 0,
		METADATA_PARAMS(Z_Construct_UClass_ASProjectileWeapon_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_ASProjectileWeapon_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASProjectileWeapon()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ASProjectileWeapon_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ASProjectileWeapon, 1268995125);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASProjectileWeapon(Z_Construct_UClass_ASProjectileWeapon, &ASProjectileWeapon::StaticClass, TEXT("/Script/CoopGame"), TEXT("ASProjectileWeapon"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASProjectileWeapon);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
