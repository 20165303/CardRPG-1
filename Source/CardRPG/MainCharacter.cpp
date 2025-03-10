// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include"GameFramework/SpringArmComponent.h"
#include"Camera/CameraComponent.h"
#include"Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"
#include "WallSkill.h"
#include "RangeSkill.h"
#include "Teleport.h"
#include "ShockSkill.h"
#include "YellowRushStart.h"
#include "FollowingDrone.h"
#include "MovingSkill.h"
#include "SpiderMine.h"
#include "StatComponent.h"
#include "MyUserWidget.h"
#include "HealSkill.h"
#include "ShieldSkill.h"
#include "IceSkill.h"
#include "CardDropActor.h"
#include "InGameHud.h"
#include "HardAttackSkill.h"
#include "DamageTextActor.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "ai_tags.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/AnimMontage.h"
#include "Spidering.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
AMainCharacter::AMainCharacter() :	
	right_fist_collision_box(CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM1"));
	Camera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA1"));

	CastFrom= CreateDefaultSubobject<USceneComponent>(TEXT("CASTFROM"));
	DroneLocation= CreateDefaultSubobject<USceneComponent>(TEXT("DRONELOCATION"));
	FireTornadoLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FIRETORNADOLOCATION"));
	IceSkillLocation= CreateDefaultSubobject<USceneComponent>(TEXT("ICESKILLLOCATION"));
	TeleportLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TELEPORTLOCATION"));
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteraactionBox"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SOUNDEFFECT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation=true;
	Camera->SetupAttachment(SpringArm);

	SpringArm1->SetupAttachment(GetCapsuleComponent());
	Camera1->SetupAttachment(SpringArm1);

	CastFrom->SetupAttachment(GetCapsuleComponent());
	DroneLocation->SetupAttachment(GetCapsuleComponent());
	FireTornadoLocation->SetupAttachment(GetCapsuleComponent());
	IceSkillLocation->SetupAttachment(GetCapsuleComponent());
	TeleportLocation->SetupAttachment(GetCapsuleComponent());
	InteractionBox->SetupAttachment(RootComponent);
	AudioComponent->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SetRelativeRotation(FRotator(-25.0f, 0.0f, 0.0f));
	SpringArm1->TargetArmLength = 350.0f;
	SpringArm1->SetRelativeLocation(FVector(700,0,80.0f));
	SpringArm1->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	Camera1->SetRelativeRotation(FRotator(0,0,-180));

	CastFrom->SetRelativeLocation(FVector(85.f,0.f,20.f));
	DroneLocation->SetRelativeLocation(FVector(5.0f, 140.0f, 45.0f));
	FireTornadoLocation->SetRelativeLocation(FVector(500.0f, 0.f,0.0f));
	IceSkillLocation->SetRelativeLocation(FVector(20.0f,0.0f,0.0f));
	TeleportLocation->SetRelativeLocation(FVector(800.0f,0,-88.0f));

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SM(TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));


	static ConstructorHelpers::FObjectFinder<USoundBase> EF(TEXT("SoundWave'/Game/ParagonPhase/Characters/Heroes/Phase/Sounds/SoundWaves/Phase_Effort_Pain_03.Phase_Effort_Pain_03'"));
	if (EF.Succeeded())
	{
		AttackedSound = EF.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> FRS(TEXT("SoundWave'/Game/Resources/soundEffect/SE-29.SE-29'"));
	if (FRS.Succeeded())
	{
		FireReadySound = FRS.Object;
	}	static ConstructorHelpers::FObjectFinder<USoundBase> RS(TEXT("SoundWave'/Game/Resources/soundEffect/SE-43.SE-43'"));
	if (RS.Succeeded())
	{
		RollSound = RS.Object;
	}
	if (SM.Succeeded())
	{	
		UE_LOG(LogTemp,Warning,TEXT("GetMesh Succeeded"));
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/BluePrints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(64.0f, 130.0f, 120.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());



	Stats = CreateDefaultSubobject<UStatComponent>(TEXT("STATS"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bStartWithTickEnabled = true;

	setup_stimulus();

	if (right_fist_collision_box)
	{
		FVector const extent(5.0f);
		right_fist_collision_box->SetBoxExtent(extent, false);
		right_fist_collision_box->SetCollisionProfileName("NoCollision");
	}
}

void AMainCharacter::on_attack_overlap_begin(UPrimitiveComponent* const overlapped_component, AActor* const other_actor, UPrimitiveComponent* other_component, int const other_body_index, bool const from_sweep, FHitResult const& sweep_result)
{
}

void AMainCharacter::on_attack_overlap_end(UPrimitiveComponent* const overlapped_component, AActor* const other_actor, UPrimitiveComponent* other_component, int const other_body_index)
{
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Camera1->Deactivate();
	PC = Cast<APlayerController>(GetController());
    CursorToWorld->SetVisibility(false);
	//InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBoxBeginOverlap);
	//InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnBoxEndOverlap);
	if (right_fist_collision_box)
	{
		FAttachmentTransformRules const rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);
		right_fist_collision_box->AttachToComponent(GetMesh(), rules, "hand_r_socket");
		right_fist_collision_box->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
	}

	if (right_fist_collision_box)
	{
		right_fist_collision_box->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_begin);
		right_fist_collision_box->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_end);
	}
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMainCharacter::OnAttackMontageEnded);
	}

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CursorToWorld != nullptr)
	{
		if (PC)
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	TArray<AActor*>OverlappingActors;
	InteractionBox->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num()==0)
	{
		if (Interface)
		{
			Interface->HideInteractionWidget();
			Interface=nullptr;
		}
		return;
	}

	AActor* CloestActor = OverlappingActors[0];

	for (auto CurrentActor:OverlappingActors)
	{
		if (GetDistanceTo(CurrentActor)<GetDistanceTo(CloestActor))
		{
			CloestActor=CurrentActor;
		}
	}

	if (Interface)
	{
		Interface->HideInteractionWidget();
	}

	Interface= Cast<IInteractionInterface>(CloestActor);

	if (Interface)
	{
		Interface->ShowInteractionWidget();
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Lookup"),this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Attack"),EInputEvent::IE_Pressed,this,&AMainCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Dodge"),EInputEvent::IE_Pressed,  this, &AMainCharacter::Dodge);
	PlayerInputComponent->BindAction(TEXT("Rush"),EInputEvent::IE_Pressed, this, &AMainCharacter::Rush);
	PlayerInputComponent->BindAction(TEXT("Fast"),EInputEvent::IE_Pressed,this, &AMainCharacter::Fast);
	PlayerInputComponent->BindAction(TEXT("Mine"),EInputEvent::IE_Pressed,this, &AMainCharacter::Mine);
	PlayerInputComponent->BindAction(TEXT("Heal"),EInputEvent::IE_Pressed,this, &AMainCharacter::Heal);
	PlayerInputComponent->BindAction(TEXT("Shield"),EInputEvent::IE_Pressed,this, &AMainCharacter::Shield);
	PlayerInputComponent->BindAction(TEXT("GetXp"),EInputEvent::IE_Pressed,this, &AMainCharacter::GenerateXp);
	PlayerInputComponent->BindAction(TEXT("DroneAttack"), EInputEvent::IE_Pressed, this, &AMainCharacter::DroneAttack);
	PlayerInputComponent->BindAction(TEXT("WallSkill"),EInputEvent::IE_Pressed,this, &AMainCharacter::WallSkill);
	PlayerInputComponent->BindAction(TEXT("WallSkill"),EInputEvent::IE_Released,this, &AMainCharacter::WallSkillOn);
	PlayerInputComponent->BindAction(TEXT("ElecSkill"), EInputEvent::IE_Pressed, this, &AMainCharacter::ElectoronicShock);
	PlayerInputComponent->BindAction(TEXT("ElecSkill"), EInputEvent::IE_Released, this, &AMainCharacter::ElectoronicShockOn);
	PlayerInputComponent->BindAction(TEXT("IceExplosion"),EInputEvent::IE_Pressed,this,&AMainCharacter::IceExplosion);
	PlayerInputComponent->BindAction(TEXT("IceExplosion"),EInputEvent::IE_Pressed,this,&AMainCharacter::IceExplosion);
	PlayerInputComponent->BindAction(TEXT("IceSkill"),EInputEvent::IE_Pressed,this, &AMainCharacter::BlueRush);
	PlayerInputComponent->BindAction(TEXT("Teleport"),EInputEvent::IE_Pressed,this, &AMainCharacter::Teleport);
	//Interact Key
	PlayerInputComponent->BindAction(TEXT("Interact"),EInputEvent::IE_Pressed,this, &AMainCharacter::OnInteract);
	PlayerInputComponent->BindAction(TEXT("Distract"), IE_Pressed, this, &AMainCharacter::on_distract);
	PlayerInputComponent->BindAction(TEXT("CameraChange"),IE_Pressed,this,&AMainCharacter::ChangeCamera);
}

