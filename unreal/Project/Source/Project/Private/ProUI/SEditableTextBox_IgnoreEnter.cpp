// Fill out your copyright notice in the Description page of Project Settings.

#include "ProUI/SEditableTextBox_IgnoreEnter.h"

#include "Framework/Application/SlateApplication.h"
#include "Input/Events.h"

FReply SEditableTextBox_IgnoreEnter::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    // Check if the Enter key is pressed
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        // Prevent default behavior and keep focus on the input box
        FSlateApplication::Get().SetKeyboardFocus(SharedThis(this));
        return FReply::Handled();
    }

    // Call parent class behavior for other keys
    return SEditableTextBox_IgnoreEnter::OnKeyDown(MyGeometry, InKeyEvent);
}