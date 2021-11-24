set FXCOMPILER=fxc.exe

%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo hight2normal.pso hight2normal_ps.hlsl
%FXCOMPILER% /T ps_5_0 /E "main" /O3  /Fo wave.pso wave_ps.hlsl

pause