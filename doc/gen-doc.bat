@echo off
REM Copyright (c) 2023-2025 Rakyuu Studio. All rights reserved.
REM File: `gen-doc.bat`.

setlocal

set trying=0
set success=0
set shot_count=0

for /f "tokens=*" %%i in ('where python 2^>nul') do set PYTHON_PATH=%%i

if not defined PYTHON_PATH (
    echo "[gen-doc.bat]: Python installation not found... Do you want to install Python 3.12? (Y/N):"
    set /p choice="Enter your choice: "
    if /i "%choice%" == "Y" (
        call :install_python
    ) else if /i "%choice%" == "N" (
        echo "Why... This script and documentation couldn't be served without Python..."
        echo "Do you want to retry or quit? (R/Q):"
        set /p k_c="Enter your choice: "
        if /i "%k_c%" == "R" (
            goto :CHECK_PYTHON
        ) else (
            echo "Exiting script. Please install Python manually if needed."
            pause
            exit /b 1
        )
    ) else (
        echo "Invalid choice! Please ONLY enter Y or N."
        goto :CHECK_PYTHON
    )
)

:CHECK_PIP
for /f "tokens=*" %%i in ('where pip 2^>nul') do set PIP_PATH=%%i
if not defined PIP_PATH (
    echo "PIP not found! Please make sure PIP is installed and available in PATH."
    pause
    exit /b 1
)

%PYTHON_PATH% -m pip --version >nul 2>&1
if errorlevel 1 (
    echo "PIP is not functional. Please fix your Python installation and try again."
    pause
    exit /b 1
)

goto :INSTALL_MKDOCS

:INSTALL_MKDOCS
%PIP_PATH% show mkdocs mkdocs-material >nul 2>&1
if errorlevel 1 (
    echo "Can't find mkdocs and mkdocs-material... Installing them now..."
    :RETRY_INSTALL
    %PYTHON_PATH% -m pip install mkdocs mkdocs-material
    if errorlevel 1 (
        echo "Failed to install mkdocs or mkdocs-material. Retrying..."
        set /a shot_count+=1
        if %shot_count% geq 5 (
            echo "Too many failed attempts. Please manually run: pip install mkdocs mkdocs-material."
            pause
            exit /b 1
        )
        goto :RETRY_INSTALL
    )
)

echo "mkdocs and mkdocs-material installed successfully."
goto :SETUP_DOCS

:SETUP_DOCS
set DOC_FOLDER=Retoccilus-Document
if not exist "%DOC_FOLDER%" (
    echo "Creating new documentation project..."
    %PYTHON_PATH% -m mkdocs new "%DOC_FOLDER%"
    if errorlevel 1 (
        echo "Failed to create mkdocs project. Exiting..."
        exit /b 1
    )
)

echo "Copying documentation files..."
xcopy /E /I /Y doc "%DOC_FOLDER%"
cd /d "%DOC_FOLDER%"
echo "Serving documentation locally..."
%PYTHON_PATH% -m mkdocs serve

echo "Documentation is now available. Happy coding!"
pause
exit /b 0

:install_python
if %trying% geq 5 (
    echo "Too many failed attempts to install Python. Exiting..."
    pause
    exit /b 1
)

echo "Installing Python..."
echo "Note: This will install Python with default settings. If you need a custom path, please install manually."
echo "Continue? (Y/N):"
set /p c_c="Enter your choice: "
if /i "%c_c%" == "Y" (
    powershell -Command "Invoke-WebRequest -Uri 'https://www.python.org/ftp/python/3.12.0/python-3.12.0-amd64.exe' -OutFile 'python_installer.exe'"
    start /wait python_installer.exe /quiet PrependPath=1
    for /f "tokens=*" %%i in ('where python 2^>nul') do set PYTHON_PATH=%%i
    if not defined PYTHON_PATH (
        echo "Failed to install Python 3.12. Retrying..."
        set /a trying+=1
        goto :install_python
    ) else (
        echo "Python installed successfully."
        exit /b 0
    )
) else (
    echo "Skipping automatic installation. Please install Python manually if needed."
    exit /b 1
)
