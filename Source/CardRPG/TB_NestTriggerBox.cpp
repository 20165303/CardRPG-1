// Fill out your copyright notice in the Description page of Project Settings.


#include "TB_NestTriggerBox.h"
#include "MainCharacter.h"
#include "InGameHud.h"
#include "Kismet/GameplayStatics.h"

ATB_NestTriggerBox::ATB_NestTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ATB_NestTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATB_NestTriggerBox::OnOverlapEnd);

}


void ATB_NestTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	InGameHud = Cast<AInGameHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
}


void ATB_NestTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ON"));
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MainCharacter->NestAnimationVisible = true;
	InGameHud->UpdateWidgetVisibilityNest();
	InGameHud->PlayAnimationByNameNest();
	FTimerHandle WaitHandle;
	float WaitTime = 4.0; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

			Destroy();

		}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������
}

void ATB_NestTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OFF"));
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}
