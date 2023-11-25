// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsWidget.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

bool UOptionsWidget::Initialize()
{
	Super::Initialize();

	UserSettings = UGameUserSettings::GetGameUserSettings();
	
	ComboBoxStringWindow->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringWindowUpdate);
	ComboBoxStringResolution->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringResolutionUpdate);
	ComboBoxStringFPS->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringFPSUpdate);
	ComboBoxStringVSync->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringVSyncUpdate);
	ComboBoxStringTexture->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringTextureUpdate);
	ComboBoxStringShadows->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringShadowsUpdate);
	ComboBoxStringAA->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringAAUpdate);
	ComboBoxStringPP->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::ComboBoxStringPPUpdate);
	
	//UserSettings->LoadSettings();
	UpdateTooltips();
//		UserSettings->ApplySettings(false);

	bIsResDirty = false;
	bIsOtherDirty = false;
	
	return true;
}

void UOptionsWidget::UpdateTooltips()
{
	// Could probably refactor this but tbh seems like pointless work
	
	if(UserSettings->GetFullscreenMode() == EWindowMode::Fullscreen)
	{
		ComboBoxStringWindow->SetSelectedIndex(0);
	}
	else if(UserSettings->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
	{
		ComboBoxStringWindow->SetSelectedIndex(1);
	}
	else if(UserSettings->GetFullscreenMode() == EWindowMode::Windowed)
	{
		ComboBoxStringWindow->SetSelectedIndex(2);
	}
	
	if(UserSettings->GetScreenResolution() == FIntPoint(1920, 1080))
	{
		ComboBoxStringResolution->SetSelectedIndex(0);
	}
	else if(UserSettings->GetScreenResolution() == FIntPoint(1280, 720))
	{
		ComboBoxStringResolution->SetSelectedIndex(1);
	}
	else if(UserSettings->GetScreenResolution() == FIntPoint(720, 480))
	{
		ComboBoxStringResolution->SetSelectedIndex(2);
	}

	if(UserSettings->GetFrameRateLimit() == 0)
	{
		ComboBoxStringFPS->SetSelectedIndex(4);
	}
	else if(UserSettings->GetFrameRateLimit() == 240.0f)
	{
		ComboBoxStringFPS->SetSelectedIndex(3);
	}
	else if(UserSettings->GetFrameRateLimit() == 120.0f)
	{
		ComboBoxStringFPS->SetSelectedIndex(2);
	}
	else if(UserSettings->GetFrameRateLimit() == 60.0f)
	{
		ComboBoxStringFPS->SetSelectedIndex(1);
	}
	else if(UserSettings->GetFrameRateLimit() == 30.0f)
	{
		ComboBoxStringFPS->SetSelectedIndex(0);
	}
	
	if(UserSettings->IsVSyncEnabled())
	{
		ComboBoxStringVSync->SetSelectedIndex(0);
	}
	else
	{
		ComboBoxStringVSync->SetSelectedIndex(1);
	}
	
	ComboBoxStringTexture->SetSelectedIndex(UserSettings->GetTextureQuality());
	ComboBoxStringShadows->SetSelectedIndex(UserSettings->GetShadowQuality());
	ComboBoxStringAA->SetSelectedIndex(UserSettings->GetAntiAliasingQuality());
	ComboBoxStringPP->SetSelectedIndex(UserSettings->GetPostProcessingQuality());
}

void UOptionsWidget::ComboBoxStringWindowUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("Fullscreen"))
	{
		UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		bIsResDirty = true;
	}
	else if(SelectedItem.Equals("Borderless"))
	{
		UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		bIsResDirty = true;
	}
	else if(SelectedItem.Equals("Windowed"))
	{
		UserSettings->SetFullscreenMode(EWindowMode::Windowed);
		bIsResDirty = true;
	}
}

void UOptionsWidget::ComboBoxStringResolutionUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("1920 x 1080"))
	{
		UserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		bIsResDirty = true;
	}
	else if(SelectedItem.Equals("1280 x 720"))
	{
		UserSettings->SetScreenResolution(FIntPoint(1280, 720));
		bIsResDirty = true;
	}
	else if(SelectedItem.Equals("720 x 480"))
	{
		UserSettings->SetScreenResolution(FIntPoint(720, 480));
		bIsResDirty = true;
	}
}

