// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Projekt/Inventory/ChestBase.h"
#include "Projekt/Dialog/Dialog.h"
#include "Projekt/Input/EIEnhancedInputComponent.h"
#include "Projekt/Miscellaneous/EIGameplayTags.h"
#include "Projekt/AI/EnemyBase.h"
#include "Projekt/GAS/GASAbilitySystemComponent.h"
#include "Projekt/GAS/GASAttributeSet.h"
#include "Projekt/GAS/GASGameplayAbility.h"
#include "Projekt/Inventory/ItemObject.h"
#include "Projekt/Inventory/ItemSlotWidget.h"
#include "GameFramework/PlayerController.h"
#include "Projekt/Inventory/MainCharacterInventory.h"
#include "MainCharacterPlayerController.h"
#include "Projekt/Miscellaneous/MyGameInstance.h"
#include "Projekt/Inventory/Pickup.h"
#include "CharacterPaladin.h"
#include "MainCharacterHUDWidget.h"
#include "UPlayerResourceWidget.h"
#include "Projekt/GAS/TickTimestopActor.h"
#include "Projekt/Inventory/WidgetInventory.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projekt/Shared/PCharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//Create a camera boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	//Colision test for the camera
	SpringArm->bDoCollisionTest = false;
	
	// Rotate the arm based on the controller
	SpringArm->bUsePawnControlRotation = true;
	
	// Creating Camera components
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Creating AI Stimuli component
	AIStimuliComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIStimuli"));
	
	const FEIGameplayTags& GameplayTags = FEIGameplayTags::Get();
	BlockAbilityUse = GameplayTags.Block_Attacks;
	DeadTag = GameplayTags.State_Dead;
}

UAbilitySystemComponent* AMainCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// creating camera components
	SpringArm->TargetArmLength = SpringArmLength;
	SpringArm->SetAbsolute(false, true,false);
	SpringArm->AddWorldRotation(FRotator(CameraRotation,0,0));

	// Get reference to inventory component
	MainCharacterInventoryComponent = FindComponentByClass<UMainCharacterInventory>();
	// Get reference to dialog component
	DialogComponent = FindComponentByClass<UDialog>();
	
	// Set main char ref (has to be there due to callstack)
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GI->MainCharacter = this;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChestBase::StaticClass(), GI->ChestArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), GI->PickupArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), GI->EnemyArray);

	
	GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = true;

	//TODO it fixes crash on pickup change the fix to something more stable
	InventoryPressed();
	InventoryPressed();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//ENHANCED INPUT CHANGES

	UEIEnhancedInputComponent* EnhancedInputComponent = Cast<UEIEnhancedInputComponent>(PlayerInputComponent);

	check(EnhancedInputComponent);

	const FEIGameplayTags& GameplayTags = FEIGameplayTags::Get();
	
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &AMainCharacter::Input_Move);
	//TODO Disabling rotation for the time being, feels better
	//EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_RotatePlayer, ETriggerEvent::Triggered, this, &AMainCharacter::Input_Rotate);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_ZoomCameraIn, ETriggerEvent::Triggered, this, &AMainCharacter::Input_ZoomCameraIn);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_ZoomCameraOut, ETriggerEvent::Triggered, this, &AMainCharacter::Input_ZoomCameraOut);

	// Toggle Widgets
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_ToggleFloatingWidget, ETriggerEvent::Triggered, this, &AMainCharacter::Input_ToggleFloatingWidget);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_ToggleSpellTree, ETriggerEvent::Triggered, this, &AMainCharacter::Input_ToggleSpellTree);
	
	// Inventory
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_OpenInventory, ETriggerEvent::Triggered, this, &AMainCharacter::InventoryPressed);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Pickup, ETriggerEvent::Triggered, this, &AMainCharacter::OnMouseClick);

	// Menu
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_MenuSwitch, ETriggerEvent::Triggered, this, &AMainCharacter::MenuSwitch);
	// Quest
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_QuestSwitch, ETriggerEvent::Triggered, this, &AMainCharacter::QuestSwitch);

	//Ability System
	if (AbilitySystemComponent.IsValid() && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);

		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_LightAttack, ETriggerEvent::Triggered, this, &AMainCharacter::Input_LightAttack);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_HeavyAttack, ETriggerEvent::Triggered, this, &AMainCharacter::Input_HeavyAttack);
		
		//Just for some time till we make player binding
		// IWNI JAK BEDZIESZ TO ZMIENIAL TO ALBO DAJ ZNAC ALBO TIMESTOPA ZROB W TEN SAM SPOSOB OWO
		// NO TO NIEZLE KIEDY PLAYER BINDING??? X            D
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_FirstAbility, ETriggerEvent::Triggered, this, &AMainCharacter::Input_FirstAbility);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_SecondAbility, ETriggerEvent::Triggered, this, &AMainCharacter::Input_SecondAbility);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_ThirdAbility, ETriggerEvent::Triggered, this, &AMainCharacter::Input_ThirdAbility);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_ForthAbility, ETriggerEvent::Triggered, this, &AMainCharacter::Input_FourthAbility);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_TimestopAbility, ETriggerEvent::Triggered, this, &AMainCharacter::Input_TimestopAbility);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_TimestopCancelLastMove, ETriggerEvent::Triggered, this, &AMainCharacter::Input_TimestopCancelLastMove);
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_PotionUse, ETriggerEvent::Triggered, this, &AMainCharacter::Input_UsingHealingPotion);
	}
}

