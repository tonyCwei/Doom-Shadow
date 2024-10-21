// Fill out your copyright notice in the Description page of Project Settings.


#include "Scanner.h"
#include "Components/PostProcessComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AScanner::AScanner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComp"));
	RootComponent = PostProcessComp;

	scanTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScanTimeline"));
}

// Called when the game starts or when spawned
void AScanner::BeginPlay()
{
	Super::BeginPlay();

	if (scanSound) {
		UGameplayStatics::PlaySound2D(this, scanSound);
	}
	
	if (FloatCurve) {
		// Bind the update function
		FOnTimelineFloat UpdateFunction;
		UpdateFunction.BindUFunction(this, FName("scanTimelineUpdate"));

		// Bind the finished function
		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindUFunction(this, FName("scanTimelineFinished"));

		// Add functions to the timeline
		scanTimeline->AddInterpFloat(FloatCurve, UpdateFunction);
		scanTimeline->SetTimelineFinishedFunc(FinishedFunction);

		// Set the timeline to loop or play once
		scanTimeline->SetLooping(false);

	}


	scanTimeline->PlayFromStart();


	
}

void AScanner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(destoryTimer);
}

void AScanner::scanTimelineUpdate(float Value)
{
	if (MaterialParameterCollection)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, MaterialParameterCollection, FName("Radius"), Value);
	}
}

void AScanner::scanTimelineFinished()
{
	GetWorldTimerManager().SetTimer(destoryTimer, [&]() {
		Destroy();
		}, hightlightDuration, false);
}


