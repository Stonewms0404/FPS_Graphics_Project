// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "FPS_Graphics_ProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture");
	rootArrow = CreateDefaultSubobject<UArrowComponent>("RootArrow");

	RootComponent = boxComp;

	mesh->SetupAttachment(boxComp);
	sceneCapture->SetupAttachment(mesh);
	rootArrow->SetupAttachment(RootComponent);

	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

}

// Called when the game starts or when spawned
void APortal::BeginPlay() {
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	mesh->SetHiddenInSceneCapture(true);
	mesh->CastShadow = 0;

	if (mat) {
		mesh->SetMaterial(0, mat);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdatePortals();
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBoxyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AFPS_Graphics_ProjectCharacter* playerChar = Cast<AFPS_Graphics_ProjectCharacter>(OtherActor);
	
	if (playerChar) {
		if (OtherPortal) {
			if (!playerChar->isTeleporting) {
				playerChar->isTeleporting = true;
				FVector loc = OtherPortal->rootArrow->GetComponentLocation();
				playerChar->SetActorLocation(loc);

				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, "SetBool", playerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);
			}
		}
	}
}

void APortal::SetBool(AFPS_Graphics_ProjectCharacter* playerChar) {
	if (playerChar) {
		playerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals() {
	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation();
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();
	FVector CombinedLocation = camLocation + Location;

	sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation);
}