// debug for now checks what is being clicked
// CHANGE COLLISION CHANNEL TO IF YOU WANT TO TEST DIFFERENT OBJECTS
void AMainCharacter::OnMouseClick(const FInputActionValue& InputActionValue)
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);
	
	if (HitResult.GetComponent())
	{
//		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Mouse Click+++ Component: %s"), *HitResult.GetComponent()->GetName()));
	}

	if (HitResult.GetActor())
	{
//		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Mouse Click+++ Actor: %s"), *HitResult.GetActor()->GetName()));
	}

	if (APickup* ClickedPickup = Cast<APickup>(HitResult.GetActor()))
	{
		if(MainCharacterInventoryComponent->AddItem(ClickedPickup->ItemObject))
		{
			Cast<UMyGameInstance>(GetGameInstance())->PickupArray.Remove(ClickedPickup);
			ClickedPickup->Destroy();
		}
		else
		{
			// TODO IMPLEMENT FULL INVENTORY
		}
	}
	//TODO Keep an eye on in case of some weird tag crashes
	//This tag doesn't have to do anything with Inventory Open it is attack block tag (something to keep in mind), also maybe name the tags more universaliy?
	GetWorldTimerManager().SetTimer(DelayTagRemovalTimer,this, &AMainCharacter::RemoveTag,DelayTagRemoval);
}

//Toggle Floating Widget
void AMainCharacter::Input_ToggleFloatingWidget()
{
	ACharacterPaladin* Player = Cast<ACharacterPaladin>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
	{
		UUPlayerResourceWidget* PlayerResourceWidget = Player->GetFloatingResourceWidget();
		if(PlayerResourceWidget)
		{
			if(PlayerResourceWidget->bWidgetIsVisible)
			{
				PlayerResourceWidget->ToggleFloatingWidget(PlayerResourceWidget->bWidgetIsVisible = false);
			}
			else
			{
				PlayerResourceWidget->ToggleFloatingWidget(PlayerResourceWidget->bWidgetIsVisible = true);
			}
		}
	}
}

void AMainCharacter::Input_ToggleSpellTree()
{
	AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetController());
	if(PC)
	{
		UMainCharacterHUDWidget* HUD = PC->GetHUD();
		if(HUD)
		{
			if(HUD->bIsSkillTreeVisible)
			{
				HUD->ToggleSpellTree(HUD->bIsSkillTreeVisible = false);
				AbilitySystemComponent->RemoveLooseGameplayTag(BlockAbilityUse);
			}
			else
			{
				HUD->ToggleSpellTree(HUD->bIsSkillTreeVisible = true);
				AbilitySystemComponent->AddLooseGameplayTag(BlockAbilityUse);
			}
		}
	}
}

