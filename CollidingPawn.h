// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class PRACTICE_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 파티클 시스템 컴포넌트를 추적하는 변수
	// 이것을 통해 별도의 추적 변수 없이 컴포넌트를 만들 수 있게 된다.
	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem;

	// Pawn Movement Component를 사용하기 위해서 먼저 폰 클래스를 추가하여 추적하게 해야 한다
	// 그것을 위한 부분이다.
	UPROPERTY()
	class UCollidingPawnMovementComponent* OurMovementComponent;

	// 폰이 GetMovementComponent 함수를 호출해야 한다
	// 폰이 이동을 가능하게 해주는 부분
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	// 폰이 받을 입력을 받는 함수
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();


};