// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

USTRUCT()
struct FItemProperties
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FText ItemName;

	UPROPERTY(EditAnywhere,  Category = "Pickup Properties")
	UTexture2D* ItemPicture;
	
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	int32 Value;
};

class USphereComponent;
class UTextRenderComponent;
class UItemObject;

UCLASS()
class PROJEKT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	FORCEINLINE FItemProperties GetItemProperties() {return ItemProperties;}

	UPROPERTY(EditAnywhere, Instanced, Category = "Pickup Properties")
	UItemObject* ItemObject;

	FGameplayTag HoverOverItemOnTheGround;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginCursorOver() override;

	virtual void NotifyActorEndCursorOver() override;
	
	UFUNCTION()
	void RotateTextRenderer();

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FItemProperties ItemProperties;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
public:
	UStaticMeshComponent* GetMeshComponent() const;
	USphereComponent* GetSphereComponent() const;
	
protected:
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* RenderComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
