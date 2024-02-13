// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "PlayerCharacterAnimInstance.h"
#include "ItemActor.h"
#include "ItemBoxActor.h"
#include "PlayerSight.h"
#include "NormalWeaponActor.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	PlayerSight = CreateDefaultSubobject<UPlayerSight>(TEXT("PLAYERSIGHT"));
	CurrentWeapon = CreateDefaultSubobject<ANormalWeaponActor>(TEXT("NORMALWEAPON"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	PlayerSight->SetupAttachment(Camera);
	PlayerSight->SetRelativeLocation(FVector(150.f, 0.f, 88.f));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/ChAnimBlueprint.ChAnimBlueprint_C"));

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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.f);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	auto CharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
		CharacterAnimInstance->SetIsPawnRun(m_bRun);
	}

}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &ABaseCharacter::MoveLeft);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABaseCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ABaseCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABaseCharacter::Jump); // 수정 필요

	PlayerInputComponent->BindAction(TEXT("GetItem"), IE_Pressed, this, &ABaseCharacter::GetItem);

}


void ABaseCharacter::MoveForward(float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
		
	}
	else {
		SetSpeed(200.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
}

void ABaseCharacter::MoveLeft(float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
	}
	else {
		SetSpeed(200.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
}

void ABaseCharacter::Run()
{
	if (m_bRun) {
		m_bRun = false;
	}
	else {
		m_bRun = true;
	}
}

void ABaseCharacter::Jump()
{

}

void ABaseCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ABaseCharacter::Turn(float NewAxisValue)
{

	AddControllerYawInput(NewAxisValue);
}
// 수정 필요
void ABaseCharacter::GetItem()
{
	if (PlayerSight->GetIsHit()) {
		
		auto Itembox = Cast<AItemBoxActor>(PlayerSight->GetHitActor());
		
		FString itemid = Itembox->GetInBoxItemId();

		if (itemid == "SquareWood") {

		}
		
		Itembox->OnChracterOvelapNew(this);

		PlayerSight->GetHitActor()->Destroy();
	}
}

bool ABaseCharacter::CanSetWeapon()
{
	return (CurrentWeapon == nullptr);
}

void ABaseCharacter::SetWeapon(ANormalWeaponActor* NewWeapon)
{
	if (NewWeapon != nullptr) {
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;

		// 이렇게 하면 안되고 소켓을 만들고 거기다 부착하는 식으로 해야 할 듯 캐릭터랑 무기랑 따로논다.
		CurrentWeapon->SetActorRelativeLocation(FVector(-20.f, 20.f, 88.f));
		CurrentWeapon->SetActorRelativeRotation(FRotator(0.f, 0.f, 60.f));                    
		UE_LOG(LogTemp, Warning, TEXT("wEAPONZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"));
	}
}
