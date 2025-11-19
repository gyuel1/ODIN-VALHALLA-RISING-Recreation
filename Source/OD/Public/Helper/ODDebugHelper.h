// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// OD_LOG_SERVER 에서 사용
#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT")) : \
(GetNetMode() == ENetMode::NM_DedicatedServer) ? *FString::Printf(TEXT("DEDICATED")) : \
(GetNetMode() == ENetMode::NM_ListenServer) ? *FString::Printf(TEXT("LISTEN")) : \
(GetNetMode() == ENetMode::NM_Standalone) ? *FString::Printf(TEXT("STAND")) : \
(GetNetMode() == ENetMode::NM_MAX) ? *FString::Printf(TEXT("MAX")) :\
*FString::Printf(TEXT("NONE")))

//NetMode Debug Log NOParam
#define OD_LOG_SERVER(LogCht, Verbosity, Format, ...) UE_LOG(LogCht,Verbosity,TEXT("[%s] %s"),LOG_NETMODEINFO,*FString::Printf(Format, ##__VA_ARGS__));

// OD_LOG_SERVER_ONEPARAM 에서 사용
#define LOG_NETMODEINFO_ONEPARAM(NetMode) ((NetMode == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT")) : \
(NetMode == ENetMode::NM_DedicatedServer) ? *FString::Printf(TEXT("DEDICATED")) : \
(NetMode == ENetMode::NM_ListenServer) ? *FString::Printf(TEXT("LISTEN")) : \
(NetMode == ENetMode::NM_Standalone) ? *FString::Printf(TEXT("STAND")) : \
(NetMode == ENetMode::NM_MAX) ? *FString::Printf(TEXT("MAX")) : \
*FString::Printf(TEXT("NONE")))

//NetMode Debug Log NOParam
//첫번쨰 인자로 ENetMode 값을 넣어줘야함.
//GetNetMode를 사용할수 없는 곳에서 사용 EX) ENetMode NetMode = GetOwner()->GetNetMode())
#define OD_LOG_SERVER_ONEPARAM(NetMode,LogCht, Verbosity, Format, ...) UE_LOG(LogCht,Verbosity,TEXT("[%s] %s"),LOG_NETMODEINFO_ONEPARAM(NetMode),*FString::Printf(Format, ##__VA_ARGS__));

namespace Debug
{
	static void Print(const FString& Msg, float ViewportTime = 3.0f, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, ViewportTime, InColor, Msg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
		}
	}
}
