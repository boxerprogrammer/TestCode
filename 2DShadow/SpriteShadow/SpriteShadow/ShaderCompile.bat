set FXCOMPILER=fxc.exe

%FXCOMPILER% /T ps_5_0 /O3 /E "main" /Fo SpriteShadowPS.pso SpriteShadowPS.hlsl
%FXCOMPILER% /T ps_5_0 /O3 /E "main" /Fo SpriteShadowPLPS.pso SpriteShadowPLPS.hlsl
pause