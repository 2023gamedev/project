// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SEditableTextBox.h"

/**
 * A custom EditableTextBox that keeps focus after pressing Enter.
 */
class SEditableTextBox_IgnoreEnter : public SEditableTextBox
{
public:
    SLATE_BEGIN_ARGS(SEditableTextBox_IgnoreEnter) {}
    SLATE_END_ARGS()

    /** Constructs this widget with the given arguments */
    /*void Construct(const FArguments& InArgs)
    {
        SEditableTextBox_IgnoreEnter::Construct(InArgs);
    }*/

    /** Override OnKeyDown to handle Enter key */
    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};
