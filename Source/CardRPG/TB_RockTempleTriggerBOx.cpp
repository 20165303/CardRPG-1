// Fill out your copyright notice in the Description page of Project Settings.


#include "TB_RockTempleTriggerBOx.h"
#include "MainCharacter.h"
#include "InGameHud.h"
#include "Kismet/GameplayStatics.h"

void ATB_RockTempleTriggerBOx::BeginPlay()
{
	Super::BeginPlay();

	InGameHud = Cast<AInGameHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

ATB_RockTempleTriggerBOx::ATB_RockTempleTriggerBOx()
{
	OnActorBeginOverlap.AddDynamic(this, &ATB_RockTempleTriggerBOx::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATB_RockTempleTriggerBOx::OnOverlapEnd);

}

void ATB_RockTempleTriggerBOx::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ON"));
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MainCharacter->TempleAnimationVisible = true;
	InGameHud->UpdateWidgetVisibilityRockTemple();
	InGameHud->PlayAnimationByNameRockTemple();
	FTimerHandle WaitHandle;
	float WaitTime = 4.0; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

			Destroy();

		}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������

}

void ATB_RockTempleTriggerBOx::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OFF"));
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}
