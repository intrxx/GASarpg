// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OilSpawner.generated.h"

class ATrapOil;
class USplineComponent;

UCLASS()
class PROJEKT_API AOilSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOilSpawner();


protected:

	UFUNCTION(CallInEditor, Category="Spawn Functions")
	void SpawnActors();

	UFUNCTION(CallInEditor, Category="Spawn Functions")
	void DeleteActors();

	// Calculates the biggest and smallest vector values that the polygon has
	UFUNCTION()
	void CalculateMinMaxLocation();
	//void CalculateMinMaxLocation(FVector& MinLocation, FVector& MaxLocation);
	
	// Function that checks if our chosen point is inside a polygon
	UFUNCTION()
	bool IsInsidePolygon(FVector Point);

	// Checks intersection of our chosen point
	// Implemented algorithm https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
	UFUNCTION()
	bool IsIntersecting(FVector P1, FVector Q1, FVector P2, FVector Q2);

	// Checks point orientation as described in th algorithm ^
	UFUNCTION()
	int CheckOrientation(FVector P, FVector Q, FVector R);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ATrapOil> OilTrapClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent* Spline;
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	TArray<ATrapOil*> TrapsArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spacing Variables")
	float Spacing = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spacing Variables")
	float DistanceX = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spacing Variables")
	float DistanceY = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spacing Variables")
	float VolumeHeight = 200.0f;

	FVector CurrentPoint;

	FVector PointMin;

	FVector PointMax;

	float TempX;

	float TempY;
};
