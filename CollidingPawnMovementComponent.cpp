// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovementComponent.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFuncion)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFuncion);

	// 모든 것이 유효하고 움직이는 것을 허용한다는 것을 확인하는 구문
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// ACollidingPawn::Tick에서 설정한 입력 벡터를 받아온다.
	/*
		이 코드는 폰이 공간을 부드럽게 표면 위를 움직일 수 있도록 하는 코드
		- 일단 폰에 중력이 적용되지 않는다
		- 최고 속력을 언리얼 단위로 초당 150 유닛만큼 이동하도록 하드 코드함
	*/
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}

	/*  TickComponent 함수는 몇 가지 다양한 기능들이 제공된다(UPawnMovementComponent 클래스로부터 제공됨)
		- ConsumeInputVector		: 사용할 입력 정보를 받아들여 저장할 수 있다
		- SafeMoveUpdatedComponent  : 언리얼 엔진의 물리를 이용하여 폰이 단단한 무엇인가에 닿기 전까지 움직일 수 있도록 한다
		- SlideAlongSurface			: 계산 처리 및 물리적으로 부드럽게 표면 상을 움직일 수 있게 한다 (벽 등을 만나면 간단하게 멈춘다)
		- 이 밖에 더 많은 기능들이 존재한다

	*/
}