// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2016 gameDNA studio. All Rights Reserved.

#include "MobileUtilsPrivatePCH.h"
#include "MobileUtilsPlatform.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


jmethodID FMobileUtilsPlatform::CheckInternetConnectionMethod;
jmethodID FMobileUtilsPlatform::CheckGooglePlayServicesMethod;
jmethodID FMobileUtilsPlatform::GetPersistentUniqueDeviceIdMethod;
jmethodID FMobileUtilsPlatform::SelectImageMethod;
jmethodID FMobileUtilsPlatform::GetImagePathMethod;
jstring ImageFilePath;

FMobileUtilsPlatform::FMobileUtilsPlatform()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		CheckInternetConnectionMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckInternetConnection", "()Z", false);
		CheckGooglePlayServicesMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckGooglePlayServices", "()Z", false);
		GetPersistentUniqueDeviceIdMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetPersistentUniqueDeviceId", "()Ljava/lang/String;", false);
		SelectImageMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SelectImage", "()V", false);
		GetImagePathMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetImagePath", "()Ljava/lang/String;", false);
	}
}

FMobileUtilsPlatform::~FMobileUtilsPlatform()
{
}

bool FMobileUtilsPlatform::CheckInternetConnection()
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::CheckInternetConnectionMethod);
	}
	return bResult;
}

bool FMobileUtilsPlatform::CheckGooglePlayServices()
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::CheckGooglePlayServicesMethod);
	}
	return bResult;
}

FString FMobileUtilsPlatform::GetPersistentUniqueDeviceId()
{
	FString ResultDeviceId = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetPersistentUniqueDeviceIdMethod);
		const char *nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		ResultDeviceId = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
	}
	return ResultDeviceId;
}

void FMobileUtilsPlatform::SelectImage()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::SelectImageMethod);
	}
}

FString FMobileUtilsPlatform::GetImagePath() 
{
	FString ResultFilePath = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultFilePathString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetImagePathMethod);
		const char* nativeFilePathString = Env->GetStringUTFChars(ResultFilePathString, 0);
		ResultFilePath = FString(nativeFilePathString);
		Env->ReleaseStringUTFChars(ResultFilePathString, nativeFilePathString);
		Env->DeleteLocalRef(ResultFilePathString);
	}
	return ResultFilePath;
}

EImageFormat::Type FMobileUtilsPlatform::GetImageType(FString InPath)
{
	FString lhs;
	FString Extension;
	InPath.Split(FString("."), &lhs, &Extension, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	Extension = Extension.ToLower();
	if (Extension == "png")
		return EImageFormat::PNG;
	if (Extension == "jpeg" || Extension == "jpg")
		return EImageFormat::JPEG;
	if (Extension == "bmp")
		return EImageFormat::BMP;
	return EImageFormat::Invalid;
}

UTexture2D* FMobileUtilsPlatform::GetImgTexture(float& Height, float& Width)
{
	// I would be nothing without RaMa
	FString ResultFilePath = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultFilePathString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::GetImagePathMethod);
		const char* nativeFilePathString = Env->GetStringUTFChars(ResultFilePathString, 0);
		ResultFilePath = FString(nativeFilePathString);
		Env->ReleaseStringUTFChars(ResultFilePathString, nativeFilePathString);
		Env->DeleteLocalRef(ResultFilePathString);
		
		UTexture2D* LoadedTex = NULL;

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		EImageFormat::Type ImageFormat = GetImageType(ResultFilePath);
		IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
		TArray<uint8> RawFileData;
		FString ExternalFilePath = GetRootPath() + ResultFilePath;
		if (!FFileHelper::LoadFileToArray(RawFileData, *ExternalFilePath))
		{
			return NULL;
		}


		//Create T2D!
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			const TArray<uint8>* UncompressedBGRA = NULL;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				Height = ImageWrapper->GetHeight();
				Width = ImageWrapper->GetWidth();
				LoadedTex = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

				//Valid?
				if (!LoadedTex)
				{
					return NULL;
				}

				//Copy!
				void* TextureData = LoadedTex->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
				LoadedTex->PlatformData->Mips[0].BulkData.Unlock();

				//Update!
				LoadedTex->UpdateResource();
			}
		}

		// Success!
		return LoadedTex;
	
	}
	return NULL;
	
}

FString FMobileUtilsPlatform::GetRootPath()
{
	FString PathStr = "";
#if PLATFORM_ANDROID
	TArray<FString> Folders;
	GExternalFilePath.ParseIntoArray(Folders, TEXT("/"));
	for (FString Folder : Folders)
	{
		PathStr += FString("/..");
	}

#endif
	return PathStr;
}


// this is the start of developing a delegate system to elimnate the static global in GameActivity.java
extern "C" void Java_com_epicgames_ue4_GameActivity_nativeImageFilePath(jstring filepath)
{
	ImageFilePath = filepath;
}