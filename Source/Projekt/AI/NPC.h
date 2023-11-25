// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

class UDialog;
class UDataTable;

struct FDialogM;

UCLASS()
class PROJEKT_API ANPC : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ANPC();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UDialog* DialogComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UDataTable* PlayerDialog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UDataTable* FirstDialog;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	bool bIsTalking = false;
};
