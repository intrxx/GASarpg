// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

#include "Projekt/Miscellaneous/MyGameInstance.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"

UItemObject::UItemObject()
{
	ItemName = FText::FromString("No override on item name");
	ItemDescription = FText::FromString("No override on item description");
	UseActionText = FText::FromString("No override on item use");
	
	InventoryIndex = 0;
	SlotIndex = 0;
	Amount = 1;
	MaxStackableAmount = 3;
	bIsStackable = false;
}

void UItemObject::Use(AMainCharacter* MainCharacter)
{
}

void UItemObject::SpawnItem()
{
	FRotator Rotation = FRotator::ZeroRotator;

	FHitResult MouseHitResult;
			
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseHitResult);

	FActorSpawnParameters SpawnParams;

	MouseHitResult.Location.Z += 25;
			
	APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupData, MouseHitResult.Location, Rotation, SpawnParams);

	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->PickupArray.Add(SpawnedPickup);

	float RandomXOffset = FMath::RandRange(MinXYOffset, MaxXYOffset);
	float RandomYOffset = FMath::RandRange(MinXYOffset, MaxXYOffset);
	float RandomZOffset = FMath::RandRange(MinZOffset, MaxZOffset);
	
	FVector ImpulseVector = FVector(RandomXOffset, RandomYOffset, RandomZOffset);
	
	SpawnedPickup->GetMeshComponent()->AddImpulse(ImpulseVector);
}