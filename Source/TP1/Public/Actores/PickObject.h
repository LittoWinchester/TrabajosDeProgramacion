// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "interface/PlayerInterface.h"
#include "GameFramework/Actor.h"
#include "PickObject.generated.h"

class USphereComponent;
class UStaticMesh;

UCLASS()
class TP1_API APickObject : public AActor, public IPlayerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickObject();


	//Creando componentes

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
