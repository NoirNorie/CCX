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

	// 0: ��� ������Ʈ�� �������� ��Ʈ ������Ʈ�� �� ���ΰ��� �����Ѵ�
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	// 1: �̹� ���������� ��ü ������Ʈ�� ��Ʈ ������Ʈ�� �����
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	// 2: ����ƽ �޽� ������ ����Ͽ� ���̴� ��ü�� �����Ѵ�(������ 50 ¥�� ��ü)
	// - ������ ũ�� 80%�� ������, z������ -40 �̵��� ��ü�� �����
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	/* ������ ��θ� �ϵ��ڵ��ϴ� ���� �Ϲ������� �ּ��� ����� �ƴ϶�� ��
		- �ַ� �����ڸ� ���� ������Ʈ�� �����ϰ� ������ �� �ְ� ����
		- ���� ������ �������Ʈ ������ ����� �����Ϳ��� ����ƽ �޽ø� ��ġ�� �� �ֵ��� ����� ������ �ϴ� ���� �Ϲ����̶�� �Ѵ� 

		- �̷��Ե� �� �� ������ ���α׷��� ���忡���� ������̳� �� ����� �߰��ϴ� ���� �� ����� �� �����ٰ� ��
	 */

	// �о���µ� ���������� �Ǵ��Ѵ�
	// ���� �� �� ������ �������� �ʰ� �Ǿ� ������ ���´�
	if (SphereVisualAsset.Succeeded()) 
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object); // ��ü�� ����� ��
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f)); // Z ������ -40 ��ŭ �̵�
		SphereVisual->SetWorldScale3D(FVector(0.8f)); // �������� 0.8(= 80%)
	}

	// 3: ��Ȱ��ȭ�� ��ƼŬ �ý��� ������Ʈ�� ���� ������ ���δ�.
	// ������Ʈ�� Ȱ��/��Ȱ���� �ڵ忡�� ������ �� �ִ�.
	// ��ƼŬ �ý��� ������Ʈ�� ����ƽ �޽� ������Ʈ�� ���δ�(��Ʈ ������Ʈ�� ���̴� ���� �ƴϴ�
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// 4: ������ �� ������Ʈ�� �����Ѵ�
	// ī�޶� �ܴ��� ��ü�� �հ� ���� ���� �����ϴ� ���� ����� �����Ѵ�
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// 5: ���� ī�޶� ������Ʈ�� �����Ѵ�
	// ī�޶� ������Ʈ ����� ���� ����(�� ��쿡���� Ư���� ������ �ʿ� ����)
	// ������ �� ������Ʈ�� ����� ������ �����Ͽ� �����ϴ� ���� ����
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// 6: �����ϰ� ������ ������Ʈ�鿡 ���ؼ� �� ���� ����Ʈ �÷��̾�� ��Ʈ�� �� ���̶�� �����Ѵ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// �� ���� ���� ������Ʈ���� �����ϴ� ���� �Ϸ�Ǿ��� ������� ��Ʈ���� ���� �� �ְ� �Ǿ���.
	// ���Ĵ� �𸮾� �����Ϳ��� �����Ѵ�.


	// 0: �Է� ó��
	// 1: Pawn Movement Component�� �����ϰ� ���� �����Ѵ�
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

	// �Է� �̺�Ʈ�� �Լ����� ���ε����ִ� �κ�
	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ParticleToggle);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);
	


}

// �Է�
UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}
// �Է� ó��
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