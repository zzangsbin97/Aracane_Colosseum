// Copyright Epic Games, Inc. All Rights Reserved.

#include "Aracane_ColosseumPickUpComponent.h"

UAracane_ColosseumPickUpComponent::UAracane_ColosseumPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UAracane_ColosseumPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UAracane_ColosseumPickUpComponent::OnSphereBeginOverlap);
}

void UAracane_ColosseumPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AAracane_ColosseumCharacter* Character = Cast<AAracane_ColosseumCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
