#include <iostream>
#include "ConfigurePasswordDialog.h"
#include "Resource.h"
#include <PasswordValidator.h>
#include <EncryptionUtils.h>
#include "PasswordStorage.h"




// Implement the functions for the Configure Password screen here

// Function to display the Configure Password dialog
void ShowConfigurePasswordDialog(HWND hParent)
{
    // Create a modal dialog box for configuring the password
    int result = DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_CONFIGURE_PASSWORD), hParent, ConfigurePasswordDlgProc);

    // Check if the dialog was created successfully
    if (result == -1)
    {
        // Handle error here
        MessageBox(hParent, L"Failed to create Configure Password dialog box!", L"Error", MB_OK | MB_ICONERROR);
    }
}

// Function to display an error message
void ShowPasswordError(HWND hDlg)
{
    MessageBox(hDlg, L"Password must meet the following criteria:\n- At least 8 characters\n- At least one uppercase letter\n- At least one digit\n- At least one special character", L"Password Error", MB_OK | MB_ICONERROR);
}

// Function to validate the password and show an error message if necessary
bool ValidateAndShowPasswordError(HWND hDlg)
{
    // Get the password from the input field
    TCHAR szPassword[256];
    GetDlgItemText(hDlg, IDC_PASSWORD, szPassword, 256);

    // Validate the password
    if (!ValidatePassword(szPassword))
    {
        // Show an error message if the password does not meet the requirements
        ShowPasswordError(hDlg);
        return false;
    }

    return true;
}



// Function to save the new password with salt
void SaveNewPassword(const std::wstring& newPassword)
{
    try
    {
        // Generate a random salt with minimum length of 8 characters
        std::string salt = GenerateSalt(8);

        // Combine salt with password
        std::string saltedPassword = salt + std::string(newPassword.begin(), newPassword.end());

        // Encrypt the salted password
        std::string encryptedPassword = EncryptAES(saltedPassword, GenerateAESKey());

        // Save the salt and encrypted password
        if (!SavePassword(encryptedPassword, salt))
        {
            // Show error message if failed to save the password
            MessageBox(nullptr, L"Failed to save the password.", L"Error", MB_OK | MB_ICONERROR);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception occurred while saving password: " << ex.what() << std::endl;
    }
}

// Function to verify password
bool VerifyPassword(const std::wstring& password)
{
    try
    {
        // Retrieve salt and encrypted password
        auto result = RetrievePasswordAndSalt();
        std::string salt = result.first;
        std::string encryptedPassword = result.second;

        // Combine salt with input password
        std::string saltedPassword = salt + std::string(password.begin(), password.end());

        // Encrypt the input password with the retrieved salt
        std::string encryptedInputPassword = EncryptAES(saltedPassword, GenerateAESKey());

        // Compare encrypted passwords
        return encryptedInputPassword == encryptedPassword;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception occurred while verifying password: " << ex.what() << std::endl;
        return false;
    }
}

// Add any other function implementations here
// 
// Dialog procedure for the Configure Password dialog
INT_PTR CALLBACK ConfigurePasswordDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDOK:
        {
            // Validate the entered password
            if (ValidateAndShowPasswordError(hDlg))
            {
                // Get the entered password
                TCHAR szPassword[256];
                GetDlgItemText(hDlg, IDC_PASSWORD, szPassword, 256);

                // Save the new password
                SaveNewPassword(szPassword);

                // Close the dialog
                EndDialog(hDlg, IDOK);
            }
            return TRUE;
        }
        break;
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
    }
    break;
    }
    return FALSE;
}