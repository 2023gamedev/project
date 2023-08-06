// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponet()::UTriggerComponet()
{
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

   // UE_LOG(LogTemp, Display, TEXT("Trigger Componet Alive"));
	
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // UE_LOG(LogTemp, Display, TEXT("Trigger Component is Ticking"));


    // if(Actors.Num() > 0){
    //     FString ActorName = Actors[0]->GetActorNameOrLabel();
    //    UE_LOG(LogTemp, Display, TEXT("Overlapping : %s"), *ActorName);
    // }

    // int32 index = 0;
    // while (index < Actors.Num())
    // {
    //     String ActorName = Actors[index]->GetActorNameOrLabel();
    //     UE_LOG(LogTemp, Display, TEXT("Overlapping : %s"), *ActorName);
    //     ++index;
    // }
    
    // for(int32 i = 0; i< Actors.Num(); ++i){
    //     FString ActorName = Actors[i]->GetActorNameOrLabel();
    //     UE_LOG(LogTemp, Display, TEXT("Overlapping : %s"), *ActorName);
    // }


    AActor* Actor = GetAcceptableActor();
    if(Actor != nullptr){
        //UE_LOG(LogTemp, Display, TEXT("Unlocking"));
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if(Component != nullptr){
            Component->SetSimulatePhysics(false);
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else{
        //UE_LOG(LogTemp, Display, TEXT("Relocking"));
        Mover->SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() cosnt
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for(AActor* Actor : Actors){
        // FString ActorName = Actor->GetActorNameOrLabel();
        // UE_LOG(LogTemp, Display, TEXT("Overlapping : %s"), *ActorName);

        bool HasAccepttableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");

        if(HasAccepttableTag && !IsGrabbed){
            // UE_LOG(LogTemp, Display, TEXT("Unlocking : %s"), *ActorName);
            return Actor;
        }
    }   

    return nullptr;
}