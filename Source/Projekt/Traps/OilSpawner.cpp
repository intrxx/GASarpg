// Fill out your copyright notice in the Description page of Project Settings.


#include "OilSpawner.h"

#include "TrapOil.h"
#include "Components/SplineComponent.h"

// Sets default values
AOilSpawner::AOilSpawner()
{
	Spline = CreateDefaultSubobject<USplineComponent>(FName("Spline"));
}


void AOilSpawner::SpawnActors()
{
	if(TrapsArray.IsEmpty())
	{
		CalculateMinMaxLocation();
		
		TempY = PointMin.Y;
		
		while (TempY < PointMax.Y)
		{
			TempX = PointMin.X;
			while (TempX < PointMax.X)
			{
				CurrentPoint = FVector(TempX, TempY, PointMax.Z);

				if(IsInsidePolygon(CurrentPoint))
				{
					FVector StartTrace = CurrentPoint + FVector(0,0,VolumeHeight);
					FVector EndTrace = CurrentPoint - FVector(0,0,VolumeHeight);
					FHitResult HitResult;
					FCollisionQueryParams QueryParams;
					
					// For point visibility
					DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5.0f);

					GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility,
						QueryParams);

					FActorSpawnParameters SpawnParameters;
					
					TrapsArray.Add(GetWorld()->SpawnActor<ATrapOil>(OilTrapClass, HitResult.Location, FRotator(0,0,0), SpawnParameters));
				}
				TempX += DistanceX;
			}
			TempY += DistanceY;
		}	
	}
	// Just delete last actors and spawn new ones
	else
	{
		DeleteActors();
		SpawnActors();
	}
}

void AOilSpawner::DeleteActors()
{
	for (auto Trap : TrapsArray)
	{
		Trap->Destroy();
	}
	TrapsArray.Empty();
}

void AOilSpawner::CalculateMinMaxLocation()
{
	float Distance = 0.0f;
	FVector TempPointMin = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector TempPointMax = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	while (Distance < Spline->GetSplineLength())
	{
		TempPointMin = FVector(
			FMath::Min(Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).X, TempPointMin.X),
			FMath::Min(Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).Y, TempPointMin.Y),
			FMath::Min(Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).Z, TempPointMin.Z));

		TempPointMax = FVector(
			FMath::Max(Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).X, TempPointMax.X),
			FMath::Max(Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).Y, TempPointMax.Y),
			FMath::Max(Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).Z, TempPointMax.Z));

		Distance += Spacing;
	}
	PointMin = TempPointMin;
	PointMax = TempPointMax;
}

bool AOilSpawner::IsInsidePolygon(FVector Point)
{
	uint8 IntersectionCount = 0;
	for(int i = 0; i <= Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		if(IsIntersecting(
			Point,
			PointMin + FVector(-100.0f, 0, 0),
			Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
			Spline->GetLocationAtSplinePoint((i + 1) % Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World)
			))
		{
			IntersectionCount++;
		}
	}
	return IntersectionCount % 2 != 0;
}

bool AOilSpawner::IsIntersecting(FVector P1, FVector Q1, FVector P2, FVector Q2)
{
	int o1 = CheckOrientation(P1, Q1, P2);
	int o2 = CheckOrientation(P1, Q1, Q2);
	int o3 = CheckOrientation(P2, Q2, P1);
	int o4 = CheckOrientation(P2, Q2, Q1);


	return o1 != o2 && o3 != o4;
}

int AOilSpawner::CheckOrientation(FVector P, FVector Q, FVector R)
{
	float Result = (Q.Y - P.Y) * (R.X - Q.X) - (Q.X - P.X) * (R.Y - Q.Y);

	// Collinear
	if(Result == 0)
	{
		return 0;
	}
	// Clockwise
	if (Result > 0)
	{
		return 1;
	}
	// Counter clockwise
	if (Result < 0)
	{
		return 2;
	}
	// Should never reach this point
	return 1;
}

