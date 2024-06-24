@ECHO OFF
git diff --name-only -- ../Depedencies/Includes/Public/* > differences
echo Fetched Differences to exlude non modified header files
start "" /w %~dp0ctags.exe -f Tools/tags --c-kinds=c --fields=i -R Depedencies/Includes/Public/*
echo Finished tagging
start "" /w %~dp0Tools.exe Tools/tags
echo Finished Generating

PAUSE

::depedencies should be DEPENDENCIES