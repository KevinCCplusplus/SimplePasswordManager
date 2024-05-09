// ConfigurePasswordDialog.h
#pragma once

#include <Windows.h>

// Define the dialog box identifier
#define IDD_CONFIGURE_PASSWORD 102

// Declare the dialog box procedure 
INT_PTR CALLBACK ConfigurePasswordDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Declare the function to show the configure password dialog
void ShowConfigurePasswordDialog(HWND hParent);
// Declare other functions if needed

// Function to display an error message
void ShowPasswordError(HWND hDlg);

// Function to validate the password and display an error message if necessary
bool ValidateAndShowPasswordError(HWND hDlg);