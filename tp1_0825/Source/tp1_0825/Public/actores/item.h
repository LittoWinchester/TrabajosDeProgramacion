// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "item.generated.h"

UCLASS()
class TP1_0825_API Aitem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aitem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category= "STATS", meta = (clampmin ="0", ClampMax="100", UImin="0", UImax ="100", tooltip= "ESTA ES LA ARMADURA PORCENTUAL QUE TENDRA EL PERSONAJE AL INICIAR"))
	int ARMOR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "STATS", meta = (clampmin = "0", ClampMax = "300", UImin = "0", UImax = "300", tooltip ="STRENGHT = FUERZA, Actua modificando directamente el daño que realiza el jugador."))
	int STR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "STATS", meta=(clampmin = "0", ClampMax = "1000", UImin = "0", UImax = "1000", tooltip= "Esta es la vida que tendra el personaje"))
	int HP;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
