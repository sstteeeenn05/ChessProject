@ECHO OFF

IF EXIST application\Chess\out\build\x64-Debug\ChessProject.exe (
	MOVE /Y application\Chess\out\build\x64-Debug\ChessProject.exe application\chess.exe
) ELSE (
	ECHO The Program no need to update
)

IF EXIST application\chess.exe (
	ECHO Executing
	node index.js
) ELSE (
	ECHO Missing chess.exe. Try to compile the project
)

PAUSE
