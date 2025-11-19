// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODCameraControllerComponent.h"

#include "Character/ODPlayer.h"
#include "Player/ODPlayerController.h"


// Sets default values for this component's properties
UODCameraControllerComponent::UODCameraControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	//카메라 시작 세팅
	PresetQ.ArmLength = 320.f;
	PresetQ.FOV = 100.f;
	PresetQ.ArmRot = FRotator(0.f, 0.f, 100.f);
	PresetQ.Offset = FVector(-50.f, 0.f, 100.f);
	PresetQ.ZoomSpeed = 5000.f;
	PresetQ.ArmMin = 250.f;
	PresetQ.ArmMax = 750.f;

	PresetN.ArmLength = 320.f;
	PresetN.FOV = 100.f;
	PresetN.ArmRot = FRotator(0.f, -25.f, 0.f);
	PresetN.Offset = FVector(50.f, 0.f, 100.f);
	PresetN.ZoomSpeed = 5000.f;
	PresetN.ArmMin = 150.f;
	PresetN.ArmMax = 700.f;
	PresetN.PitchMin = -60.f;
	PresetN.PitchMax = 20.f;

	PresetD.ArmLength = 150.f;
	PresetD.FOV = 100.f;
	PresetD.ArmRot = FRotator(0.f, 0.f, 0.f);
	PresetD.Offset = FVector(-50.f, 20.f, 100.f);
	PresetD.ZoomSpeed = 0.f;
	PresetD.ArmMin = 0.f;
	PresetD.ArmMax = 400.f;
	PresetD.PitchMin = -50.f;
	PresetD.PitchMax = 10.f;

	SetIsReplicatedByDefault(true);
	
}

void UODCameraControllerComponent::SetZoomAxis(float AxisValue)
{
	if (!SpringArm || FMath::IsNearlyZero(AxisValue) || CurrentPreset.ZoomSpeed <= 0.f)
	{
		return;
	}
	const float Delta = AxisValue * CurrentPreset.ZoomSpeed * GetWorld()->GetDeltaSeconds();

   
	CurrentTargetArmLength = FMath::Clamp(CurrentTargetArmLength - Delta,
		FMath::Max(0.f, CurrentPreset.ArmMin),
		FMath::Max(CurrentPreset.ArmMin, CurrentPreset.ArmMax));

	// Q값만 변환
	if (CurrentMode == ECamMode::Q)
	{
		constexpr  float Pitch_50 = 300.f;
		const float Pitch_0 = CurrentPreset.ArmMin;
		const float Alpha = FMath::GetMappedRangeValueClamped(
			FVector2D(Pitch_0, Pitch_50),
			FVector2D(0.0f, 1.0f),
			CurrentTargetArmLength
		);
		const float NewPitch = FMath::Lerp(0.0f, -50.0f, Alpha);
		
		if (APawn* Pawn = GetOwner<APawn>())
		{
			if (AODPlayerController* Controller = Cast<AODPlayerController>(Pawn->GetController()))
			{
				FRotator CameraControlRotation = Controller->GetControlRotation();
				CameraControlRotation.Pitch = NewPitch;
				Controller->SetControlRotation(CameraControlRotation);
			}
		}
	}
}

bool UODCameraControllerComponent::EnsureRig()
{
	//카메라 생성및 부착
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}
	SpringArm = NewObject<USpringArmComponent>(Owner, USpringArmComponent::StaticClass(), NAME_None);
	SpringArm->SetupAttachment(Owner->GetRootComponent());
	SpringArm->RegisterComponent();
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 0.f;

	Camera = NewObject<UCameraComponent>(Owner, UCameraComponent::StaticClass(), NAME_None);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->RegisterComponent();
	Camera->bUsePawnControlRotation = true;
    
	return SpringArm && Camera;
}

void UODCameraControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	EnsureRig();
	UsePreset(CurrentMode);
	TargetAutoHuntPreset = PresetN;
	
}

void UODCameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	  
	if (!SpringArm) 
	{
		return;
	}
	if (IsLerpingToNDPreset)
	{
		return;
	}
	
	//보간처리(자동사냥중이 아닌 일반 프리셋 변경일때)
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CurrentTargetArmLength, DeltaTime, InterpSpeed);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, CurrentTargetOffset, DeltaTime, InterpSpeed);
	
	if (IsAutoHuntActive && (CurrentMode == ECamMode::N || CurrentMode == ECamMode::D))
	{
		// 딜레이 체크(마우스 회전을 땐시점을 찾기위해)
		bool bDelayElapsed = GetWorld()->GetTimeSeconds() > (LastMouseLookTime + YawSyncDelay);
        
		// 자동사냥 중 & 마우스 조작X & 딜레이 시간이 경과
		if (!bMouseLookActive && bDelayElapsed)
		{
			if (APawn* Pawn = GetOwner<APawn>())
			{
				if (AODPlayerController* Controller = Cast<AODPlayerController>(Pawn->GetController()))
				{
					FRotator CurrentControlRotation = Controller->GetControlRotation();
       
					
					FRotator TargetRotation = CurrentControlRotation;
					TargetRotation.Yaw = Pawn->GetActorRotation().Yaw; 
					
					FRotator Rotation = FMath::RInterpTo(
						CurrentControlRotation, 
						TargetRotation, 
						DeltaTime, 
						AutoHuntYawInterpSpeed//회전 속도 설정
					);
					CurrentControlRotation.Yaw = Rotation.Yaw;
                  
					Controller->SetControlRotation(CurrentControlRotation);
				}
			}
		}
	}
	
}

