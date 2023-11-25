// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "AbilitySystemComponent.h"
#include "ItemObject.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projekt/Miscellaneous/EIGameplayTags.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetupAttachment(SceneComponent);


	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);
	
	ItemProperties.ItemName = FText::FromString("Placeholder");
	ItemProperties.Value = 0;	

	RenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	RenderComponent->SetupAttachment(Mesh);
	RenderComponent->SetText(FText(ItemProperties.ItemName));
	RenderComponent->SetVisibility(true);
	RenderComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;

	// To exclude from postprocess outline shader
	RenderComponent->SetRenderInMainPass(false);
	RenderComponent->SetRenderCustomDepth(true);

	const FEIGameplayTags& GameplayTags = FEIGameplayTags::Get();
	HoverOverItemOnTheGround = GameplayTags.Block_Attacks;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	RenderComponent->AddLocalRotation(FRotator(60.0f,180.0f,0.0f));
	Mesh->SetCustomDepthStencilValue(252);
	
	if(ItemObject)
	{
		RenderComponent->SetText(FText(ItemObject->ItemName));	
	}
	
}

void APickup::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	
	Mesh->SetRenderCustomDepth(true);
	
	AMainCharacter* MainCharacter =  Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	MainCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(HoverOverItemOnTheGround);
	
}

void APickup::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	Mesh->SetRenderCustomDepth(false);

	AMainCharacter* MainCharacter =  Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	MainCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(HoverOverItemOnTheGround);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTextRenderer();
}

//TODO might create performance issues
void APickup::RotateTextRenderer()
{
	if(RenderComponent->IsVisible())
	{
		if(AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			FRotator PlayerRotation = Player->GetActorRotation();
			//RenderComponent->SetWorldRotation(PlayerRotation);
		//	RenderComponent->AddLocalRotation(FRotator(60.0f,180.0f,0.0f));
		}
	}
}

UStaticMeshComponent* APickup::GetMeshComponent() const
{
	return Mesh;
}

USphereComponent* APickup::GetSphereComponent() const
{
	return SphereComponent;
}

