set FXCOMPILER=fxc.exe

%FXCOMPILER% /T vs_5_0 /E "main" /O3  /Fo threed_outline.vso threed_outline_vs.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo threed_outline.pso threed_outline_ps.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo noise.pso noise_ps.hlsl
pause