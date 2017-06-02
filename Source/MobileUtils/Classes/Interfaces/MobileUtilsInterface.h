// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#pragma once

#include "Runtime/Engine/Classes/Engine/Texture2D.h"


class IMobileUtilsInterface
{
public:
	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	virtual bool CheckInternetConnection() = 0;

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	virtual bool CheckGooglePlayServices() = 0;

	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	virtual FString GetPersistentUniqueDeviceId();

	/**
	* Prompt user to select an image from gallery.
	*
	*
	*/
	virtual void SelectImage();

	/**
	* Return the path to user's selected image.
	*
	* @return - The full file path of the selected image.
	*/
	virtual FString GetImagePath();

	virtual UTexture2D* GetImgTexture(float& Height, float& Width);
};
