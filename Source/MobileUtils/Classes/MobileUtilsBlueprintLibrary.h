// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "MobileUtilsBlueprintLibrary.generated.h"


UCLASS()
class MOBILEUTILS_API UMobileUtilsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static bool CheckInternetConnection();

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static bool CheckGooglePlayServices();

	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static FString GetPersistentUniqueDeviceId();


	/**
	* Start intent to select image.
	*
	*
	*/
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
	static void SelectImage();

	/**
	* Return the path to the image that the user selected.
	*
	* @return - full path to the image user selected in gallery prompt.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static FString GetImagePath();

	UFUNCTION(BlueprintCallable, Category = MobileUtils)
	static UTexture2D* GetImgTexture(float& Height, float& Width);
};
