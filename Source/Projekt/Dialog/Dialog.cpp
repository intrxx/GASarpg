// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialog.h"

#include "DialogWidget.h"
#include "Projekt/MainCharacter/MainCharacter.h"
#include "Projekt/AI/NPC.h"

// Sets default values for this component's properties
UDialog::UDialog()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsTalking = false;
	bIsInTalkRange = false;
	AssociatedPawn = nullptr;
}

// Called when the game starts
void UDialog::BeginPlay()
{
	Super::BeginPlay();

	if(DialogClass)
	{
		UI = Cast<UDialogWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), DialogClass));
	}
}

// Called every frame
void UDialog::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Changes Question array to new lines or quits if there is no new dialog table
void UDialog::Talk(FString Excerpt)
{
	GeneratePlayerLines(*AssociatedPawn->PlayerDialog);
	
	//Get all the row names based on our stored lines
	TArray<FName> PlayerOptions = AvailableLines->GetRowNames();
 
	for (auto It : PlayerOptions)
	{
		//Search inside the available lines table to find the pressed Excerpt from the UI
		FDialogM* Dialog = RetrieveDialog(AvailableLines, It);

		for (auto DialogSubtitle : Dialog->Subtitles)
		{
			if(DialogSubtitle.Subtitle == Excerpt)
			{
				FDialogM* NewDialog = RetrieveDialog(DialogSubtitle.NewSubtitleTable, It);
				if(NewDialog)
				{
					AssociatedPawn->PlayerDialog = DialogSubtitle.NewSubtitleTable;

					UI->UpdateSubtitles(*NewDialog);
				}
				else
				{
					UI->RemoveFromViewport();
				}
			}
		}
	}
}

// Casts DataTable to a table with Dialog IMPORTANT KEEP ROW NAMES AS '1'
FDialogM* UDialog::RetrieveDialog(UDataTable* TableToSearch, FName RowName)
{
	if(!TableToSearch) return nullptr;
 
	//The table is valid - retrieve the given row if possible
	FString ContextString;
	return TableToSearch->FindRow<FDialogM>(RowName, ContextString);
}

// Checks for associated pawn that we want to talk to and our distance to the pawn
void UDialog::ToggleTalking()
{
	if (bIsInTalkRange)
	{
		//If we are in talk range handle the talk status and the UI
		bIsTalking = !bIsTalking;
		ToggleUI();
		if (bIsTalking && AssociatedPawn)
		{
			//The associated pawn is polite enough to face us when we talk to him!
			FVector Location = AssociatedPawn->GetActorLocation();
			FVector TargetLocation = GetOwner()->GetActorLocation();
 
			AssociatedPawn->SetActorRotation((TargetLocation - Location).Rotation());
		}
	}
}

// Populates array with string lines to update UI with
void UDialog::GeneratePlayerLines(UDataTable& PlayerLines)
{
	//Get all the row names of the table
	TArray<FName> PlayerOptions = PlayerLines.GetRowNames();
 
	//For each row name try to retrieve the contents of the table
	for (auto It : PlayerOptions)
	{
		//Retrieve the contents of the table
		FDialogM* Dialog = RetrieveDialog(&PlayerLines, It);

		NPCMessage = Dialog->NPCSubtitle;
 
		if (Dialog)
		{
			//We retrieved a valid row - populate the questions array with our excerpts
			for (auto Subtitle : Dialog->Subtitles)
			{
				Questions.Add(Subtitle.Subtitle);
			}
		}
	}
	//Make sure to create a reference of the available line for later use
	AvailableLines = &PlayerLines;
}

void UDialog::ToggleUI()
{
	if(!UI) { return; }

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwner());

	if(UI->IsInViewport() && MainCharacter)
	{
		UI->RemoveFromParent();
	}
	else
	{
		UI->Questions = Questions;
		UI->UpdateSubtitles(*RetrieveDialog(AssociatedPawn->FirstDialog, FName(TEXT("1"))));
	}
}


