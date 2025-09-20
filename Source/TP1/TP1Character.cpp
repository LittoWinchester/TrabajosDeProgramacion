// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP1Character.h"
#include "Engine/LocalPlayer.h"
#include "Public/Actores/PickObject.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TP1.h"

ATP1Character::ATP1Character()
{
	// Obtiene el componente de colisi�n "Capsule" y define su tama�o (radio y altura).
	// Este componente es el que delimita la "caja de colisiones" del personaje en el mundo.
	// Pod�s cambiar 42.f (ancho) y 96.f (alto) para hacer al personaje m�s ancho/angosto o m�s alto/bajo.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Desactiva que el personaje rote con el controlador en Pitch (mirar arriba/abajo).
	bUseControllerRotationPitch = false;
	// Desactiva que el personaje rote con el controlador en Yaw (mirar izquierda/derecha).
	bUseControllerRotationYaw = false;
	// Desactiva que el personaje rote con el controlador en Roll (inclinarse de lado).
	bUseControllerRotationRoll = false;

	// Configura que la rotaci�n del personaje se oriente autom�ticamente hacia la direcci�n en la que se mueve.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Define la velocidad de rotaci�n del personaje. Un valor mayor hace que gire m�s r�pido hacia donde se mueve.
	// Pod�s cambiar 500.0f para notar si gira lento o inmediato.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Velocidad inicial al saltar. Cuanto mayor sea el valor, m�s alto salta el personaje.
	GetCharacterMovement()->JumpZVelocity = 500.f;

	// Control del personaje en el aire. Si aument�s este valor (ej: 1.0f), podr�s moverlo mucho m�s f�cil en el aire.
	GetCharacterMovement()->AirControl = 0.35f;

	// Velocidad m�xima de caminar. Si baj�s el valor, se mueve m�s lento; si lo sub�s, m�s r�pido.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	// Velocidad m�nima al caminar con joystick anal�gico. Ajusta la sensibilidad al caminar lento.
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	// Qu� tan r�pido se frena el personaje al dejar de moverse mientras camina.
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Qu� tan r�pido se frena el personaje cuando cae en el aire.
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Crea un "Camera Boom" (brazo de resorte) que mantiene la c�mara a cierta distancia del personaje.
	// Este componente hace que la c�mara se aleje autom�ticamente si choca con paredes.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	// Adjunta el "Camera Boom" al componente ra�z del personaje (normalmente la c�psula).
	CameraBoom->SetupAttachment(RootComponent);

	// Define la distancia de la c�mara al personaje. Si aument�s el valor, la c�mara se aleja m�s.
	CameraBoom->TargetArmLength = 400.0f;

	// Permite que la c�mara gire siguiendo el control del jugador (mouse/joystick).
	CameraBoom->bUsePawnControlRotation = true;



	// Crea la c�mara que seguir� al personaje.
	// Este componente es la "c�mara principal" por defecto desde la que el jugador ve el mundo.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// Adjunta la c�mara al extremo del "CameraBoom".
	// El segundo par�metro (USpringArmComponent::SocketName) indica que la c�mara se posiciona en el "socket" final del brazo.
	// Esto permite que la c�mara mantenga la distancia definida en TargetArmLength del boom.
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Desactiva que la c�mara rote directamente con la rotaci�n del controlador.
	// En vez de eso, la c�mara seguir� la rotaci�n aplicada al "CameraBoom".
	// Si lo pusieras en true, la c�mara rotar�a directamente con el input, ignorando el boom.
	FollowCamera->bUsePawnControlRotation = false;

	// NOTA: El mesh del personaje se configura desde el Blueprint "ThirdPersonCharacter".
	// Esto evita tener referencias directas a archivos de contenido en C++ y permite cambiar el modelo o la animaci�n en el editor.
}
// conecta las acciones del jugador (inputs) con las funciones del personaje.
void ATP1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Configura las acciones de entrada (teclas, mouse, joystick).
	// Esta funci�n se ejecuta al inicializar el personaje y enlaza los inputs a funciones.
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Asocia la acci�n "JumpAction" para cuando se presiona el bot�n (Started).
		// Llama a la funci�n Jump() de ACharacter para que el personaje salte.
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);

		// Asocia la misma acci�n "JumpAction" para cuando se suelta el bot�n (Completed).
		// Llama a StopJumping() para que el salto termine (�til en saltos de altura variable).
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Asocia la acci�n "MoveAction" para cuando se activa (Triggered).
		// Llama a la funci�n Move de este personaje (ATP1Character), donde se define c�mo se mueve.
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP1Character::Move);

		// Asocia la acci�n "MouseLookAction" para cuando se activa.
		// Llama a la funci�n Look de este personaje, que controla la rotaci�n de c�mara con el mouse.
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATP1Character::Look);

		// Asocia la acci�n "LookAction" (puede venir de joystick derecho u otro input configurado).
		// Llama a la misma funci�n Look para girar la c�mara.
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP1Character::Look);


		//Tecla E del ejercicio

		PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &ATP1Character::AccionInteraccion);
		EnhancedInputComponent->BindAction(InteraccionAction, ETriggerEvent::Started, this, &ATP1Character::AccionInteraccion);	

		// tecla shift ejercicio, correr.
		EnhancedInputComponent->BindAction(CorrerAction, ETriggerEvent::Started, this, &ATP1Character::Correr);
		EnhancedInputComponent->BindAction(CorrerAction, ETriggerEvent::Completed, this, &ATP1Character::Correr);

		// click izquiero del ejercicio.
		EnhancedInputComponent->BindAction(SpawnBolaAction, ETriggerEvent::Started, this, &ATP1Character::SpawnBola);



		

		////
	}
	else
	{
		// Si no encuentra un EnhancedInputComponent, lanza un error en el log.
		// Esto pasa si el proyecto intenta usar el sistema viejo de Input en lugar del nuevo "Enhanced Input".
		UE_LOG(LogTP1, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
// Estas funciones son las que reciben el input ya procesado 
// por el sistema de Enhanced Input y lo env�an a las funciones que realmente ejecutan la l�gica
void ATP1Character::Move(const FInputActionValue& Value)
{
	// El par�metro "Value" contiene el input recibido de la acci�n configurada (MoveAction).
	// En este caso, es un Vector2D (eje X = izquierda/derecha, eje Y = adelante/atr�s).

	// Convierte el valor de entrada en un FVector2D para poder usarlo.
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Env�a los valores del vector a la funci�n DoMove().
	// MovementVector.X controla el eje horizontal (izquierda/derecha).
	// MovementVector.Y controla el eje vertical (adelante/atr�s).
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATP1Character::Look(const FInputActionValue& Value)
{
	// El par�metro "Value" contiene el input recibido de la acci�n configurada (LookAction o MouseLookAction).
	// Tambi�n es un Vector2D (eje X = movimiento del mouse/joystick horizontal, eje Y = movimiento vertical).

	// Convierte el valor de entrada en un FVector2D.
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Env�a los valores del vector a la funci�n DoLook().
	// LookAxisVector.X controla la rotaci�n izquierda/derecha (Yaw).
	// LookAxisVector.Y controla la rotaci�n arriba/abajo (Pitch).
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

// l�gica pura del movimiento, la c�mara y el salto.
void ATP1Character::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr) // Verifica que el personaje tenga un controlador (jugador o IA).
	{
		// Obtiene la rotaci�n actual del controlador (c�mara del jugador).
		const FRotator Rotation = GetController()->GetControlRotation();

		// Crea un rotador que solo tenga en cuenta el eje Yaw (giro izquierda/derecha).
		// Se descartan Pitch (arriba/abajo) y Roll (inclinaci�n lateral) porque no influyen en el movimiento en el suelo.
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Calcula el vector de direcci�n "adelante" en el mundo seg�n la c�mara.
		// Esto permite que al presionar "adelante" el personaje vaya hacia donde est� mirando la c�mara.
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Calcula el vector de direcci�n "derecha" en el mundo seg�n la c�mara.
		// Esto permite que al presionar "derecha" el personaje se mueva lateralmente respecto a la c�mara.
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Aplica movimiento hacia adelante/atr�s seg�n el input recibido.
		AddMovementInput(ForwardDirection, Forward);

		// Aplica movimiento hacia la derecha/izquierda seg�n el input recibido.
		AddMovementInput(RightDirection, Right);
	}
}

void ATP1Character::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr) // Solo funciona si hay un controlador activo.
	{
		// Suma input de rotaci�n horizontal (Yaw).
		// Controla girar la c�mara/personaje a izquierda/derecha.
		AddControllerYawInput(Yaw);

		// Suma input de rotaci�n vertical (Pitch).
		// Controla mover la c�mara arriba/abajo.
		AddControllerPitchInput(Pitch);
	}
}