//Camera Zooms
void AMainCharacter::Input_ZoomCameraIn()
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - 45.f, 170.f, SpringArmLength);
}

void AMainCharacter::Input_ZoomCameraOut()
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + 45.f, 170.f, SpringArmLength);
	
}

void AMainCharacter::SendLocalInputToASC(bool bIsPressed, const EGASAbilityInputID AbilityInputID)
{
	if(AbilitySystemComponent.IsValid())
	{
		if(bIsPressed)
		{
			AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(AbilityInputID));
		}
		else
		{
			AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(AbilityInputID));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("We don't have a valid Ability System Component"));
	}
}

//Abilities input passing
void AMainCharacter::Input_LightAttack()
{
	SendLocalInputToASC(true, EGASAbilityInputID::LightAttack);
}

void AMainCharacter::Input_HeavyAttack()
{
	SendLocalInputToASC(true, EGASAbilityInputID::HeavyAttack);
}

void AMainCharacter::Input_FirstAbility()
{
	SendLocalInputToASC(true, EGASAbilityInputID::FirstAbility);
}

void AMainCharacter::Input_SecondAbility()
{
	SendLocalInputToASC(true, EGASAbilityInputID::SecondAbility);
}

void AMainCharacter::Input_ThirdAbility()
{
	SendLocalInputToASC(true, EGASAbilityInputID::ThirdAbility);
}

void AMainCharacter::Input_FourthAbility()
{
	SendLocalInputToASC(true, EGASAbilityInputID::ForthAbility);
}

void AMainCharacter::Input_TimestopAbility()
{
	SendLocalInputToASC(true, EGASAbilityInputID::TimestopAbility);
}

void AMainCharacter::Input_TimestopCancelLastMove()
{
	if(TimestopActor)
	{
		TimestopActor->CancelLastMove();
	}
}

void AMainCharacter::Input_UsingHealingPotion()
{
	SendLocalInputToASC(true, EGASAbilityInputID::UseHealingPotion);
}

//Keyboard movement
void AMainCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if((Controller != nullptr)) 
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (MoveValue.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MoveValue.X);
		}

		if (MoveValue.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MoveValue.Y);
		}
	}
}

void AMainCharacter::Input_Rotate()
{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if(PlayerController == nullptr)
		{
			return;
			
		}
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

		AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MainCharacter == nullptr)
		{
			return;
		}
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(MainCharacter->GetActorLocation(), HitResult.Location);
		MainCharacter->SetActorRotation(FRotator(GetControlRotation().Pitch, TargetRotation.Yaw, GetControlRotation().Roll));
}

void AMainCharacter::InventoryPressed()
{
	if(WidgetInventoryClass == nullptr) { return; }
	
	if(!bIsInventoryOpen)
	{
		AbilitySystemComponent->AddLooseGameplayTag(BlockAbilityUse);

		bIsInventoryOpen = true;
		// Creating widget ref and adding to the screen
		WidgetInventory = CreateWidget(GetWorld()->GetFirstPlayerController(), WidgetInventoryClass);
		WidgetInventory->AddToViewport();

		AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());

		PC->WidgetInventory = Cast<UWidgetInventory>(WidgetInventory);
	}
	else 
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(BlockAbilityUse);
		// Removes widget from viewport
		bIsInventoryOpen = false;

		AMainCharacterPlayerController* PC = Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController());

		if(Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController())->ItemSlotWidget)
		{
			if(UItemObject* Item = PC->ItemSlotWidget->ItemObject)
			{
				switch (Item->ItemOwner)
				{
				case CharacterInventory:
					GetInventoryComponent()->AddItem(Item);
					break;
				case CharacterEquipment:
					GetInventoryComponent()->EquipmentArray.Add(Item);
					break;
				case Chest:
					break;
				default:
					Item->SpawnItem();
					break;
				}
				Cast<AMainCharacterPlayerController>(GetWorld()->GetFirstPlayerController())->RemoveItemSlotWidget();
			}
		}
		
		if(WidgetInventory)
		{
			WidgetInventory->RemoveFromViewport();	
		}
	}
}

