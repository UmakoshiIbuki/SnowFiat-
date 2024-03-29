#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer cbCamera
// {
//
//   row_major float4x4 g_mV;           // Offset:    0 Size:    64
//   row_major float4x4 g_mP;           // Offset:   64 Size:    64
//   float3 g_CamPos;                   // Offset:  128 Size:    12 [unused]
//   int g_DistanceFogEnable;           // Offset:  140 Size:     4 [unused]
//   float3 g_DistanceFogColor;         // Offset:  144 Size:    12 [unused]
//   float g_DistanceFogDensity;        // Offset:  156 Size:     4 [unused]
//
// }
//
// cbuffer cbObject
// {
//
//   row_major float4x4 g_mW;           // Offset:    0 Size:    64
//   float2 g_UVOffset;                 // Offset:   64 Size:     8
//   float2 g_UVTiling;                 // Offset:   72 Size:     8
//   int g_LightEnable;                 // Offset:   80 Size:     4 [unused]
//   int g_FogEnable;                   // Offset:   84 Size:     4 [unused]
//   float g_AlphaTestThreshold;        // Offset:   88 Size:     4 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// cbObject                          cbuffer      NA          NA            cb0      1 
// cbCamera                          cbuffer      NA          NA            cb7      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyzw        0     NONE   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
// NORMAL                   0   xyz         2     NONE   float   xyz 
// COLOR                    0   xyzw        3     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
// TEXCOORD                 1   xyz         2     NONE   float   xyz 
// TEXCOORD                 2   xyzw        3     NONE   float   xyzw
// TEXCOORD                 3   xyz         4     NONE   float   xyz 
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB7[8], immediateIndexed
dcl_constantbuffer CB0[5], immediateIndexed
dcl_input v0.xyzw
dcl_input v1.xy
dcl_input v2.xyz
dcl_input v3.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_output o2.xyz
dcl_output o3.xyzw
dcl_output o4.xyz
dcl_temps 2
mul r0.xyzw, v0.yyyy, cb0[1].xyzw
mad r0.xyzw, v0.xxxx, cb0[0].xyzw, r0.xyzw
mad r0.xyzw, v0.zzzz, cb0[2].xyzw, r0.xyzw
mad r0.xyzw, v0.wwww, cb0[3].xyzw, r0.xyzw
mul r1.xyzw, r0.yyyy, cb7[1].xyzw
mad r1.xyzw, r0.xxxx, cb7[0].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb7[2].xyzw, r1.xyzw
mad r1.xyzw, r0.wwww, cb7[3].xyzw, r1.xyzw
mov o4.xyz, r0.xyzx
mul r0.xyzw, r1.yyyy, cb7[5].xyzw
mad r0.xyzw, r1.xxxx, cb7[4].xyzw, r0.xyzw
mad r0.xyzw, r1.zzzz, cb7[6].xyzw, r0.xyzw
mad o0.xyzw, r1.wwww, cb7[7].xyzw, r0.xyzw
mad o1.xy, v1.xyxx, cb0[4].zwzz, cb0[4].xyxx
mul r0.xyz, v2.yyyy, cb0[1].xyzx
mad r0.xyz, v2.xxxx, cb0[0].xyzx, r0.xyzx
mad r0.xyz, v2.zzzz, cb0[2].xyzx, r0.xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul o2.xyz, r0.wwww, r0.xyzx
mov o3.xyzw, v3.xyzw
ret 
// Approximately 22 instruction slots used
#endif

