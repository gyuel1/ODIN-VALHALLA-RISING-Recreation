// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ODCameraControllerComponent.generated.h"
UENUM(BlueprintType)
enum class ECamMode : uint8
{
	Q UMETA(DisplayName="Q"),
	N UMETA(DisplayName="N"),
	D UMETA(DisplayName="D"),
};

USTRUCT(BlueprintType)
struct FCamPreset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ArmLength = 300.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FOV       = 100.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator ArmRot = FRotator(0.f, -35.f, 0.f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector  Offset  = FVector(30.f, 0.f, 80.f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ZoomSpeed = 5000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ArmMin    = 230.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ArmMax    = 800.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PitchMin = -80.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PitchMax =  45.f;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OD_API UODCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	
	UODCameraControllerComponent();
	
	UFUNCTION(BlueprintCallable)
	void SetZoomAxis(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void NextPreset();
	UFUNCTION(BlueprintCallable)
	void HandleLooK(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable)
	void UsePreset(ECamMode Mode);
	UFUNCTION(BlueprintCallable)
	void UseQ();
	UFUNCTION(BlueprintCallable)
	void UseN();
	UFUNCTION(BlueprintCallable)
	void UseD();
	UFUNCTION(BlueprintPure, Category="Camera")
	ECamMode GetMode() const;
	UFUNCTION(BlueprintCallable)
	void UpdateFromMoveAxis(const FVector2D& Axis);
	UFUNCTION(BlueprintCallable)
	void SetMouseLookActive(bool bMouseLookDominant);
	FVector GetLookRightDir()const;
	FVector GetLookFWdDir()const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Presets")
	FCamPreset PresetQ;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Presets")
	FCamPreset PresetN;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Presets")
	FCamPreset PresetD;
private:
	UPROPERTY(VisibleAnywhere, Category="Presets")
	FCamPreset CurrentPreset;
	UPROPERTY()
	FVector CurrentTargetOffset;
	UPROPERTY()
	float CurrentTargetArmLength = 0.f;
	UPROPERTY()
	FRotator CurrentTargetArmRot;
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float InterpSpeed = 1.f;
	UPROPERTY(VisibleAnywhere, Category="Presets")
	ECamMode CurrentMode = ECamMode::N;
	//카메라 회전속도
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float AutoYawMaxDegPerSec = 10.f; //키보드 회전 속도
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float DeadzoneDeg = 2.f;
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float StrafeDominance = 0.35f;
		
	UPROPERTY(EditAnywhere,  Category = "Smoothing")
	float MouseRotationInterpSpeed = 10.0f;//마우스 회전속도
	FVector2D PreviousMouseInput = FVector2D::ZeroVector;
	bool bMouseLookActive;
		
	bool EnsureRig();

//자동사냥중 카메라 움직임
private:
	//자동사냥 중 카메라 복귀 시간
	FTimerHandle AutoHuntCameraTimer;
	//카메라 이동 중인지 확인
	bool IsMovingCamera = false;
	//카메라 이동 중인지 확인
	bool IsLerpingToNDPreset = false;
	//카메라 프리셋
	FCamPreset TargetAutoHuntPreset;
	//lerp시간 기록
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float LerpStartTime = 0.f;
	//보간 시간
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float LerpDuration = 2.0f;
	//카메라 자동 위치 조절 속도
	UPROPERTY(EditAnywhere, Category="Smoothing")
	float AutoHuntYawInterpSpeed = 1.0f;
	// 시작 값을 저장
	float StartArmLength = 0.f;
	FVector StartOffset = FVector::ZeroVector;
	float StartPitch = 0.f;
	//자동사냥 여부 저장
	bool IsAutoHuntActive = false;
	// 마우스 룩이 비활성화된 마지막 시간
	float LastMouseLookTime = -999.0f; 
	// Yaw 동기화를 시작할 딜레이 시간 (예: 2.0초)
	const float YawSyncDelay = 2.0f;
	

	void UpdateCameraLerp();

public:
	//자동사냥 연동
	void StartLerpToNDPreset();
	void StopLerpToNDPreset();
	void SetAutoHuntActive(bool bActive);
	
	
		
	
};
