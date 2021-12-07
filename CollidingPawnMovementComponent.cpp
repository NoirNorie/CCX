// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawnMovementComponent.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFuncion)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFuncion);

	// ��� ���� ��ȿ�ϰ� �����̴� ���� ����Ѵٴ� ���� Ȯ���ϴ� ����
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// ACollidingPawn::Tick���� ������ �Է� ���͸� �޾ƿ´�.
	/*
		�� �ڵ�� ���� ������ �ε巴�� ǥ�� ���� ������ �� �ֵ��� �ϴ� �ڵ�
		- �ϴ� ���� �߷��� ������� �ʴ´�
		- �ְ� �ӷ��� �𸮾� ������ �ʴ� 150 ���ָ�ŭ �̵��ϵ��� �ϵ� �ڵ���
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

	/*  TickComponent �Լ��� �� ���� �پ��� ��ɵ��� �����ȴ�(UPawnMovementComponent Ŭ�����κ��� ������)
		- ConsumeInputVector		: ����� �Է� ������ �޾Ƶ鿩 ������ �� �ִ�
		- SafeMoveUpdatedComponent  : �𸮾� ������ ������ �̿��Ͽ� ���� �ܴ��� �����ΰ��� ��� ������ ������ �� �ֵ��� �Ѵ�
		- SlideAlongSurface			: ��� ó�� �� ���������� �ε巴�� ǥ�� ���� ������ �� �ְ� �Ѵ� (�� ���� ������ �����ϰ� �����)
		- �� �ۿ� �� ���� ��ɵ��� �����Ѵ�

	*/
}