void AMainCharacter::MenuSwitch()
{
	if(MenuClass == nullptr) { return; }

	if(!bIsMenuOpen)
	{
		AbilitySystemComponent->AddLooseGameplayTag(MenuOpened);

		bIsMenuOpen = true;

		MenuWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), MenuClass);
		MenuWidget->AddToViewport();

		GetWorld()->GetFirstPlayerController()->SetPause(true);
	}
	else
	{
		bIsMenuOpen = false;
		if(MenuWidget)
		{
			MenuWidget->RemoveFromViewport();
			
			GetWorld()->GetFirstPlayerController()->SetPause(false);
		}
	}
}

void AMainCharacter::QuestSwitch()
{
	if(QuestClass == nullptr) { return; }

	if(!bIsQuestOpen)
	{
		QuestWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), QuestClass);
		QuestWidget->AddToViewport();
		bIsQuestOpen = true;
	}
	else
	{
		if(QuestWidget)
		{
			QuestWidget->RemoveFromViewport();
			bIsQuestOpen = false;
		}
	}
}

USpringArmComponent* AMainCharacter::GetSpringArm() const
{
	return SpringArm;
}

UCameraComponent* AMainCharacter::GetCamera() const
{
	return Camera;
}

void AMainCharacter::UseItem(UItemObject* Item)
{
	if(Item)
	{
		Item->Use(this);
		//bp
		Item->OnUse(this);
	}
}

UMainCharacterInventory* AMainCharacter::GetInventoryComponent()
{
	return MainCharacterInventoryComponent;
}

UDialog* AMainCharacter::GetDialogComponent()
{
	return DialogComponent;
}

void AMainCharacter::InitializeAttributes()
{
	if(AbilitySystemComponent.IsValid() && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		//TODO Later we need to made different levels of abilities so this is one place it needs to be adjusted
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, GetCharacterLevel(), EffectContext);

		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AMainCharacter::GiveAbilities()
{
	if(GetLocalRole() != ROLE_Authority && !AbilitySystemComponent.IsValid())
	{
		return;	
	}
	
	for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			//TODO Later we need to made different levels of abilities so this is one place it needs to be adjusted
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this)
		);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AMainCharacter::GiveStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority && !AbilitySystemComponent.IsValid())
	{
		return;	
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffects : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffects, GetCharacterLevel(), EffectContextHandle);
		if(NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
}


void AMainCharacter::RemoveTag()
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(BlockAbilityUse);
	
}

float AMainCharacter::GetHeath() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AMainCharacter::GetMaxHeath() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AMainCharacter::GetResource() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetResource();
	}
	return 0.0f;
}

float AMainCharacter::GetMaxResource() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxResource();
	}
	return 0.0f;
}

float AMainCharacter::GetHealthRegen() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealthRegen();
	}
	return 0.0f;
}

float AMainCharacter::GetResourceRegen() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetResourceRegen();
	}
	return 0.0f;
}

int32 AMainCharacter::GetExperience() const
{
	if(AttributeSet.IsValid())
	{
		return static_cast<int32>(AttributeSet->GetExperience());
	}
	return 0.0f;
}

int32 AMainCharacter::GetCharacterLevel() const
{
	if(AttributeSet.IsValid())
	{
		return static_cast<int32>(AttributeSet->GetCharacterLevel());
	}
	return 0.0f;
}

float AMainCharacter::GetMoveSpeed() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeed();
	}
	return 0.0f;
}

float AMainCharacter::GetMoveSpeedBaseValue() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue();
	}
	return  0.0f;
}

void AMainCharacter::SetHealth(float Health)
{
	if(AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void AMainCharacter::SetResource(float Resource)
{
	if(AttributeSet.IsValid())
	{
		AttributeSet->SetResource(Resource);
	}
}

void AMainCharacter::RemoveCharacterAbilities()
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for(const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if((Spec.SourceObject == this) && DefaultAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for(int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void AMainCharacter::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	//OnCharacterDied.Broadcast(this);

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	FinishDying();
	
	/*
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
	*/
}

bool AMainCharacter::IsAlive() const
{
	return GetHeath() > 0;
}

void AMainCharacter::FinishDying()
{
	Destroy();
}








