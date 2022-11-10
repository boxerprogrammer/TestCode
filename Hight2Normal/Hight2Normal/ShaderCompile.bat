set FXCOMPILER=fxc.exe

%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo hight2normal.pso hight2normal_ps.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo wave.pso wave_ps.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo ps.pso ps.hlsl

%FXCOMPILER% /T vs_5_0 /E "main" /O3  /Fo threed.vso threed_vs.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo threed.pso threed_ps.hlsl

pause