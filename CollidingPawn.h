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

	// ��ƼŬ �ý��� ������Ʈ�� �����ϴ� ����
	// �̰��� ���� ������ ���� ���� ���� ������Ʈ�� ���� �� �ְ� �ȴ�.
	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem;

	// Pawn Movement Component�� ����ϱ� ���ؼ� ���� �� Ŭ������ �߰��Ͽ� �����ϰ� �ؾ� �Ѵ�
	// �װ��� ���� �κ��̴�.
	UPROPERTY()
	class UCollidingPawnMovementComponent* OurMovementComponent;

	// ���� GetMovementComponent �Լ��� ȣ���ؾ� �Ѵ�
	// ���� �̵��� �����ϰ� ���ִ� �κ�
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	// ���� ���� �Է��� �޴� �Լ�
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();


};