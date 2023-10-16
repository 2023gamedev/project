// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "CharacterAnimInstance.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAnimBlueprint.CharacterAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SpringArm->TargetArmLength = 300.f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f,0.f);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	auto CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
		CharacterAnimInstance->SetIsPawnRun(m_bRun);
		CharacterAnimInstance->SetIsPawnHandInBall(m_bHandInBall);
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &APlayerCharacter::MoveLeft);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Throw"), IE_Pressed, this, &APlayerCharacter::Throw);

}

void APlayerCharacter::MoveForward(float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
	}
	else {
		SetSpeed(200.f);
	}

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
}

void APlayerCharacter::MoveLeft(float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
	}
	else {
		SetSpeed(200.f);
	}

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
}

void APlayerCharacter::Run()
{
	if (m_bRun) {
		m_bRun = false;
	}
	else {
		m_bRun = true;
	}
}

void APlayerCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void APlayerCharacter::Turn(float NewAxisValue)
{

	AddControllerYawInput(NewAxisValue);
}

void APlayerCharacter::Throw()
{
	
	// 수정 필요
	if (m_bHandInBall) {
		m_bHandInBall = false;
	}
	else {
		m_bHandInBall = true;
	}
}