void UODCameraControllerComponent::NextPreset()
{
	switch (CurrentMode)
	{
		case ECamMode::N: UseQ(); break;
		case ECamMode::Q: UseD(); break;
		case ECamMode::D:
		default: UseN(); break;
	}
}

void UODCameraControllerComponent::HandleLooK(const FInputActionValue& InputActionValue)
{
	const FVector2D InputVal = InputActionValue.Get<FVector2D>();
	if (IsLerpingToNDPreset)
	{
		return;
	}
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		// 보간된 입력값으로 컨트롤러 회전 적용
		OwnerPawn->AddControllerYawInput(InputVal.X*MouseRotationInterpSpeed); 
		if (CurrentMode != ECamMode::Q)
		{
			OwnerPawn->AddControllerPitchInput(-InputVal.Y*MouseRotationInterpSpeed);
		}
	}

}


void UODCameraControllerComponent::UsePreset(ECamMode Mode)
{
	CurrentMode = Mode;

	const FCamPreset* Preset = nullptr;
	switch (Mode)
	{
	case ECamMode::Q:
		Preset = &PresetQ;
		break;
	case ECamMode::N:
		Preset = &PresetN;
		break;
	case ECamMode::D:
		Preset = &PresetD;
		break;
	}
	if (!Preset)
	{
		return;
	}
	CurrentPreset = *Preset;        
	CurrentTargetArmLength = CurrentPreset.ArmLength;
	CurrentTargetOffset = CurrentPreset.Offset;
        
	if (Camera)
	{
		Camera->SetFieldOfView(CurrentPreset.FOV);
	}

	//카메라 위치 설정
	if (APawn* Pawn = GetOwner<APawn>())
	{
		if (AODPlayerController* Controller =Cast<AODPlayerController>( Pawn->GetController()))
		{
			FRotator CameraControlRotation = Controller->GetControlRotation();
			const float ActorYaw = Pawn ? Pawn->GetActorRotation().Yaw : 0.f;
			CameraControlRotation.Yaw = CurrentPreset.ArmRot.Yaw; CameraControlRotation.Pitch = CurrentPreset.ArmRot.Pitch;
			if (Mode == ECamMode::D || Mode == ECamMode::N)
			{
				CameraControlRotation.Yaw = FRotator::NormalizeAxis(ActorYaw);
			}
			Controller->SetControlRotation(CameraControlRotation);
		}
	}
	
}

void UODCameraControllerComponent::UseQ()
{
	UsePreset(ECamMode::Q);
}

void UODCameraControllerComponent::UseN()
{
	UsePreset(ECamMode::N);
}

void UODCameraControllerComponent::UseD()
{
	UsePreset(ECamMode::D);
}

ECamMode UODCameraControllerComponent::GetMode() const
{
	return CurrentMode; 
}

void UODCameraControllerComponent::UpdateFromMoveAxis(const FVector2D& Axis)
{
	if (!(CurrentMode == ECamMode::N || CurrentMode == ECamMode::D))
	{
		return;
	}
	if (bMouseLookActive) return;
	if (IsLerpingToNDPreset)
	{
		return;
	}
	if (Axis.IsNearlyZero())
	{
		return;
	}

	APawn* Pawn = GetOwner<APawn>(); 
	if (!Pawn)
	{
		return;
	}

	AODPlayerController* ODPC = Cast<AODPlayerController>(Pawn->GetController());
	if (!ODPC)
	{
		return;
	}
	if (IsAutoHuntActive)
	{
		return;
	}

	FRotator ODCtrlRot = ODPC->GetControlRotation();
	if (CurrentMode == ECamMode::N || CurrentMode == ECamMode::D)
	{
		FVector2D Input2D = FVector2D::ZeroVector;
		//이동 입력
		const FVector Last = Pawn->GetLastMovementInputVector();  
		
		const FVector Pending = Pawn->GetPendingMovementInputVector();

		if (!Last.IsNearlyZero())
			Input2D = FVector2D(Last.X, Last.Y);
		else if (!Pending.IsNearlyZero())
			Input2D = FVector2D(Pending.X, Pending.Y);

		if (!Input2D.IsNearlyZero())
		{
			const FRotator ViewYawOnly(0.f, ODCtrlRot.Yaw, 0.f);
			const FVector ViewRight3 = FRotationMatrix(ViewYawOnly).GetScaledAxis(EAxis::Y).GetSafeNormal2D();
			
			const FVector2D ViewRight(ViewRight3.X, ViewRight3.Y);
			const FVector2D InputDir2D = Input2D.GetSafeNormal();
			
			const float strafeDot = FVector2D::DotProduct(InputDir2D, ViewRight); //횡이동 하고있는지 판단
			//카메라 회전
			if (FMath::Abs(strafeDot) > StrafeDominance)
			{
				const float TargetYaw =
					(strafeDot > 0.f) ? FRotator::NormalizeAxis(ODCtrlRot.Yaw + 90.f)
									  : FRotator::NormalizeAxis(ODCtrlRot.Yaw - 90.f);
				float CurYaw   = FRotator::NormalizeAxis(ODCtrlRot.Yaw);
				const float Dolly  = FMath::FindDeltaAngleDegrees(CurYaw, TargetYaw);

				if (FMath::Abs(Dolly) > DeadzoneDeg)
				{
					const float step = AutoYawMaxDegPerSec * GetWorld()->GetDeltaSeconds();
					const float add  = FMath::Clamp(Dolly, -step, step);
					CurYaw = FRotator::NormalizeAxis(CurYaw + add);
				}
				else
				{
					CurYaw = TargetYaw;
				}

				FRotator Final = ODCtrlRot;
				Final.Yaw   = CurYaw;
				Final.Pitch = FMath::Clamp(FRotator::NormalizeAxis(Final.Pitch), CurrentPreset.PitchMin, CurrentPreset.PitchMax);
				Final.Roll  = 0.f;

				ODPC->SetControlRotation(Final);
				//UE_LOG(LogTemp, Warning, TEXT("키보드 회전"));
			}
		
		}
	}
}

