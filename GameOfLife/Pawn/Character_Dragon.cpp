// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Dragon.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Public/TimerManager.h"


// Sets default values
ACharacter_Dragon::ACharacter_Dragon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Time = 0;

    GetCharacterMovement()->GravityScale = 0;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> object_Dragon(TEXT("SkeletalMesh'/Game/Character/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
    static ConstructorHelpers::FObjectFinder<UAnimationAsset> object_Anim_Fly(TEXT("AnimSequence'/Game/Character/MountainDragon/Animations/ANIM_MOUNTAIN_DRAGON_flyNormal.ANIM_MOUNTAIN_DRAGON_flyNormal'"));
    GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    if (object_Dragon.Succeeded()) GetMesh()->SetSkeletalMesh(object_Dragon.Object);
    if (object_Anim_Fly.Succeeded()) GetMesh()->SetAnimation(object_Anim_Fly.Object);

    GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

    pSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
    pSpline->SetupAttachment(RootComponent);
    pSpline->SetClosedLoop(true);
    
    static ConstructorHelpers::FObjectFinder<USoundCue> object_Sound_Wing(TEXT("SoundCue'/Game/Sound/Cue/Cue_Wing.Cue_Wing'"));
    pAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    pAudio->SetupAttachment(GetMesh());
    if (object_Sound_Wing.Succeeded()) pAudio->SetSound(object_Sound_Wing.Object);
}

// Called when the game starts or when spawned
void ACharacter_Dragon::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacter_Dragon::TimerFunction, 0.01f, true);
}

// Called every frame
void ACharacter_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ACharacter_Dragon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacter_Dragon::TimerFunction() {
    Time += 0.0001;

    float Distance = pSpline->GetSplineLength() * Time;
    FVector Location = pSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
    FRotator Rotation = pSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
    Rotation.Yaw -= 90.f;

    GetMesh()->SetWorldLocationAndRotation(Location, Rotation);

    if (Time >= 1.f) Time = 0;
}

void ACharacter_Dragon::ToggleDragon(const bool bDragonVisible) {
    if (bDragonVisible) {
        GetMesh()->SetHiddenInGame(false);
        pAudio->SetActive(true);
    }
    else {
        GetMesh()->SetHiddenInGame(true);
        pAudio->SetActive(false);
    }
}
