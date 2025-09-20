// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "Public/Actores/PickObject.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TP1Character.generated.h"


// Sirve para decirle al compilador "esta clase existe", sin necesidad de incluir todo su header aqu�.
// Se usa para el componente que extiende un brazo entre el personaje y la c�mara.
class USpringArmComponent;
// Es la clase que maneja la c�mara del personaje.
class UCameraComponent;
// Forma parte del sistema "Enhanced Input".
class UInputAction;
// Es un contenedor que guarda el valor de la acci�n (por ejemplo: un vector2D de movimiento).
struct FInputActionValue;


// Declara una categor�a de log llamada "LogTemplateCharacter".
// Esto permite imprimir mensajes en la consola/log de Unreal con diferentes niveles (Log, Warning, Error).
// "Log, All" significa que se aceptan todos los niveles de mensajes.
// Se usa m�s abajo en el cpp con UE_LOG(LogTemplateCharacter, Log, TEXT("..."));
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */

 // Macro que marca a esta clase como UCLASS (propia de Unreal).
 // "abstract" significa que no se puede instanciar directamente en el editor (se debe derivar de ella).
 // Sirve para usarla como base y crear versiones Blueprint.
UCLASS(abstract)
class ATP1Character : public ACharacter
{
	// Macro necesaria en todas las clases que heredan de UObject.
	// Genera c�digo que Unreal necesita para reflexion, replicaci�n en red y blueprints.
	GENERATED_BODY()

	/** Brazo de c�mara (CameraBoom) que posiciona la c�mara detr�s del personaje */
	// VisibleAnywhere = se ve en el editor, pero no se puede editar.
	// BlueprintReadOnly = visible en Blueprints, pero no modificable desde ah�.
	// Category = "Components" agrupa esta variable en el panel del editor.
	// meta = AllowPrivateAccess="true" permite que, aunque est� en privado, Unreal pueda acceder desde el editor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** C�mara que sigue al personaje */
	// Se expone con las mismas reglas que el CameraBoom.
	// Es la c�mara principal que se adjunta al boom y define la vista del jugador.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected: // Todo lo que est� ac� solo se puede acceder desde la propia clase o clases hijas.

	/** Acci�n de input para saltar */
	// EditAnywhere = se puede configurar en el editor y en blueprints.
	// Esto permite asignarle la acci�n creada en el sistema Enhanced Input.
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Acci�n de input para moverse */
	// Igual que JumpAction, pero esta se usar� para el movimiento en eje X/Y.
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Acci�n de input para mirar con joystick */
	// Usada normalmente en consolas o gamepads.
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Acci�n de input para mirar con mouse */
	// Diferente a LookAction para distinguir entre joystick y rat�n.
	// Esto permite configurar sensibilidad distinta o comportamientos separados.
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;
public:

	/** Constructor */
	// Se ejecuta cuando se crea un objeto de esta clase.
	// Ac� se inicializan los componentes (CameraBoom, FollowCamera, etc.).
	ATP1Character();

protected:

	/** Inicializa el binding de acciones de input */
	// Sobrescribe la funci�n de ACharacter.
	// Se ejecuta al inicio y enlaza los inputs (Jump, Move, Look) con funciones del personaje.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Llamada para manejar el input de movimiento */
	// Esta funci�n recibe un FInputActionValue (vector 2D) desde Enhanced Input.
	// Luego llama a DoMove() para aplicar el movimiento.
	void Move(const FInputActionValue& Value);

	/** Llamada para manejar el input de c�mara/mirada */
	// Igual que Move, pero recibe un FInputActionValue (vector 2D de rotaci�n).
	// Luego llama a DoLook() para aplicar la rotaci�n.
	void Look(const FInputActionValue& Value);

public:

	/** Maneja inputs de movimiento desde controles o interfaces de UI */
	// UFUNCTION(BlueprintCallable) = puede ser llamada desde Blueprints.
	// Par�metros: Right (X), Forward (Y).
	// Se marca virtual para permitir sobreescribir en clases derivadas.
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Maneja inputs de c�mara desde controles o UI */
	// BlueprintCallable permite invocar esta funci�n desde Blueprints 
	// Par�metros: Yaw = izquierda/derecha, Pitch = arriba/abajo.
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Maneja inicio del salto desde controles o UI */
	// Se llama cuando se presiona el bot�n de saltar.
	// Internamente ejecuta Jump().
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Maneja fin del salto desde controles o UI */
	// Se llama cuando se suelta el bot�n de saltar.
	// Internamente ejecuta StopJumping().
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();


public:

	/** Devuelve el subobjeto CameraBoom */
	// FORCEINLINE = le indica al compilador que inserte esta funci�n directamente donde se llama (sin overhead de funci�n).
	// Devuelve un puntero a USpringArmComponent, permitiendo acceder al CameraBoom desde otros c�digos o blueprints.
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Devuelve el subobjeto FollowCamera */
	// Igual que GetCameraBoom, pero devuelve la c�mara que sigue al personaje.
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }








	// EJERCICIO B // 

protected:
	// Acci�n para interactuar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteraccionAction;

	//correr shift
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CorrerAction;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class APickObject> PickObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SpawnBolaAction;



protected:
	// Funci�n que se ejecuta al presionar E
	UFUNCTION()
	void AccionInteraccion();

	UFUNCTION()
	void Correr(const FInputActionValue& Value);

	void SpawnBola(const FInputActionValue& Value);
	

};