void UOptionsWidget::ComboBoxStringFPSUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("Unlimited"))
	{
		UserSettings->SetFrameRateLimit(0.0f);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("240 FPS"))
	{
		UserSettings->SetFrameRateLimit(240.0f);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("120 FPS"))
	{
		UserSettings->SetFrameRateLimit(120.0f);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("60 FPS"))
	{
		UserSettings->SetFrameRateLimit(60.0f);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("30 FPS"))
	{
		UserSettings->SetFrameRateLimit(30.0f);
		bIsOtherDirty = true;
	}
}

void UOptionsWidget::ComboBoxStringVSyncUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("Enabled"))
	{
		UserSettings->SetVSyncEnabled(true);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Disabled"))
	{
		UserSettings->SetVSyncEnabled(false);
		bIsOtherDirty = true;
	}
}

void UOptionsWidget::ComboBoxStringTextureUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange,FString::Printf(TEXT("%d"), UserSettings->GetTextureQuality()));
	if(SelectedItem.Equals("Epic"))
	{
		UserSettings->SetTextureQuality(3);
		ComboBoxStringTexture->SetSelectedIndex(3);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("High"))
	{
		UserSettings->SetTextureQuality(2);
		ComboBoxStringTexture->SetSelectedIndex(2);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Medium"))
	{
		UserSettings->SetTextureQuality(1);
		ComboBoxStringTexture->SetSelectedIndex(1);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Low"))
	{
		UserSettings->SetTextureQuality(0);
		ComboBoxStringTexture->SetSelectedIndex(0);
		bIsOtherDirty = true;
	}
//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,FString::Printf(TEXT("%d"), UserSettings->GetTextureQuality()));
}

void UOptionsWidget::ComboBoxStringShadowsUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("Epic"))
	{
		UserSettings->SetShadowQuality(3);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("High"))
	{
		UserSettings->SetShadowQuality(2);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Medium"))
	{
		UserSettings->SetShadowQuality(1);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Low"))
	{
		UserSettings->SetShadowQuality(0);
		bIsOtherDirty = true;
	}
}

void UOptionsWidget::ComboBoxStringAAUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("Epic"))
	{
		UserSettings->SetAntiAliasingQuality(3);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("High"))
	{
		UserSettings->SetAntiAliasingQuality(2);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Medium"))
	{
		UserSettings->SetAntiAliasingQuality(1);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Low"))
	{
		UserSettings->SetAntiAliasingQuality(0);
		bIsOtherDirty = true;
	}
}

void UOptionsWidget::ComboBoxStringPPUpdate(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals("Epic"))
	{
		UserSettings->SetPostProcessingQuality(3);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("High"))
	{
		UserSettings->SetPostProcessingQuality(2);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Medium"))
	{
		UserSettings->SetPostProcessingQuality(1);
		bIsOtherDirty = true;
	}
	else if(SelectedItem.Equals("Low"))
	{
		UserSettings->SetPostProcessingQuality(0);
		bIsOtherDirty = true;
	}
}

void UOptionsWidget::BackPressed()
{
	if(!bIsResDirty && !bIsOtherDirty)
	{
		RemoveFromParent();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Nofing"));
	}
	else if(bIsResDirty && ResolutionWidget)
	{
		UserSettings->ApplyResolutionSettings(false);
		CreateWidget(GetWorld()->GetFirstPlayerController(), ResolutionWidget)->AddToViewport();
		
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Res"));
	}
	else if(ConfirmWidget)
	{
		CreateWidget(GetWorld()->GetFirstPlayerController(), ConfirmWidget)->AddToViewport();
		
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Other"));
	}
}

void UOptionsWidget::DefaultsPressed()
{
	UserSettings->RunHardwareBenchmark();
	UserSettings->ApplyHardwareBenchmarkResults();
	
	bIsOtherDirty = true;

	if(UserSettings->IsFullscreenModeDirty())
	{
		bIsResDirty = true;
	}
	
	UpdateTooltips();
}