void AMainCharacter::UpDown(float Value)
{	
	if ((IsAttacking==false)&&(IsSkillUsing==false))
	{
		
		UpdownValue = Value;
		AddMovementInput(GetActorForwardVector(), Value);
	}


}

void AMainCharacter::LeftRight(float Value)
{
	if ((IsAttacking == false) && (IsSkillUsing == false))
	{
		
		LeftRightValue = Value;
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMainCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::Dodge()
{
	if ((IsAttacking == false) && (IsSkillUsing == false))
	{
	    AnimInstance->PlayDodgeMontage();
		AudioComponent->SetSound(RollSound);
		AudioComponent->Play();
	}
	IsSkillUsing=true;

}

void AMainCharacter::Attack()
{
	if (IsAttacking || IsSkillUsing)
	{
		return;
	}
	AnimInstance->PlayAttackMontage();

	FVector SpawnLocation= CastFrom->GetComponentLocation();
	FRotator SpawnRotation=GetCapsuleComponent()->GetComponentRotation();
	if (AttackIndex<5)
	{
		GetWorld()->SpawnActor<ABullet>(SpawnLocation, SpawnRotation);
	}
	else
	{
		GetWorld()->SpawnActor<AHardAttackSkill>(SpawnLocation,SpawnRotation);
	}

	AnimInstance->JumpToSection(AttackIndex);

	AttackIndex = (AttackIndex + 1) % 6;
	IsAttacking = true;

	FVector const loc = GetActorLocation();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tags::noise_tag);
}

void AMainCharacter::UseSkill() {  //Binding Q Key Pressed
	switch (SkillCode) {
	case 101:
		WallSkill();
		break;
	case 102:
		Rush(); //Yellow Rush
		break;
	case 103:
		Fast();
		break;
	case 104:
		Mine();
		break;
	case 105:
		Heal();
		break;
	case 106:
		Shield();
		break;
	case 107:
		BlueRush();//Blue Rush
		break;
	case 108:
		Teleport();
		break;
	case 109:
		IceExplosion();//ice_explosion
		break;
	case 110: //electoronic Shock
		ElectoronicShock();
		break;
	default:
		break;
	}
	AttackIndex = 1;
}

void AMainCharacter::UseSkill_R() { //Binding Q Key Released
	switch (SkillCode)
	{
	case 101:
		WallSkillOn();
		AudioComponent->SetSound(FireReadySound);
		AudioComponent->Play();
		break;

	case 110: //electoronic Shock
		ElectoronicShockOn();
		AudioComponent->SetSound(FireReadySound);
		AudioComponent->Play();
	default:
		break;
	}
}

#pragma region Q Skills
void AMainCharacter::Rush()
{
	if (IsSkillUsing)
	{
		return;
	}

	AnimInstance->PlayRushSkillMontage();
	FVector SpawnLocation = IceSkillLocation->GetComponentLocation();
	FRotator SpawnRotation = GetCapsuleComponent()->GetComponentRotation();
	GetWorld()->SpawnActor<AYellowRushStart>(SpawnLocation, SpawnRotation);
	IsSkillUsing = true;
}

void AMainCharacter::WallSkill()
{
	UE_LOG(LogTemp, Warning,TEXT("WALLSKILL"));
	if (IsSkillUsing)
	{
	return;
	}
    CursorToWorld->SetVisibility(true);	
    auto PlayerController = UGameplayStatics::GetPlayerController(this,0);
    PlayerController->SetMouseLocation(600,300);
}
void AMainCharacter::WallSkillOn()
{
	UE_LOG(LogTemp, Warning, TEXT("WALLSKILLON"));
	if (IsSkillUsing)
	{
		return;
	}
	IsSkillUsing= true;
    CursorToWorld->SetVisibility(false);
	FVector WorldLocation;
    FVector WorldDirection;
	float DistanceAboveGround = CursorToWorld->GetComponentLocation().Z;
	//float DistanceAboveGround=50.0f;

    auto PlayerController = UGameplayStatics::GetPlayerController(this,0);
    PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

    FVector PlaneOrigin(0.0f, 0.0f, DistanceAboveGround);
    
    FVector ActorWorldLocation = FMath::LinePlaneIntersection(
    	WorldLocation,
    	WorldLocation + WorldDirection,
    	PlaneOrigin,
    	FVector::UpVector);   		


    FRotator SpawnRotation = GetCapsuleComponent()->GetComponentRotation();
    
    AnimInstance->PlayWallSkillMontage();
    GetWorld()->SpawnActor<AWallSkill>(ActorWorldLocation, SpawnRotation);
}

void AMainCharacter::BlueRush()
{
	if (IsSkillUsing)
	{
		return;
	}

	AnimInstance->PlayWallSkillMontage();
	FVector SpawnLocation = IceSkillLocation->GetComponentLocation();
	FRotator SpawnRotation = GetCapsuleComponent()->GetComponentRotation();
	GetWorld()->SpawnActor<ARangeSkill>(SpawnLocation, SpawnRotation);
	IsSkillUsing = true;
}

void AMainCharacter::Teleport()
{
	if (IsSkillUsing)
	{
		return;
	}

	AnimInstance->PlayWallSkillMontage();
	TelpoLoc = TeleportLocation->GetComponentLocation();
	FVector CurrentLoc= GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -88);

	GetWorld()->SpawnActor<ATeleport>(TelpoLoc, FRotator(0, 0, 0));
	GetWorld()->SpawnActor<ADamageTextActor>(TelpoLoc+FVector(0,0,50),FRotator(0,0,0));
	GetWorld()->SpawnActor<ATeleport>(CurrentLoc, FRotator(0, 0, 0));
	float WaitTime= 1.0f; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			SetActorLocation(TelpoLoc +FVector(0,0,88));
		}), WaitTime, false);
	IsSkillUsing = true;
}

