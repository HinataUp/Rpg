// Fill out your copyright notice in the Description page of Project Settings.


#include "RpgPlayer.h"
#include "Camera/CameraComponent.h"
#include "Component/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/RpgPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARpgPlayer::ARpgPlayer()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// mesh
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void ARpgPlayer::BeginPlay()
{
	Super::BeginPlay();

	LoadSkeletalMesh("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'");
	LoadAnimation("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn'");
}

// Called every frame
void ARpgPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARpgPlayer::OnDeath_Implementation()
{
	ARpgPlayerController* PlayerController = Cast<ARpgPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowRestartWidget();
	}
}

void ARpgPlayer::OnTakeDamage_Implementation()
{
	ARpgPlayerController* PlayerController = Cast<ARpgPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->UpdateHealthPercent(HealthComponent->GetHealthPercent());
	}
}

void ARpgPlayer::LoadSkeletalMesh(const FString& MeshPath)
{
	if (USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath))
	{
		GetMesh()->SetSkeletalMesh(mesh);
		UE_LOG(LogTemp, Log, TEXT("Successfully loaded skeletal mesh: %s"), *MeshPath)
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load skeletal mesh: %s"), *MeshPath);
	}
}

void ARpgPlayer::LoadAnimation(const FString& AnimationPath)
{
	if (const UAnimBlueprint* AnimBlueprint = LoadObject<UAnimBlueprint>(this, *AnimationPath))
	{
		// GetAnimBlueprintGeneratedClass  和 getclass 
		LoadedAnimBPClass = AnimBlueprint->GetAnimBlueprintGeneratedClass();
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(LoadedAnimBPClass);
		UE_LOG(LogTemp, Log, TEXT("Successfully loaded animation blueprint: %s"), *AnimationPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load animation blueprint: %s"), *AnimationPath);
	}
}
