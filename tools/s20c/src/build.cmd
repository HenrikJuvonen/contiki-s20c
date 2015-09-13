@set msbuild="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
@%msbuild% cc.sln /nologo /t:rebuild /p:Configuration=Release
@mv bin/Release/cc.exe ../cc.exe
@rd /s /q obj bin