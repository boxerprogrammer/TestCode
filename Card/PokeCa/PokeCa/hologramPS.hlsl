SamplerState smp : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> norm : register(t1);
Texture2D<float4> grad : register(t2);
Texture2D<float4> monst : register(t3);
Texture2D<float4> marble : register(t4);

struct PSInput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 norm : NORMAL;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
    
};

cbuffer CardState : register(b1)
{
    float time;
    float3 padding;
};

float GenerateRandomFloat(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

float2 GenerateRandomFloat2(float2 st)
{
    st = float2(dot(st, float2(127.1, 311.7)), dot(st, float2(269.5, 183.3)));
    return -1.0 + 2.0 * frac(sin(st) * 43758.5453123);
}

/**
  * バリューノイズ
  * @param {vec2} st 位置
  * 参考: https://nogson2.hatenablog.com/entry/2017/11/18/150645
  * 参考: https://thebookofshaders.com/11/?lan=jp
  */
float GenerateValueNoise( float2 st)
{
    float2 i = floor(st);
    float2 f = frac(st);

    float a = GenerateRandomFloat(i);
    float b = GenerateRandomFloat(i + float2(1.0, 0.0));
    float c = GenerateRandomFloat(i + float2(0.0, 1.0));
    float d = GenerateRandomFloat(i + float2(1.0, 1.0));

    float2 u = f * f * (3.0 - 2.0 * f);

    return lerp(a, b, u.x) +
    (c - a) * u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

float3 HSB2RGB(float3 hsb)
{
    float3 rgb = saturate(abs(frac(hsb.x + float3(0, 2, 1) / 3) * 6 - 3) - 1);
    rgb = rgb * rgb * (3 - 2 * rgb);
    return hsb.z * lerp(float3(1, 1, 1), rgb, hsb.y);
}
float GetViewAngle(float3 norm)
{
    float3 faceNormal = normalize(norm); // 法線ベクトル
    float3 lightDir = normalize(float3(0.0, -1.0, -1.0)); // ライトの向きベクトル
    float angle = acos(dot(faceNormal, lightDir));
    return angle; // 0.0 ～ π
}


float3 GenerateAngleRGB(float strength,float3 norm)
{
    float pi = 3.141592653589793;
    float angle = fmod(GetViewAngle(norm) * strength, pi) / pi; // 0.0 ～ 1.0
    float3 colorHSV = float3(angle, 1.0, 1.0);
    return HSB2RGB(colorHSV);
}

float3 GenerateKiraRGB(float3 colorNoiseRGB,float3 norm)
{
    float3 angleColorRGB = GenerateAngleRGB(5.0,norm); // 角度から表示色を算出
    float colorDiff = distance(colorNoiseRGB, angleColorRGB); // 表示色とノイズの値の差を計算
    float brightness = max(1.0 - colorDiff, 0.0); // 明るさを計算

    float3 kiraNoiseHSV = HSB2RGB(colorNoiseRGB); // 明るさ設定のためHSVに変換
    kiraNoiseHSV.b = brightness; // 明るさを設定
    return HSB2RGB(kiraNoiseHSV); // HSVをRGBに戻す
}
 

/**
 * UV座標から擬似乱数(0.0 ～ 1.0)を生成する
 * @param uv : テクスチャ座標 (0.0 ～ 1.0)
 * @return : ランダムな float 値
 */
float randXorShift(float2 uv)
{
    // 1. シードの生成 (Seed Initialization)
    // floatのUV座標を、ビット演算可能な uint (32bit符号なし整数) へ変換します。
    // 単純なキャストではパターンが見えてしまうため、ドット積と大きな係数で撹拌します。
    // (これは一般的なシェーダのハッシュ生成テクニックです)
    uint seed = uint(dot(uv, float2(12.9898, 78.233)) * 43758.5453);
    
    // XorShiftはシードが0だと0を返し続けてしまうため、安全策としてオフセットを加えます
    if (seed == 0u)
        seed = 123456789u;

    // 2. XorShift アルゴリズム (32-bit version)
    // 3回のXORとシフト操作でビットを強烈に撹拌します
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;

    // 3. 正規化 (Normalization)
    // 生成されたuint (0 ～ 4,294,967,295) を float (0.0 ～ 1.0) に変換します。
    // 1.0 / 4294967295.0 = 2.3283064365386963e-10
    return float(seed) * 2.3283064365386963e-10;
}

/**
 * 2次元座標から1つの乱数を生成するハッシュ関数
 * UV座標ではなく、ピクセル座標(整数っぽい値)を入力することを推奨
 */
float hash12(float2 p)
{
    // 1. ビットレベルでの撹拌を行うための準備
    // 入力pを3次元ベクトルとみなし、ドット積でスカラーへ潰すのではなく
    // ビット列としてランダムな係数と混ぜ合わせます。
    // Fracを使って0-1の範囲に折り返すことで、広大な座標でもパターンを消します。
    float3 p3 = frac(float3(p.xyx) * .1031);
    
    // 2. 非線形な混合 (Dot product with shifted values)
    // 自分自身のスウィズル(yzx)や定数と足し合わせることで
    // 入力のわずかな変化を出力の大きな変化に変えます（Avalanche Effect）
    p3 += dot(p3, p3.yzx + 33.33);
    
    // 3. 最終的な撹拌と正規化
    // XOR演算的な効果を擬似的に作り出し、0.0-1.0に収めます
    return frac((p3.x + p3.y) * p3.z);
}


/// セル内のランダムな位置を取得
float3 GetCellPosition(float2 cell)
{
    float2 randomOffset = GenerateRandomFloat2(cell);
    float sgn = GenerateRandomFloat(cell);
    float affectValue = sign(sgn*2-1.0);
    return float3(cell + randomOffset * 0.5, affectValue);
}
/// セルラーノイズ取得
float GetCellerNoise(float2 uv,float scale)
{
    uv *= scale;
    float2 p_i = floor(uv);
    float2 p_f = frac(uv);
    float minDist = 10000.0; //最小距離初期化=大きな値
    // 近傍9セルをループ
    for (int j = -1; j <= +1; ++j)
    {
        for(int i = -1; i <= +1; ++i)
        {
            float2 neighborCell = p_i + float2(i, j);
            float3 pointData = GetCellPosition(neighborCell);
            float2 pointPos = pointData.xy;
            float dist = distance(uv,pointPos);
            // 最小距離を更新
            minDist = min(minDist, dist);   
        }
    }
    return minDist;
}
float GetMarbleValue(float2 uv,float scale,float time,float2 tiltDir,float2 pd)
{
    uv *= scale;
    uv += tiltDir.xy * uv.yx * 0.25;
    float2 p_i = floor(uv);
    float2 p_f = frac(uv);
    float totalAffect = 0.0;
    float totalWeight = 0.0;
    for (int y = 0; y <= (int)scale; y++)
    {
        for (int x = 0; x <= (int)scale; x++)
        {
            float2 offset = float2(x, y);
            float2 cell = offset;

            float3 pointData = GetCellPosition(cell); // 時間でポイント移動（アニメ）
            float affect = pointData.z; // +1 or -1
            float2 pointPos = pointData.xy;
            if (affect > 0.0)
            {
                offset += pd;
            }
            pointPos += offset;
            float dist = distance(uv, pointPos);
            float weight = 1.0 / (dist * dist + 0.01); // 逆二乗で減衰（滑らかに）

            totalAffect += affect * weight;
            totalWeight += weight;
        }
    }
    float value = totalAffect / (totalWeight + 0.001);
    value = value * 0.5 + 0.5; // 0～1に正規化（必要に応じ調整）

    return value;
}

float3 GetMarbleColor(float noiseValue, float time)
{
    // グラデーションテクスチャ風（1Dグラデ）
    float3 colA = float3(0.8, 0.1, 0.2); // 赤寄り（+影響）
    float3 colB = float3(0.1, 0.0, 0.4); // 青寄り（-影響）
    float3 colM = float3(0.4, 0.1, 0.8); // 中間紫

    float3 color = lerp(colA, colM, smoothstep(0.0, 0.5, noiseValue));
    color = lerp(color, colB, smoothstep(0.5, 1.0, noiseValue));

    // 等高線風強調（オプション）
    float lines = abs(frac(noiseValue * 5.0 + time * 0.2) - 0.5) * 2.0;
    lines = pow(1.0 - lines, 4.0);
    color += lines * float3(1.0, 0.8, 1.0) * 0.3;

    return color;
}

float2 ShearUV(float2 uv, float factor)
{
    float2 centeredUV = uv - 0.5;
    float shearX = centeredUV.y * factor;
    float shearY = centeredUV.x * factor;
    float2 shearedUV = centeredUV + float2(shearX, shearY);
    return shearedUV + 0.5;
}
float2 RadialTwistUV(float2 uv, float2 center, float twistAmount)
{
    float2 centeredUV = uv - center;
    float radius = length(centeredUV);
    float angle = atan2(centeredUV.y, centeredUV.x);
    angle += twistAmount * radius;
    float2 twistedUV = float2(cos(angle), sin(angle)) * radius;
    return twistedUV + center;
}

//疑似PositionDynamics的なオフセット計算
float2 PseudoPDOffset(float2 tilt,float2 time,float strength,float damping)
{
    //遅延計算
    float factor = time * strength;
    float2 delay = tilt * sin(factor) * cos(factor * 0.5);
    
    //ショックアブソーバー的な減衰
    float amp = 1.0 - length(tilt) * damping;
    amp = saturate(amp);
    return delay * amp*strength/10.0;
}

float4 main(PSInput input) : SV_TARGET
{
    //float2 uv = input.uv*10;
    //float2 p_i = floor(uv);
    //float3 cp= GetCellPosition(p_i);
    //return float4(frac(cp.xyz), 1);
    //float celler = GetCellerNoise(input.uv, 4.0);
    //return float4(celler, celler, celler, 1.0);
    
    //float2 r2 = GenerateRandomFloat2(input.uv);
    float2 uv = input.uv;
    float tm = sin(time) + 1.0;
    float2 offset = PseudoPDOffset(input.norm.xy, tm, 5.0, 0.25);
  
    uv = RadialTwistUV(uv, float2(0.5, 0.5), 3.0);
    uv = ShearUV(uv, 0.3);
    
    float mv = GetMarbleValue(uv, 3.0, tm, input.norm.xy,offset);
    float3 marbleCol = marble.Sample(smp, float2(mv,0)).rgb;
    
    float u = abs(1 - distance((input.uv * float2(2, -2)) + float2(-1, 1), input.norm.xy * 2));
    float2 muv = abs(input.uv * 2 - float2(1, 1));

        
    float4 gcol = grad.Sample(smp, float2(fmod(u, 1), 0));
    float4 gcol2 = grad.Sample(smp, float2(fmod(1 - u + abs(dot(input.norm.xyz, float3(0, 1, 0))), 0.5), 0));
    
    gcol2.rgb = 1 - gcol2.bgr;
    
    float4 frameCol = tex.Sample(smp, input.uv);
    float scale = 1.5 - saturate(dot(input.norm.xyz, float3(0, 0, 1))) * 0.5;
    float4 monCol = monst.Sample(smp, (input.uv - float2(0.5, 0.5)) * scale +
                            float2(0.5, 0.5)
                            - input.norm.rg * float2(0.05, -0.05));
    
    float4 normTex = norm.Sample(smp, input.uv);
    float3 normal = normalize(input.norm.xyz + normalize(normTex.xyz * 2.0 - 1.0));
    float4 cardCol = float4(lerp(monCol.rgb, marbleCol.rgb, frameCol.a), max(frameCol.a, monCol.a));
    
    //カードフレーム枠部分
    if (frameCol.r < 0.65 && frameCol.g<0.65 && frameCol.a>0.0)
    {
        float kiraFactor = 1-saturate(dot(normalize(normTex.xyz * 2.0 - 1.0), input.norm.xyz));
        return lerp(float4(1-gcol.rgb,1), gcol + u, kiraFactor);
        //return lerp(cardCol, gcol+u  ,kiraFactor );
    }
    else
    {
        if (frameCol.a == 0.0 && cardCol.a > 0.0)
        {
            float f = step(2.8, monCol.r + monCol.g + monCol.b);
            normTex.rg = -normTex.rg;
            float kiraFactor = 1 - saturate(dot(normalize(normTex.xyz * 2.0 - 1.0), input.norm.xyz));
            float4 col = lerp(monCol, lerp(gcol2, monCol, 0.5), kiraFactor);
            return lerp(monCol, col, f + 1 - monCol.r);
            
        }
        return cardCol;
    }
    
    
    const float pi = 3.1415926535;
    float theta = atan2(normal.y, normal.x) + pi;
    float3 kiraCol = GenerateKiraRGB(HSB2RGB(float3(theta / (2 * pi), 1.0, 1.0)),
                    input.norm.xyz);
    float noise = GenerateValueNoise(input.uv * 3.0 + float2(0.0, 0.0));
    float3 noiseCol = GenerateKiraRGB(HSB2RGB(noise), normal);
    
    
    
    
    float3 eyepos = normalize(float3(0, 0, -10));
    float3 eyeDir = normalize(input.pos.xyz-eyepos);
    
    float3 light = normalize(float3(1.0, -1.0, 1.0));
    float3 reflectDir = reflect(-light, normal);
    float3 rayReflectDir = normalize(reflect(eyeDir, normal));
    float spec = pow(saturate(dot(reflectDir, eyeDir)), 50.0);
    
    float factor = pow(saturate(dot(input.norm.xyz, eyeDir)), 4);
    factor = 1.0 - factor;
    
    float4 gradCol = grad.Sample(smp, (rayReflectDir.rg + 1) / 2);
    
    
    
    return float4(lerp(cardCol.rgb, gradCol.rgb, factor) + spec, cardCol.a);
            //lerp(kiraCol.rgb, noiseCol, frameCol.a), factor) + spec, max(frameCol.a, monCol.a));
}