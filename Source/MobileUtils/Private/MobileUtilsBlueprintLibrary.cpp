// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#include "MobileUtilsPrivatePCH.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


UMobileUtilsBlueprintLibrary::UMobileUtilsBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UMobileUtilsBlueprintLibrary::CheckInternetConnection()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->CheckInternetConnection();
#else
	return true;
#endif
}

bool UMobileUtilsBlueprintLibrary::CheckGooglePlayServices()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->CheckGooglePlayServices();
#else
	return false;
#endif
}

FString UMobileUtilsBlueprintLibrary::GetPersistentUniqueDeviceId()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->GetPersistentUniqueDeviceId();
#else
	return UKismetSystemLibrary::GetUniqueDeviceId();
#endif
}

//	REMOVE BEFORE PULL REQUEST
//	My stuff.

void UMobileUtilsBlueprintLibrary::SelectImage()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->SelectImage();
#else
	return;
#endif
}

FString UMobileUtilsBlueprintLibrary::GetImagePath() 
{
	#if PLATFORM_ANDROID || PLATFORM_IOS
		return IMobileUtils::Get().GetPlatformInterface()->GetImagePath();
	#else
		return FString("");
	#endif
}

UTexture2D* UMobileUtilsBlueprintLibrary::GetImgTexture(float& Height, float& Width)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->GetImgTexture(Height, Width);
#else
	return NULL;
#endif
}