void UODCameraControllerComponent::SetMouseLookActive(bool bMouseLookDominant)
{
	if (bMouseLookActive && !bMouseLookDominant)
	{
		//마우스를 뗀 순간의 현재 게임 시간을 기록
		LastMouseLookTime = GetWorld()->GetTimeSeconds();
	}
	bMouseLookActive = bMouseLookDominant;

}

FVector UODCameraControllerComponent::GetLookRightDir() const
{	
	return Camera->GetRightVector();
}

FVector UODCameraControllerComponent::GetLookFWdDir() const
{
	return Camera->GetForwardVector();
}

void UODCameraControllerComponent::UpdateCameraLerp()
{
	if (!SpringArm || !IsLerpingToNDPreset)
	{
		StopLerpToNDPreset();
		return;
	}
	//경과 시간 보간 계산
	float TimeElapsed = GetWorld()->GetTimeSeconds() - LerpStartTime;
	if (TimeElapsed < 0.f)
	{
		TimeElapsed = 0.f;
	}
	float Alpha = FMath::Clamp(TimeElapsed / LerpDuration, 0.f, 1.f);
	
    if (Alpha <= 0.0f) 
    {
       if (SpringArm)
       {
          // 시작값 설정
          SpringArm->TargetArmLength = StartArmLength;
          SpringArm->SocketOffset = StartOffset;
       }
       
       return;
    }    
    // 보간적용
    SpringArm->TargetArmLength = FMath::Lerp(StartArmLength, TargetAutoHuntPreset.ArmLength, Alpha);
    SpringArm->SocketOffset = FMath::Lerp(StartOffset, TargetAutoHuntPreset.Offset, Alpha);

    // 보간 완료
    if (Alpha >= 1.f )
    {
       // 최종 값으로 강제 설정
       SpringArm->TargetArmLength = TargetAutoHuntPreset.ArmLength;
       SpringArm->SocketOffset = TargetAutoHuntPreset.Offset;
     
		//보간이 끝나면 타이머 종료
       StopLerpToNDPreset();
    }
	
	
}

void UODCameraControllerComponent::StartLerpToNDPreset()
{
	if (CurrentMode == ECamMode::Q)
	{
		return;
	}

	if (IsLerpingToNDPreset)
	{
		return;
	}
	if (CurrentMode == ECamMode::N)
	{
		TargetAutoHuntPreset = PresetN;
	}
	else if (CurrentMode == ECamMode::D)
	{
		TargetAutoHuntPreset = PresetD;
	}
	else
	{
		return;
	}
	if (SpringArm)
	{
		StartArmLength = SpringArm->TargetArmLength;
		StartOffset = SpringArm->SocketOffset;
	}

	IsLerpingToNDPreset = true;
	LerpStartTime = GetWorld()->GetTimeSeconds();
	//카메라 보간 적용을 위한 타이머
	    
	GetWorld()->GetTimerManager().SetTimer(
	   AutoHuntCameraTimer,
	   this,
	   &UODCameraControllerComponent::UpdateCameraLerp,
	   0.016f,
	   true
	   );
	
}

void UODCameraControllerComponent::StopLerpToNDPreset()
{
	if (!IsLerpingToNDPreset)
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(AutoHuntCameraTimer);
	IsLerpingToNDPreset = false;
	
	CurrentTargetArmLength = TargetAutoHuntPreset.ArmLength;
	CurrentTargetOffset = TargetAutoHuntPreset.Offset;
}

void UODCameraControllerComponent::SetAutoHuntActive(bool bActive)
{
	IsAutoHuntActive = bActive;
}


