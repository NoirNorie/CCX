// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class PRACTICE_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

	UPROPERTY(VisibleAnyWhere) UStaticMeshComponent* VisualMesh;
	// 스태틱 메시 컴포넌트를 선언함
	// UPROPERTY 매크로는 언리얼 엔진 내부에서 보일 수 있게 만들어주는 매크로

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
