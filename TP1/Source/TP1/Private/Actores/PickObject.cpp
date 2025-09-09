// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/PickObject.h"
#include "Components/SphereComponent.h"

// Sets default values
APickObject::APickObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Creacion componente
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SphereMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APickObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

