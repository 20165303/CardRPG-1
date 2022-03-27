// Fill out your copyright notice in the Description page of Project Settings.


#include "TB_TownTriggerBox.h"
#include "MyUserWidget.h"
#include "MainCharacter.h"
#include "InGameHud.h"
#include "Kismet/GameplayStatics.h"



ATB_TownTriggerBox::ATB_TownTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ATB_TownTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATB_TownTriggerBox::OnOverlapEnd);
}

void ATB_TownTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	InGameHud = Cast<AInGameHud>(GetWorld()->GetFirstPlayerController()->GetHUD());

}

void ATB_TownTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp,Warning,TEXT("ON"));
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter != nullptr) {
		MainCharacter->TownAnimationVisible = true;
		InGameHud->UpdateWidgetVisibilityTown();
		InGameHud->PlayAnimationByNameTown();
		FTimerHandle WaitHandle;
		float WaitTime = 4.0; //�ð��� �����ϰ�
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{

				Destroy();

			}), WaitTime, false); //�ݺ��� ���⼭ �߰� ������ ������ ��������
	}
}

void ATB_TownTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OFF"));
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}
