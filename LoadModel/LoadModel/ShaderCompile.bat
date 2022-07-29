set FXCOMPILER=fxc.exe

%FXCOMPILER% /T vs_5_0 /E "main" /O3  /Fo vs.vso vs.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo ps.pso ps.hlsl
pause