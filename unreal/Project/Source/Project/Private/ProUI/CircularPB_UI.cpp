// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/CircularPB_UI.h"
#include "Animation/WidgetAnimation.h"
#include "Runtime/Engine/Classes/Animation/AnimNodeBase.h"

//#include "C:\Program Files/Epic Games/UE_5.3/Engine/Plugins/ScriptPlugin/Source/ScriptPlugin/Classes/ScriptBlueprintGeneratedClass.h"
//#include "C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\UObjectBase.h"
//#include "Runtime/CoreUObject/Public/UObject/UObjectBase.h"
//#include "C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\UnrealType.h"
//#include "C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\NoExportTypes.h"
//#include "C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\UnrealTypePrivate.h"
//#include "C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\UnrealTypePrivate.h"
//#include "C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\UnrealTypePrivate.h"

/*UCircularPB_UI::UCircularPB_UI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}*/

void UCircularPB_UI::NativeConstruct()
{
	FillAnimationsMap();

	Super::NativeConstruct();
}

void UCircularPB_UI::FillAnimationsMap()
{
	AnimationsMap.Empty();

	/*UProperty* Prop = GetClass()->PropertyLink;
	while (Prop != nullptr)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene.GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}*/

	/*FProperty* Prop = this->GetClass()->PropertyLink;
	while (Prop != nullptr)
	{
		if (Prop->GetClass() == FObjectProperty::StaticClass())
		{
			FObjectProperty* ObjProp = Cast<FObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene.GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}*/

	/*FProperty* Prop = GetClass()->PropertyLink;
	if (Prop != nullptr)
	{
		if (Prop->GetClass() == FObjectProperty::StaticClass())
		{
			FObjectProperty* ObjProp = Cast<FObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene.GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}
	}*/
}

UWidgetAnimation* UCircularPB_UI::GetAnimationByName(FName AnimationName)const
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
	if (WidgetAnim)
	{
		return *WidgetAnim; 
	}
	return nullptr;
}

bool UCircularPB_UI::PlayAnimationByName(
	FName AnimationName,
	float StartAtTime,
	int32 NumLoopsToPlay,
	EUMGSequencePlayMode::Type PlayMode,
	float PlaybackSpeed)
{
	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);
	if (WidgetAnim)
	{
		PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
		return true;
	}
	return false;
}

/*void UCircularPB_UI::BeginPlay()
{
	Super::BeginPlay();


}*/