void AMainCharacter::Fast()
{
	if (IsSkillUsing)
	{
		return;
	}
	AnimInstance->PlayWallSkillMontage();
	FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation() + FVector(0,0,-88);
	GetWorld()->SpawnActor<AMovingSkill>(CurrentLoc,FRotator(0,0,0));
	UCharacterMovementComponent* CM= GetCharacterMovement();
	WalkSpeed = CM->MaxWalkSpeed;
	CM->MaxWalkSpeed=WalkSpeed*2;


	float WaitTime = 10.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			ResetWalkSpeed();
		}), WaitTime, false);
	IsSkillUsing = true;

}

void AMainCharacter::Mine()
{
	if (IsSkillUsing)
	{
		return;
	}
	AnimInstance->PlayWallSkillMontage();
	FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -80);
	GetWorld()->SpawnActor<ASpiderMine>(CurrentLoc, FRotator(0, 0, 0));
	IsSkillUsing = true;
}

void AMainCharacter::Heal()
{
	if (IsSkillUsing)
	{
		return;
	}
	AnimInstance->PlayWallSkillMontage();
	FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -80);
	GetWorld()->SpawnActor<AHealSkill>(CurrentLoc, FRotator(0, 0, 0));
	Stats->HealHp(50);
	IsSkillUsing = true;

}