void ATP1Character::DoJumpStart()
{
	// Llama a la funci�n Jump() de ACharacter.
	// Hace que el personaje empiece a saltar.
	Jump();
}

void ATP1Character::DoJumpEnd()
{
	// Llama a StopJumping() para terminar el salto.
	// Esto permite saltos de altura variable (si se mantiene la tecla salta m�s alto).
	StopJumping();
}


//Tecla E para hacer la accion 

void ATP1Character::AccionInteraccion()
{
		// print string
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				TEXT("Tecla de interaccion"));
		}
	

}


// funcion para correr, un bool que cuando se aprieta shift se cambia velocidad a 1200 y si se suelta va a 300
void ATP1Character::Correr(const FInputActionValue& Value)
{
	bool bShiftPresionado = Value.Get<bool>();
	GetCharacterMovement()->MaxWalkSpeed = bShiftPresionado ? 1200.f : 300.f;
}





// funcion para spawnear una bola con el click izq



void ATP1Character::SpawnBola(const FInputActionValue& Value)
{
	// Verificar que la clase est� asignada
	if (!PickObjectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: PickObjectClass no est� definido"));
		return;
	}

	// Verificar que el mundo exista
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: No se pudo obtener el mundo"));
		return;
	}

	// Calcular posici�n y rotaci�n
	FVector Location = GetActorLocation() + GetActorForwardVector() * 100.f;
	FRotator Rotation = GetActorRotation();

	// Intentar spawnear
	APickObject* SpawnedActor = World->SpawnActor<APickObject>(PickObjectClass, Location, Rotation);
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: No se pudo spawnear PickObject"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn exitoso de PickObject"));
	}
}