const BYTE compiledBuffer[] =
{
     68,  88,  66,  67,  41, 248, 
     10, 177,  38, 182,  54,  43, 
    207, 139, 127,  21,  59, 254, 
    120,  89,   1,   0,   0,   0, 
    208,   9,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    124,   4,   0,   0,  12,   5, 
      0,   0, 172,   5,   0,   0, 
     52,   9,   0,   0,  82,  68, 
     69,  70,  64,   4,   0,   0, 
      2,   0,   0,   0, 144,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
     22,   4,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 133,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  99,  98, 
     79,  98, 106, 101,  99, 116, 
      0,  99,  98,  67,  97, 109, 
    101, 114,  97,   0, 171, 171, 
    133,   0,   0,   0,   6,   0, 
      0,   0, 192,   0,   0,   0, 
    160,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   6,   0, 
      0,   0, 176,   2,   0,   0, 
     96,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    176,   1,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0, 192,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 228,   1, 
      0,   0,  64,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0, 192,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 233,   1,   0,   0, 
    128,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    252,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     32,   2,   0,   0, 140,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  56,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  92,   2, 
      0,   0, 144,   0,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 252,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 111,   2,   0,   0, 
    156,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    140,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    103,  95, 109,  86,   0, 102, 
    108, 111,  97, 116,  52, 120, 
     52,   0, 171, 171,   2,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    181,   1,   0,   0, 103,  95, 
    109,  80,   0, 103,  95,  67, 
     97, 109,  80, 111, 115,   0, 
    102, 108, 111,  97, 116,  51, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    242,   1,   0,   0, 103,  95, 
     68, 105, 115, 116,  97, 110, 
     99, 101,  70, 111, 103,  69, 
    110,  97,  98, 108, 101,   0, 
    105, 110, 116,   0,   0,   0, 
      2,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     52,   2,   0,   0, 103,  95, 
     68, 105, 115, 116,  97, 110, 
     99, 101,  70, 111, 103,  67, 
    111, 108, 111, 114,   0, 103, 
     95,  68, 105, 115, 116,  97, 
    110,  99, 101,  70, 111, 103, 
     68, 101, 110, 115, 105, 116, 
    121,   0, 102, 108, 111,  97, 
    116,   0, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    132,   2,   0,   0, 160,   3, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0, 192,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 165,   3,   0,   0, 
     64,   0,   0,   0,   8,   0, 
      0,   0,   2,   0,   0,   0, 
    184,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    220,   3,   0,   0,  72,   0, 
      0,   0,   8,   0,   0,   0, 
      2,   0,   0,   0, 184,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 231,   3, 
      0,   0,  80,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  56,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 245,   3,   0,   0, 
     84,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   4,   0,   0,  88,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 140,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 103,  95, 
    109,  87,   0, 103,  95,  85, 
     86,  79, 102, 102, 115, 101, 
    116,   0, 102, 108, 111,  97, 
    116,  50,   0, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    176,   3,   0,   0, 103,  95, 
     85,  86,  84, 105, 108, 105, 
    110, 103,   0, 103,  95,  76, 
    105, 103, 104, 116,  69, 110, 
     97,  98, 108, 101,   0, 103, 
     95,  70, 111, 103,  69, 110, 
     97,  98, 108, 101,   0, 103, 
     95,  65, 108, 112, 104,  97, 
     84, 101, 115, 116,  84, 104, 
    114, 101, 115, 104, 111, 108, 
    100,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
    171, 171,  73,  83,  71,  78, 
    136,   0,   0,   0,   4,   0, 
      0,   0,   8,   0,   0,   0, 
    104,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,  15,   0,   0, 
    113,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,   3,   0,   0, 
    122,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   7,   7,   0,   0, 
    129,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,  15,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  84,  69,  88, 
     67,  79,  79,  82,  68,   0, 
     78,  79,  82,  77,  65,  76, 
      0,  67,  79,  76,  79,  82, 
      0, 171,  79,  83,  71,  78, 
    152,   0,   0,   0,   5,   0, 
      0,   0,   8,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
    140,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,  12,   0,   0, 
    140,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   7,   8,   0,   0, 
    140,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,   0,   0,   0, 
    140,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,   7,   8,   0,   0, 
     83,  86,  95,  80, 111, 115, 
    105, 116, 105, 111, 110,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171, 171, 171, 
     83,  72,  69,  88, 128,   3, 
      0,   0,  80,   0,   1,   0, 
    224,   0,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   7,   0, 
      0,   0,   8,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      2,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      3,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   2,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   3,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   4,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  86,  21, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 166,  26, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 246,  31,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      7,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      7,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   7,   0, 
      0,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    114,  32,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   7,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      7,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 166,  10, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   7,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,  32,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   7,   0,   0,   0, 
      7,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11,  50,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0, 230, 138,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  70, 128,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  56,   0,   0,   8, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  86,  21,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      0,   0,   0,   0,   6,  16, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 166,  26,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,  32, 
     16,   0,   2,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      3,   0,   0,   0,  70,  30, 
     16,   0,   3,   0,   0,   0, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     22,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  19,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