void AMainCharacter::Shield()
{
	if (IsSkillUsing)
	{
		return;
	}
	AnimInstance->PlayWallSkillMontage();
	FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -80);
	GetWorld()->SpawnActor<AShieldSkill>(CurrentLoc, FRotator(0, 0, 0));
	IsSkillUsing = true;


}

void AMainCharacter::ElectoronicShock()
{
	UE_LOG(LogTemp, Warning, TEXT("WALLSKILL"));
	if (IsSkillUsing)
	{
		return;
	}
	CursorToWorld->SetVisibility(true);
	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetMouseLocation(600, 300);
}

void AMainCharacter::ElectoronicShockOn()
{
	UE_LOG(LogTemp, Warning, TEXT("WALLSKILLON"));
	if (IsSkillUsing)
	{
		return;
	}
	IsSkillUsing = true;
	CursorToWorld->SetVisibility(false);
	FVector WorldLocation;
	FVector WorldDirection;
	float DistanceAboveGround = CursorToWorld->GetComponentLocation().Z;
	//float DistanceAboveGround=50.0f;

	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FVector PlaneOrigin(0.0f, 0.0f, DistanceAboveGround);

	FVector ActorWorldLocation = FMath::LinePlaneIntersection(
		WorldLocation,
		WorldLocation + WorldDirection,
		PlaneOrigin,
		FVector::UpVector);


	FRotator SpawnRotation = GetCapsuleComponent()->GetComponentRotation();

	AnimInstance->PlayWallSkillMontage();
	GetWorld()->SpawnActor<AShockSkill>(ActorWorldLocation, SpawnRotation);
	FVector const loc = GetActorLocation();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tags::noise_tag);
}

#pragma endregion

