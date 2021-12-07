// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "CollidingPawnMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 0: 어떠한 컴포넌트가 계층에서 루트 컴포넌트가 될 것인가를 결정한다
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	// 1: 이번 연습에서는 구체 컴포넌트를 루트 컴포넌트로 만든다
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	// 2: 스태틱 메시 에셋을 사용하여 보이는 구체를 생성한다(반지름 50 짜리 구체)
	// - 시험삼아 크기 80%의 스케일, z축으로 -40 이동한 구체를 만든다
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	/* 에셋의 경로를 하드코딩하는 것은 일반적으로 최선의 방법은 아니라고 함
		- 주로 생성자를 통해 컴포넌트를 생성하고 수정할 수 있게 만듦
		- 만든 것으로 블루프린트 에셋을 만들어 에디터에서 스태틱 메시를 배치할 수 있도록 만드는 식으로 하는 것이 일반적이라고 한다 

		- 이렇게도 할 수 있으며 프로그래머 입장에서는 디버깅이나 새 기능을 추가하는 것은 이 방법이 더 빠르다고 함
	 */

	// 읽어오는데 성공한지를 판단한다
	// 실패 시 이 구문이 동작하지 않게 되어 오류를 막는다
	if (SphereVisualAsset.Succeeded()) 
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object); // 물체의 모양은 구
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f)); // Z 축으로 -40 만큼 이동
		SphereVisual->SetWorldScale3D(FVector(0.8f)); // 스케일은 0.8(= 80%)
	}

	// 3: 비활성화된 파티클 시스템 컴포넌트를 계층 구조에 붙인다.
	// 컴포넌트의 활성/비활성을 코드에서 제어할 수 있다.
	// 파티클 시스템 컴포넌트를 스태틱 메시 컴포넌트에 붙인다(루트 컴포넌트에 붙이는 것이 아니다
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// 4: 스프링 암 컴포넌트를 생성한다
	// 카메라가 단단한 물체를 뚫고 보는 것을 방지하는 내장 기능이 존재한다
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// 5: 실제 카메라 컴포넌트를 생성한다
	// 카메라 컴포넌트 만드는 것은 쉽다(이 경우에서는 특별한 세팅이 필요 없다)
	// 스프링 암 컴포넌트에 내장된 소켓이 존재하여 연결하는 것이 쉬움
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// 6: 생성하고 부착한 컴포넌트들에 대해서 이 폰이 디폴트 플레이어에게 컨트롤 될 것이라고 지정한다
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 새 폰에 위의 컴포넌트들을 부착하는 것이 완료되었고 사용자의 컨트롤을 받을 수 있게 되었다.
	// 이후는 언리얼 에디터에서 수행한다.


	// 0: 입력 처리
	// 1: Pawn Movement Component를 생성하고 폰과 연결한다
	OurMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;




}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 이벤트와 함수들을 바인딩해주는 부분
	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ParticleToggle);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);
	


}

// 입력
UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}
// 입력 처리
void ACollidingPawn::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ACollidingPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}