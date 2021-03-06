#include "Plugin.h"

LPHANDLE g_ReClassProcessAttached = nullptr;

BOOL PLUGIN_CC PluginInit( LPRECLASS_PLUGIN_INFO lpRCInfo )
{
	wcscpy_s( lpRCInfo->Name, L"Test Plugin Name" );
	wcscpy_s( lpRCInfo->Version, L"1.0.0.2" );
	wcscpy_s( lpRCInfo->About, L"This plugin is a test plugin" );
	lpRCInfo->DialogID = IDD_SETTINGS_DLG;

	if ( !ReClassOverrideMemoryOperations( WriteCallback, ReadCallback ) ) {
		ReClassPrintConsole( L"[TestPlugin] Failed to register read/write callbacks, failing PluginInit" );
		return FALSE;
	}

	g_ReClassProcessAttached = ReClassGetProcessHandle( );

	return TRUE;
}

void PLUGIN_CC PluginStateChange( bool state )
{
	if (!state)
		ReClassPrintConsole(L"[TestPlugin] Disabled!");
	else
		ReClassPrintConsole(L"[TestPlugin] Enabled!");
}

INT_PTR CALLBACK PluginSettingsDlg( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch ( Msg )
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_CLOSE:
		EndDialog( hWnd, 0 );
		break;
	}
	return FALSE;
}

BOOL WINAPI WriteCallback( LPVOID write_address, LPVOID buffer_ptr, SIZE_T write_size, PSIZE_T num_write )
{
	DWORD OldProtect;
	VirtualProtectEx( *g_ReClassProcessAttached, (void*) write_address, write_size, PAGE_EXECUTE_READWRITE, &OldProtect );
	BOOL ret = WriteProcessMemory( *g_ReClassProcessAttached, (void*) write_address, buffer_ptr, write_size, num_write );
	VirtualProtectEx( *g_ReClassProcessAttached, (void*) write_address, write_size, OldProtect, NULL );
	return ret;
}

BOOL WINAPI ReadCallback( LPVOID read_address, LPVOID buffer_ptr, SIZE_T read_size, PSIZE_T num_read )
{
	BOOL return_val = ReadProcessMemory( *g_ReClassProcessAttached, (LPVOID) read_address, buffer_ptr, read_size, num_read );
	if ( !return_val ) ZeroMemory( buffer_ptr, read_size );
	return return_val;
}