void AMainCharacter::GenerateXp()
{
	GetWorld()->SpawnActor<ACardDropActor>(FireTornadoLocation->GetComponentLocation(), FRotator(0, 0, 0));
}



void AMainCharacter::ResetWalkSpeed()
{
	UCharacterMovementComponent* CM = GetCharacterMovement();
	CM->MaxWalkSpeed = 600;
}

void AMainCharacter::DroneAttack()
{
	float WaitTime=0.5f;
	if (DroneIsAttacking==false)
	{
		DroneIsAttacking=true;
		GetWorld()->GetTimerManager().SetTimer(WaitHandleDrone, FTimerDelegate::CreateLambda([&]()
			{
				FVector SpawnLocation = DroneLocation->GetComponentLocation();
				FRotator SpawnRotation = GetCapsuleComponent()->GetComponentRotation();
				GetWorld()->SpawnActor<ABullet>(SpawnLocation, SpawnRotation);
			}), WaitTime, true); //�ݺ��� ���⼭ �߰� ������ ������ ��������
			return;
	}
	if (DroneIsAttacking==true)
	{
		DroneIsAttacking=false;
		GetWorld()->GetTimerManager().SetTimer(WaitHandleDrone, FTimerDelegate::CreateLambda([&]()
			{
				FVector SpawnLocation = DroneLocation->GetComponentLocation();
				FRotator SpawnRotation = GetCapsuleComponent()->GetComponentRotation();
				GetWorld()->SpawnActor<ABullet>(SpawnLocation, SpawnRotation);
			}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������
		return;
	}
	FVector const loc = GetActorLocation();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tags::noise_tag);
}

void AMainCharacter::IceExplosion()
{
	if (IsSkillUsing)
	{
		return;
	}
	AnimInstance->PlayWallSkillMontage();
	FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, 0);
	GetWorld()->SpawnActor<AIceSkill>(CurrentLoc, FRotator(0, 0, 0));
	IsSkillUsing = true;
	FVector const loc = GetActorLocation();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tags::noise_tag);

}

void AMainCharacter::Dead()
{
	IsSkillUsing = true;
	AnimInstance->PlayDeadMontage();
	InGameHud = Cast<AInGameHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
	InGameHud->UpdateWidgetVisibilityDead();
	InGameHud->PlayAnimationByNameDead();
	float WaitTime = 1.0;
	GetWorld()->GetTimerManager().SetTimer(WaidHandleDead, FTimerDelegate::CreateLambda([&]()
		{	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			UE_LOG(LogTemp, Warning, TEXT("DEAD"));
			GetCharacterMovement()->Deactivate();
			GetMesh()->SetSimulatePhysics(true);
			AnimInstance->StopSlotAnimation(0.1f, "DefaultSlot");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}), WaitTime, false);
}





float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsImmune==true)
	{
		return DamageAmount;
	}
	GetWorld()->SpawnActor<ADamageTextActor>(GetActorLocation(), FRotator(0, 0, 0));
	Stats->OnAttacked(DamageAmount);
	AnimInstance->PlayAttackedMontage();
	AudioComponent->SetSound(AttackedSound);
	AudioComponent->Play();
	if (Stats->GetHp()<=0 && IsDead==false)
	{
		Dead();
		IsDead=true;
	}
	return DamageAmount;
}

float AMainCharacter::GetXP(float XpAmount)
{
	Stats->SetXP(XpAmount);
	return XpAmount;
}

/*void AMainCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Interface=Cast<IInteractionInterface>(OtherActor);

	if (Interface)
	{
		Interface->ShowInteractionWidget();
	}
}

void AMainCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (Interface)
	{
		Interface->HideInteractionWidget();
		Interface=nullptr;
	}

}
*/

void AMainCharacter::OnInteract()
{
	if (Interface)
	{
		Interface->InteractWithMe();
	}
}

void AMainCharacter::ChangeCamera()
{
	if (Camera1->IsActive())
	{
		Camera1->Deactivate();
		Camera->Activate();
	}
	else
	{
		Camera1->Activate();
		Camera->Deactivate();
	}

}

void AMainCharacter::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf < UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AMainCharacter::on_distract()
{
	if (distraction_sound)
	{
		FVector const loc = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), distraction_sound, loc);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tags::noise_tag);
	}
}

void AMainCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
	IsAttacking = false;
	IsSkillUsing=false;
	if (IsDead==true)
	{
		IsAttacking = true;
		IsSkillUsing = true;
	}

	OnAttackEnd.Broadcast();
}
