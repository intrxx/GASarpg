// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Dialog.generated.h"


struct FDialogMain;

USTRUCT(BlueprintType)
struct FSubtitle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Subtitle;

	UPROPERTY(EditAnywhere)
	UDataTable* NewSubtitleTable;
	
};

USTRUCT(BlueprintType)
struct FDialogM : public FTableRowBase
{
	GENERATED_BODY()

	// Subs that are displayed for given time
	UPROPERTY(EditAnywhere)
	FString NPCSubtitle;

	UPROPERTY(EditAnywhere)
	TArray<FSubtitle> Subtitles;

	UPROPERTY(EditAnywhere)
	bool bShouldLastOptionQuit;
};

class UDialogWidget;
class ANPC;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJEKT_API UDialog : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDialog();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*Generates the player lines*/
	void GeneratePlayerLines(UDataTable& PlayerLines);

	UPROPERTY(BlueprintReadOnly)
	FString NPCMessage;
 
	/*This array is essentially an Array of Excerpts from our dialogs!*/
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Questions;
 
	/*Performs the actual talking - informs the associated pawn if necessary in order to answer
	The subtitles array contain all the subtitles for this talk - it should be passed to our UI*/
	UFUNCTION(BlueprintCallable, Category = DialogSystem)
	void Talk(FString Excerpt);
 
	/*Enables / disables our talk ability. The player can't talk if he's not in a valid range*/
	void SetTalkRangeStatus(bool Status) { bIsInTalkRange = Status; }
 
	/*Sets a new associated pawn*/
	void SetAssociatedPawn(ANPC* Pawn) { AssociatedPawn = Pawn; }
 
	/*Retrieves the UI reference*/
	UDialogWidget* GetUI() { return UI; }

	/*Initiates or terminates a conversation*/
	void ToggleTalking();

private:
	/*True if the player is currently talking with any pawn*/
	bool bIsTalking;
 
	/*True if the player is inside a valid range to start talking to a pawn*/
	bool bIsInTalkRange;
 
	/*The pawn that the player is currently talking to*/
	ANPC* AssociatedPawn;
 
	/*A reference to our lines - retrieved from the associated pawn*/
	UDataTable* AvailableLines;
 
	/*Searches in the given row inside the specified table*/
	FDialogM* RetrieveDialog(UDataTable* TableToSearch, FName RowName);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogWidget> DialogClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
 
	/*Opens or closes the conversation UI*/
	UFUNCTION(Category = DialogSystem)
	void ToggleUI();
 
	/*UI Reference*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDialogWidget* UI;
		
};
