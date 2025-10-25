#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     string_vibrato_dc1; //xy=308,2298
AudioSynthWaveform       string_vibrato_lfo1; //xy=320,2231
AudioSynthWaveformDc     string_vibrato_dc; //xy=325.8666687011719,943.8666687011719
AudioSynthWaveform       string_vibrato_lfo; //xy=337.8666687011719,876.8666687011719
AudioSynthWaveformDc     chords_vibrato_dc1; //xy=402,4328
AudioSynthWaveform       chords_vibrato_lfo1; //xy=405,4242
AudioSynthWaveformDc     chords_vibrato_dc; //xy=436.86669921875,3178.86669921875
AudioSynthWaveform       chords_vibrato_lfo; //xy=439.86669921875,3092.86669921875
AudioEffectEnvelope      envelope_string_vibrato_dc1; //xy=474,2334
AudioEffectEnvelope      envelope_string_vibrato_dc; //xy=491.8666687011719,979.8666687011719
AudioEffectEnvelope      envelope_string_vibrato_lfo1; //xy=499,2275
AudioEffectEnvelope      envelope_string_vibrato_lfo; //xy=516.8666687011719,920.8666687011719
AudioEffectEnvelope      voice4_vibrato_dc_envelope1; //xy=611,5142
AudioEffectEnvelope      voice3_vibrato_envelope1; //xy=615,4812
AudioEffectEnvelope      voice3_vibrato_dc_envelope1; //xy=618,4897
AudioSynthWaveformDc     voice3_frequency_dc1; //xy=623,4854
AudioEffectEnvelope      voice4_vibrato_envelope1; //xy=624,5076
AudioSynthWaveformDc     voice4_frequency_dc1; //xy=625,5110
AudioEffectEnvelope      voice2_vibrato_dc_envelope1; //xy=633,4598
AudioEffectEnvelope      voice2_vibrato_envelope1; //xy=634,4524
AudioSynthWaveformDc     voice2_frequency_dc1; //xy=639,4562
AudioEffectEnvelope      voice4_vibrato_dc_envelope; //xy=645.86669921875,3992.86669921875
AudioEffectEnvelope      voice3_vibrato_envelope; //xy=649.86669921875,3662.86669921875
AudioEffectEnvelope      voice1_vibrato_envelope1; //xy=648,4242
AudioEffectEnvelope      voice1_vibrato_dc_envelope1; //xy=648,4327
AudioEffectEnvelope      voice3_vibrato_dc_envelope; //xy=652.86669921875,3747.86669921875
AudioSynthWaveformDc     voice1_frequency_dc1; //xy=653,4282
AudioSynthWaveformDc     voice3_frequency_dc; //xy=657.86669921875,3704.86669921875
AudioEffectEnvelope      voice4_vibrato_envelope; //xy=658.86669921875,3926.86669921875
AudioSynthWaveformDc     voice4_frequency_dc; //xy=659.86669921875,3960.86669921875
AudioEffectEnvelope      voice2_vibrato_dc_envelope; //xy=667.86669921875,3448.86669921875
AudioEffectEnvelope      voice2_vibrato_envelope; //xy=668.86669921875,3374.86669921875
AudioMixer4              string_vibrato_mixer1; //xy=674,2336
AudioSynthWaveformDc     voice2_frequency_dc; //xy=673.86669921875,3412.86669921875
AudioEffectEnvelope      voice1_vibrato_envelope; //xy=682.86669921875,3092.86669921875
AudioEffectEnvelope      voice1_vibrato_dc_envelope; //xy=682.86669921875,3177.86669921875
AudioMixer4              string_vibrato_mixer; //xy=691.8666687011719,981.8666687011719
AudioSynthWaveformDc     voice1_frequency_dc; //xy=687.86669921875,3132.86669921875
AudioMixer4              voice3_vibrato_mixer1; //xy=840,4842
AudioMixer4              voice4_vibrato_mixer1; //xy=846,5085
AudioSynthWaveformModulated waveform_string_17; //xy=860,2490
AudioSynthWaveformModulated waveform_string_19; //xy=861,2422
AudioSynthWaveformModulated waveform_string_21; //xy=861,2527
AudioSynthWaveformModulated waveform_string_22; //xy=861,2598
AudioSynthWaveformModulated waveform_string_16; //xy=861,2705
AudioSynthWaveformModulated waveform_string_20; //xy=862,2456
AudioSynthWaveformModulated waveform_string_15;  //xy=861,2772
AudioSynthWaveformModulated waveform_string_24; //xy=862,2564
AudioSynthWaveformModulated waveform_string_23; //xy=862,2634
AudioSynthWaveformModulated waveform_string_18; //xy=863,2387
AudioSynthWaveformModulated waveform_string_13; //xy=863,2670
AudioSynthWaveformModulated waveform_string_14; //xy=863,2740
AudioSynthWaveformModulated waveform_string_4; //xy=877.8666687011719,1135.8666687011719
AudioSynthWaveformModulated waveform_string_2; //xy=878.8666687011719,1067.8666687011719
AudioSynthWaveformModulated waveform_string_5; //xy=878.8666687011719,1172.8666687011719
AudioSynthWaveformModulated waveform_string_7; //xy=878.8666687011719,1243.8666687011719
AudioSynthWaveformModulated waveform_string_10; //xy=878.8666687011719,1350.8666687011719
AudioSynthWaveformModulated waveform_string_3; //xy=879.8666687011719,1101.8666687011719
AudioSynthWaveformModulated waveform_string_12; //xy=878.8666687011719,1417.8666687011719
AudioSynthWaveformModulated waveform_string_6; //xy=879.8666687011719,1209.8666687011719
AudioSynthWaveformModulated waveform_string_8; //xy=879.8666687011719,1279.8666687011719
AudioSynthWaveformModulated waveform_string_1; //xy=880.8666687011719,1032.8666687011719
AudioSynthWaveformModulated waveform_string_9; //xy=880.8666687011719,1315.8666687011719
AudioSynthWaveformModulated waveform_string_11; //xy=880.8666687011719,1385.8666687011719
AudioMixer4              voice2_vibrato_mixer1; //xy=871,4551
AudioMixer4              voice3_vibrato_mixer; //xy=874.86669921875,3692.86669921875
AudioSynthWaveformDc     filter_dc1; //xy=886,1956
AudioMixer4              voice1_vibrato_mixer1; //xy=879,4262
AudioMixer4              voice4_vibrato_mixer; //xy=880.86669921875,3935.86669921875
AudioSynthWaveformDc     filter_dc;      //xy=903.8666687011719,601.8666687011719
AudioSynthWaveform       waveform_transient_22; //xy=902,1799
AudioSynthWaveform       waveform_transient_15; //xy=903,1671
AudioSynthWaveform       waveform_transient_16; //xy=903,1704
AudioSynthWaveform       waveform_transient_18; //xy=904,1538
AudioSynthWaveform       waveform_transient_19; //xy=904,1570
AudioSynthWaveform       waveform_transient_20; //xy=904,1603
AudioSynthWaveform       waveform_transient_17; //xy=904,1638
AudioSynthWaveform       waveform_transient_13; //xy=904,1735
AudioSynthWaveform       waveform_transient_14; //xy=905,1767
AudioSynthWaveform       waveform_transient_23; //xy=906,1858
AudioSynthWaveform       waveform_transient_21; //xy=908,1830
AudioSynthWaveform       waveform_transient_24; //xy=909,1890
AudioMixer4              voice2_vibrato_mixer; //xy=905.86669921875,3401.86669921875
AudioSynthWaveform       waveform_transient_9; //xy=919.8666687011719,444.8666687011719
AudioSynthWaveform       waveform_transient_5; //xy=920.8666687011719,316.8666687011719
AudioSynthWaveform       waveform_transient_6; //xy=920.8666687011719,349.8666687011719
AudioSynthWaveform       waveform_transient_1; //xy=921.8666687011719,183.86666870117188
AudioSynthWaveform       waveform_transient_2; //xy=921.8666687011719,215.86666870117188
AudioSynthWaveform       waveform_transient_3; //xy=921.8666687011719,248.86666870117188
AudioSynthWaveform       waveform_transient_4; //xy=921.8666687011719,283.8666687011719
AudioSynthWaveform       waveform_transient_7; //xy=921.8666687011719,380.8666687011719
AudioSynthWaveform       waveform_transient_8; //xy=922.8666687011719,412.8666687011719
AudioSynthWaveform       waveform_transient_11; //xy=923.8666687011719,503.8666687011719
AudioMixer4              voice1_vibrato_mixer; //xy=913.86669921875,3112.86669921875
AudioSynthWaveform       waveform_transient_10; //xy=925.8666687011719,475.8666687011719
AudioSynthWaveform       waveform_transient_12; //xy=926.8666687011719,535.8666687011719
AudioSynthNoiseWhite     voice3_noise1; //xy=1034,4911
AudioSynthWaveformModulated voice_osc34; //xy=1037,4877
AudioSynthNoiseWhite     voice4_noise1; //xy=1037,5188
AudioSynthWaveformModulated voice3_osc4; //xy=1039,4841
AudioSynthWaveformModulated voice4_osc4; //xy=1039,5151
AudioSynthWaveformModulated voice3_osc5; //xy=1043,4803
AudioSynthWaveformModulated voice4_osc5; //xy=1042,5114
AudioSynthWaveformModulated voice4_osc6; //xy=1044,5079
AudioSynthWaveform       chords_filter_LFO1; //xy=1057,4434
AudioSynthNoiseWhite     voice2_noise1; //xy=1060,4634
AudioSynthWaveformModulated voice2_osc4; //xy=1062,4597
AudioSynthWaveformModulated voice_osc23; //xy=1064,4561
AudioSynthWaveformModulated voice2_osc5; //xy=1065,4524
AudioSynthNoiseWhite     voice3_noise;   //xy=1068.86669921875,3761.86669921875
AudioEffectEnvelope      envelope_filter_19; //xy=1078,1987
AudioEffectEnvelope      envelope_filter_20; //xy=1078,2017
AudioEffectEnvelope      envelope_filter_21; //xy=1078,2079
AudioEffectEnvelope      envelope_filter_24; //xy=1078,2111
AudioEffectEnvelope      envelope_filter_22; //xy=1078,2142
AudioSynthWaveformModulated voice3_osc3;    //xy=1071.86669921875,3727.86669921875
AudioEffectEnvelope      envelope_filter_18; //xy=1079,1956
AudioEffectEnvelope      envelope_filter_17; //xy=1079,2047
AudioEffectEnvelope      envelope_filter_15; //xy=1078,2300
AudioEffectEnvelope      envelope_filter_13; //xy=1079,2205
AudioSynthNoiseWhite     voice4_noise;   //xy=1071.86669921875,4038.86669921875
AudioEffectEnvelope      envelope_filter_14; //xy=1079,2269
AudioSynthWaveformModulated voice3_osc2;    //xy=1073.86669921875,3691.86669921875
AudioSynthWaveformModulated voice4_osc3;    //xy=1073.86669921875,4001.86669921875
AudioSynthNoiseWhite     voice1_noise1; //xy=1073,4352
AudioEffectEnvelope      envelope_filter_16; //xy=1082,2238
AudioEffectEnvelope      envelope_filter_23; //xy=1083,2172
AudioSynthWaveformModulated voice3_osc1;    //xy=1077.86669921875,3653.86669921875
AudioSynthWaveformModulated voice4_osc2;    //xy=1076.86669921875,3964.86669921875
AudioEffectEnvelope      envelope_string_24; //xy=1083,2562
AudioEffectEnvelope      envelope_string_23;  //xy=1083,2633
AudioEffectEnvelope      envelope_string_16; //xy=1083,2701
AudioEffectEnvelope      envelope_string_15; //xy=1083,2770
AudioEffectEnvelope      envelope_string_21; //xy=1084,2527
AudioEffectEnvelope      envelope_string_22; //xy=1084,2597
AudioSynthWaveformModulated voice4_osc1;    //xy=1078.86669921875,3929.86669921875
AudioEffectEnvelope      envelope_string_13; //xy=1084,2667
AudioEffectEnvelope      envelope_string_20; //xy=1085,2456
AudioSynthWaveformModulated voice1_osc4; //xy=1078,4316
AudioEffectEnvelope      envelope_string_17;  //xy=1086,2490
AudioSynthWaveformModulated voice_osc12; //xy=1079,4246
AudioSynthWaveformModulated voice1_osc5; //xy=1079,4281
AudioEffectEnvelope      envelope_string_14; //xy=1086,2736
AudioEffectEnvelope      envelope_string_18; //xy=1088,2389
AudioEffectEnvelope      envelope_string_19; //xy=1088,2422
AudioEffectEnvelope      envelope_filter_2; //xy=1095.8666687011719,632.8666687011719
AudioEffectEnvelope      envelope_filter_3; //xy=1095.8666687011719,662.8666687011719
AudioEffectEnvelope      envelope_filter_5; //xy=1095.8666687011719,724.8666687011719
AudioEffectEnvelope      envelope_filter_6; //xy=1095.8666687011719,756.8666687011719
AudioEffectEnvelope      envelope_filter_7; //xy=1095.8666687011719,787.8666687011719
AudioEffectEnvelope      envelope_filter_1; //xy=1096.8666687011719,601.8666687011719
AudioEffectEnvelope      envelope_filter_4; //xy=1096.8666687011719,692.8666687011719
AudioEffectEnvelope      envelope_filter_12; //xy=1095.8666687011719,945.8666687011719
AudioEffectEnvelope      envelope_filter_9; //xy=1096.8666687011719,850.8666687011719
AudioEffectEnvelope      envelope_filter_11; //xy=1096.8666687011719,914.8666687011719
AudioEffectEnvelope      envelope_filter_10; //xy=1099.8666687011719,883.8666687011719
AudioEffectEnvelope      envelope_filter_8; //xy=1100.8666687011719,817.8666687011719
AudioSynthWaveform       chords_filter_LFO; //xy=1091.86669921875,3284.86669921875
AudioEffectEnvelope      envelope_string_6; //xy=1100.8666687011719,1207.8666687011719
AudioEffectEnvelope      envelope_string_8; //xy=1100.8666687011719,1278.8666687011719
AudioEffectEnvelope      envelope_string_10; //xy=1100.8666687011719,1346.8666687011719
AudioEffectEnvelope      envelope_string_12; //xy=1100.8666687011719,1415.8666687011719
AudioEffectEnvelope      envelope_string_5; //xy=1101.8666687011719,1172.8666687011719
AudioEffectEnvelope      envelope_string_7; //xy=1101.8666687011719,1242.8666687011719
AudioEffectEnvelope      envelope_string_9; //xy=1101.8666687011719,1312.8666687011719
AudioEffectEnvelope      envelope_string_3; //xy=1102.8666687011719,1101.8666687011719
AudioEffectEnvelope      envelope_string_4; //xy=1103.8666687011719,1135.8666687011719
AudioSynthNoiseWhite     voice2_noise;   //xy=1094.86669921875,3484.86669921875
AudioEffectEnvelope      envelope_string_11; //xy=1103.8666687011719,1381.8666687011719
AudioEffectEnvelope      envelope_string_1; //xy=1105.8666687011719,1034.8666687011719
AudioEffectEnvelope      envelope_string_2; //xy=1105.8666687011719,1067.8666687011719
AudioSynthWaveformModulated voice2_osc3;    //xy=1096.86669921875,3447.86669921875
AudioSynthWaveformModulated voice2_osc2;    //xy=1098.86669921875,3411.86669921875
AudioSynthWaveformModulated voice2_osc1;    //xy=1099.86669921875,3374.86669921875
AudioSynthNoiseWhite     voice1_noise;   //xy=1107.86669921875,3202.86669921875
AudioSynthWaveformModulated voice1_osc3;    //xy=1112.86669921875,3166.86669921875
AudioSynthWaveformModulated voice1_osc1;    //xy=1113.86669921875,3096.86669921875
AudioSynthWaveformModulated voice1_osc2;    //xy=1113.86669921875,3131.86669921875
AudioEffectEnvelope      envelope_transient_13; //xy=1129,1736
AudioEffectEnvelope      envelope_transient_21; //xy=1130,1829
AudioEffectEnvelope      envelope_transient_14; //xy=1131,1766
AudioEffectEnvelope      envelope_transient_22; //xy=1131,1797
AudioEffectEnvelope      envelope_transient_15; //xy=1132,1669
AudioEffectEnvelope      envelope_transient_16; //xy=1132,1701
AudioEffectEnvelope      envelope_transient_23; //xy=1132,1859
AudioEffectEnvelope      envelope_transient_17; //xy=1133,1634
AudioEffectEnvelope      envelope_transient_24; //xy=1132,1887
AudioEffectEnvelope      envelope_transient_18; //xy=1135,1540
AudioEffectEnvelope      envelope_transient_19; //xy=1135,1571
AudioEffectEnvelope      envelope_transient_20; //xy=1135,1603
AudioEffectEnvelope      envelope_transient_7; //xy=1146.8666687011719,381.8666687011719
AudioEffectEnvelope      envelope_transient_10; //xy=1147.8666687011719,474.8666687011719
AudioEffectEnvelope      envelope_transient_8; //xy=1148.8666687011719,411.8666687011719
AudioEffectEnvelope      envelope_transient_9; //xy=1148.8666687011719,442.8666687011719
AudioEffectEnvelope      envelope_transient_5; //xy=1149.8666687011719,314.8666687011719
AudioEffectEnvelope      envelope_transient_6; //xy=1149.8666687011719,346.8666687011719
AudioEffectEnvelope      envelope_transient_11; //xy=1149.8666687011719,504.8666687011719
AudioEffectEnvelope      envelope_transient_4; //xy=1150.8666687011719,279.8666687011719
AudioEffectEnvelope      envelope_transient_12; //xy=1149.8666687011719,532.8666687011719
AudioEffectEnvelope      envelope_transient_1; //xy=1152.8666687011719,185.86666870117188
AudioEffectEnvelope      envelope_transient_2; //xy=1152.8666687011719,216.86666870117188
AudioEffectEnvelope      envelope_transient_3; //xy=1152.8666687011719,248.86666870117188
AudioEffectEnvelope      voice4_envelope_filter1; //xy=1236,5254
AudioMixer4              voice3_mixer1; //xy=1240,4835
AudioMixer4              voice4_mixer1; //xy=1241,5098
AudioMixer4              voice2_mixer1; //xy=1245,4550
AudioEffectEnvelope      voice3_envelope_filter1; //xy=1244,4941
AudioEffectEnvelope      voice2_envelope_filter1; //xy=1260,4668
AudioEffectEnvelope      voice1_envelope_filter1; //xy=1262,4437
AudioMixer4              voice1_mixer1; //xy=1264,4280
AudioEffectEnvelope      voice4_envelope_filter; //xy=1270.86669921875,4104.86669921875
AudioMixer4              voice3_mixer;   //xy=1274.86669921875,3685.86669921875
AudioMixer4              voice4_mixer;   //xy=1275.86669921875,3948.86669921875
AudioMixer4              voice2_mixer;   //xy=1279.86669921875,3400.86669921875
AudioEffectEnvelope      voice3_envelope_filter; //xy=1278.86669921875,3791.86669921875
AudioEffectEnvelope      voice2_envelope_filter; //xy=1294.86669921875,3518.86669921875
AudioEffectEnvelope      voice1_envelope_filter; //xy=1296.86669921875,3287.86669921875
AudioMixer4              voice1_mixer;   //xy=1298.86669921875,3130.86669921875
AudioFilterStateVariable filter_string_17; //xy=1320,2457
AudioFilterStateVariable filter_string_21; //xy=1320,2503
AudioFilterStateVariable filter_string_18; //xy=1321,2322
AudioFilterStateVariable filter_string_22; //xy=1320,2595
AudioFilterStateVariable filter_string_19; //xy=1321,2367
AudioFilterStateVariable filter_string_23; //xy=1321,2642
AudioFilterStateVariable filter_string_20; //xy=1322,2412
AudioFilterStateVariable filter_string_24; //xy=1323,2552
AudioFilterStateVariable filter_string_13; //xy=1323,2687
AudioFilterStateVariable filter_string_14; //xy=1324,2779
AudioFilterStateVariable filter_string_15; //xy=1324,2825
AudioFilterStateVariable filter_string_16; //xy=1325,2732
AudioFilterStateVariable filter_string_4; //xy=1337.8666687011719,1102.8666687011719
AudioFilterStateVariable filter_string_5; //xy=1337.8666687011719,1148.8666687011719
AudioFilterStateVariable filter_string_1; //xy=1338.8666687011719,967.8666687011719
AudioFilterStateVariable filter_string_7; //xy=1337.8666687011719,1240.8666687011719
AudioFilterStateVariable filter_string_2; //xy=1338.8666687011719,1012.8666687011719
AudioFilterStateVariable filter_string_8; //xy=1338.8666687011719,1287.8666687011719
AudioFilterStateVariable filter_string_3; //xy=1339.8666687011719,1057.8666687011719
AudioFilterStateVariable filter_string_6; //xy=1340.8666687011719,1197.8666687011719
AudioFilterStateVariable filter_string_9; //xy=1340.8666687011719,1332.8666687011719
AudioFilterStateVariable filter_string_11; //xy=1341.8666687011719,1424.8666687011719
AudioFilterStateVariable filter_string_12; //xy=1341.8666687011719,1470.8666687011719
AudioFilterStateVariable filter_string_10; //xy=1342.8666687011719,1377.8666687011719
AudioMixer4              transient_mix_4; //xy=1352,1661
AudioMixer4              transient_mix_5; //xy=1353,1564
AudioMixer4              transient_mix_6; //xy=1358,1763
AudioMixer4              transient_mix_2; //xy=1369.8666687011719,306.8666687011719
AudioMixer4              transient_mix_1; //xy=1370.8666687011719,209.86666870117188
AudioMixer4              transient_mix_3; //xy=1375.8666687011719,408.8666687011719
AudioSynthWaveform       chords_tremolo_lfo1; //xy=1512,4438
AudioSynthWaveform       chords_tremolo_lfo; //xy=1546.86669921875,3288.86669921875
AudioFilterStateVariable voice4_filter1; //xy=1545,5102
AudioFilterStateVariable voice2_filter1; //xy=1553,4555
AudioFilterStateVariable voice3_filter1; //xy=1552,4831
AudioMixer4              transient_full_mix1; //xy=1582,1633
AudioMixer4              string_mix_4; //xy=1578,2715
AudioMixer4              string_mix_5; //xy=1581,2403
AudioFilterStateVariable voice1_filter1; //xy=1574,4287
AudioFilterStateVariable voice4_filter;  //xy=1579.86669921875,3952.86669921875
AudioMixer4              transient_full_mix; //xy=1599.8666687011719,278.8666687011719
AudioMixer4              string_mix_3;   //xy=1595.8666687011719,1360.8666687011719
AudioFilterStateVariable voice2_filter;  //xy=1587.86669921875,3405.86669921875
AudioFilterStateVariable voice3_filter;  //xy=1586.86669921875,3681.86669921875
AudioMixer4              string_mix_1;   //xy=1598.8666687011719,1048.8666687011719
AudioMixer4              string_mix_6; //xy=1595,2532
AudioMixer4              string_mix_2;   //xy=1612.8666687011719,1177.8666687011719
AudioFilterStateVariable voice1_filter;  //xy=1608.86669921875,3137.86669921875
AudioEffectEnvelope      voice1_envelope1; //xy=1730,4438
AudioEffectEnvelope      voice3_envelope1; //xy=1732,4969
AudioEffectEnvelope      voice2_envelope1; //xy=1735,4689
AudioEffectEnvelope      voice4_envelope1; //xy=1739,5288
AudioEffectEnvelope      voice1_envelope; //xy=1764.86669921875,3288.86669921875
AudioEffectEnvelope      voice3_envelope; //xy=1766.86669921875,3819.86669921875
AudioEffectEnvelope      voice2_envelope; //xy=1769.86669921875,3539.86669921875
AudioEffectEnvelope      voice4_envelope; //xy=1773.86669921875,4138.86669921875
AudioEffectMultiply      voice2_tremolo_mult1; //xy=1778,4551
AudioEffectMultiply      voice3_tremolo_mult1; //xy=1780,4831
AudioEffectMultiply      voice4_tremolo_mult1; //xy=1783,5106
AudioEffectMultiply      voice1_tremolo_mult1; //xy=1788,4281
AudioMixer4              all_string_mix1; //xy=1795,2532
AudioMixer4              all_string_mix; //xy=1812.8666687011719,1177.8666687011719
AudioEffectMultiply      voice2_tremolo_mult; //xy=1812.86669921875,3401.86669921875
AudioEffectMultiply      voice3_tremolo_mult; //xy=1814.86669921875,3681.86669921875
AudioEffectMultiply      voice4_tremolo_mult; //xy=1817.86669921875,3956.86669921875
AudioEffectMultiply      voice1_tremolo_mult; //xy=1822.86669921875,3131.86669921875
AudioEffectWaveshaper    string_waveshape1; //xy=2028,2350
AudioEffectWaveshaper    string_waveshape; //xy=2045.8666687011719,995.8666687011719
AudioMixer4              string_waveshaper_mix1; //xy=2080,2528
AudioMixer4              string_waveshaper_mix; //xy=2097.866668701172,1173.8666687011719
AudioMixer4              chord_voice_mixer1; //xy=2089,4302
AudioMixer4              chord_voice_mixer; //xy=2123.86669921875,3152.86669921875
AudioFilterStateVariable filter_delay_strings1; //xy=2264,2316
AudioEffectWaveshaper    chord_waveshape1; //xy=2267,4198
AudioFilterStateVariable filter_delay_strings; //xy=2281.866668701172,961.8666687011719
AudioEffectWaveshaper    chord_waveshape; //xy=2301.86669921875,3048.86669921875
AudioMixer4              chord_waveshaper_mix1; //xy=2420,4283
AudioMixer4              string_delay_mix1; //xy=2446,2404
AudioMixer4              chord_waveshaper_mix; //xy=2454.86669921875,3133.86669921875
AudioMixer4              string_delay_mix; //xy=2463.866668701172,1049.8666687011719
AudioFilterStateVariable filter_delay_chords1; //xy=2512,4158
AudioFilterStateVariable filter_delay_chords; //xy=2546.86669921875,3008.86669921875
AudioSynthWaveform       string_tremolo_lfo1; //xy=2603,2707
AudioSynthWaveform       string_tremolo_lfo; //xy=2620.866668701172,1352.8666687011719
AudioMixer4              strings_effect_mix1; //xy=2650,2585
AudioMixer4              strings_effect_mix; //xy=2667.866668701172,1230.8666687011719
AudioMixer4              chord_delay_mix1; //xy=2712,4258
AudioMixer4              chord_delay_mix; //xy=2746.86669921875,3108.86669921875
AudioEffectDelay         delay_strings1;  //xy=2760,2415
AudioEffectDelay         delay_strings;  //xy=2777.866668701172,1060.8666687011719
AudioSynthWaveform       string_filter_lfo1; //xy=2856,2754
AudioMixer4              chords_effect_mix1; //xy=2860,4406
AudioSynthWaveform       string_filter_lfo; //xy=2873.866668701172,1399.8666687011719
AudioEffectMultiply      string_multiply1; //xy=2877,2666
AudioEffectMultiply      string_multiply; //xy=2894.866668701172,1311.8666687011719
AudioMixer4              chords_effect_mix; //xy=2894.86669921875,3256.86669921875
AudioEffectDelay         delay_chords1; //xy=3010,4278
AudioEffectDelay         delay_chords;   //xy=3044.86669921875,3128.86669921875
AudioFilterStateVariable string_filter1; //xy=3075,2700
AudioFilterStateVariable string_filter;  //xy=3092.866668701172,1345.8666687011719
AudioFilterStateVariable chords_main_filter1; //xy=3178,4339
AudioFilterStateVariable chords_main_filter; //xy=3212.86669921875,3189.86669921875
AudioInputI2S            i2s1;           //xy=3280.866668701172,1096.8666687011719
AudioMixer4              string_filter_mixer1; //xy=3280,2701
AudioMixer4              string_filter_mixer; //xy=3297.866668701172,1346.8666687011719
AudioMixer4              chords_main_filter_mixer1; //xy=3461,4354
AudioMixer4              chords_main_filter_mixer; //xy=3495.86669921875,3204.86669921875
AudioMixer4              string_filter_mixer2; //xy=3767.5,1769
AudioMixer4              chords_main_filter_mixer2; //xy=3986.5,3728
AudioConnection          patchCord1(string_vibrato_dc1, envelope_string_vibrato_dc1);
AudioConnection          patchCord2(string_vibrato_lfo1, envelope_string_vibrato_lfo1);
AudioConnection          patchCord3(string_vibrato_dc, envelope_string_vibrato_dc);
AudioConnection          patchCord4(string_vibrato_lfo, envelope_string_vibrato_lfo);
AudioConnection          patchCord5(chords_vibrato_dc1, voice1_vibrato_dc_envelope1);
AudioConnection          patchCord6(chords_vibrato_dc1, voice2_vibrato_dc_envelope1);
AudioConnection          patchCord7(chords_vibrato_dc1, voice3_vibrato_dc_envelope1);
AudioConnection          patchCord8(chords_vibrato_dc1, voice4_vibrato_dc_envelope1);
AudioConnection          patchCord9(chords_vibrato_lfo1, voice1_vibrato_envelope1);
AudioConnection          patchCord10(chords_vibrato_lfo1, voice2_vibrato_envelope1);
AudioConnection          patchCord11(chords_vibrato_lfo1, voice3_vibrato_envelope1);
AudioConnection          patchCord12(chords_vibrato_lfo1, voice4_vibrato_envelope1);
AudioConnection          patchCord13(chords_vibrato_dc, voice1_vibrato_dc_envelope);
AudioConnection          patchCord14(chords_vibrato_dc, voice2_vibrato_dc_envelope);
AudioConnection          patchCord15(chords_vibrato_dc, voice3_vibrato_dc_envelope);
AudioConnection          patchCord16(chords_vibrato_dc, voice4_vibrato_dc_envelope);
AudioConnection          patchCord17(chords_vibrato_lfo, voice1_vibrato_envelope);
AudioConnection          patchCord18(chords_vibrato_lfo, voice2_vibrato_envelope);
AudioConnection          patchCord19(chords_vibrato_lfo, voice3_vibrato_envelope);
AudioConnection          patchCord20(chords_vibrato_lfo, voice4_vibrato_envelope);
AudioConnection          patchCord21(envelope_string_vibrato_dc1, 0, string_vibrato_mixer1, 1);
AudioConnection          patchCord22(envelope_string_vibrato_dc, 0, string_vibrato_mixer, 1);
AudioConnection          patchCord23(envelope_string_vibrato_lfo1, 0, string_vibrato_mixer1, 0);
AudioConnection          patchCord24(envelope_string_vibrato_lfo, 0, string_vibrato_mixer, 0);
AudioConnection          patchCord25(voice4_vibrato_dc_envelope1, 0, voice4_vibrato_mixer1, 1);
AudioConnection          patchCord26(voice3_vibrato_envelope1, 0, voice3_vibrato_mixer1, 0);
AudioConnection          patchCord27(voice3_vibrato_dc_envelope1, 0, voice3_vibrato_mixer1, 1);
AudioConnection          patchCord28(voice3_frequency_dc1, 0, voice3_vibrato_mixer1, 2);
AudioConnection          patchCord29(voice4_vibrato_envelope1, 0, voice4_vibrato_mixer1, 0);
AudioConnection          patchCord30(voice4_frequency_dc1, 0, voice4_vibrato_mixer1, 2);
AudioConnection          patchCord31(voice2_vibrato_dc_envelope1, 0, voice2_vibrato_mixer1, 1);
AudioConnection          patchCord32(voice2_vibrato_envelope1, 0, voice2_vibrato_mixer1, 0);
AudioConnection          patchCord33(voice2_frequency_dc1, 0, voice2_vibrato_mixer1, 2);
AudioConnection          patchCord34(voice4_vibrato_dc_envelope, 0, voice4_vibrato_mixer, 1);
AudioConnection          patchCord35(voice3_vibrato_envelope, 0, voice3_vibrato_mixer, 0);
AudioConnection          patchCord36(voice1_vibrato_envelope1, 0, voice1_vibrato_mixer1, 0);
AudioConnection          patchCord37(voice1_vibrato_dc_envelope1, 0, voice1_vibrato_mixer1, 1);
AudioConnection          patchCord38(voice3_vibrato_dc_envelope, 0, voice3_vibrato_mixer, 1);
AudioConnection          patchCord39(voice1_frequency_dc1, 0, voice1_vibrato_mixer1, 2);
AudioConnection          patchCord40(voice3_frequency_dc, 0, voice3_vibrato_mixer, 2);
AudioConnection          patchCord41(voice4_vibrato_envelope, 0, voice4_vibrato_mixer, 0);
AudioConnection          patchCord42(voice4_frequency_dc, 0, voice4_vibrato_mixer, 2);
AudioConnection          patchCord43(voice2_vibrato_dc_envelope, 0, voice2_vibrato_mixer, 1);
AudioConnection          patchCord44(voice2_vibrato_envelope, 0, voice2_vibrato_mixer, 0);
AudioConnection          patchCord45(string_vibrato_mixer1, 0, waveform_string_18, 0);
AudioConnection          patchCord46(string_vibrato_mixer1, 0, waveform_string_19, 0);
AudioConnection          patchCord47(string_vibrato_mixer1, 0, waveform_string_20, 0);
AudioConnection          patchCord48(string_vibrato_mixer1, 0, waveform_string_17, 0);
AudioConnection          patchCord49(string_vibrato_mixer1, 0, waveform_string_21, 0);
AudioConnection          patchCord50(string_vibrato_mixer1, 0, waveform_string_24, 0);
AudioConnection          patchCord51(string_vibrato_mixer1, 0, waveform_string_22, 0);
AudioConnection          patchCord52(string_vibrato_mixer1, 0, waveform_string_23, 0);
AudioConnection          patchCord53(string_vibrato_mixer1, 0, waveform_string_13, 0);
AudioConnection          patchCord54(string_vibrato_mixer1, 0, waveform_string_16, 0);
AudioConnection          patchCord55(string_vibrato_mixer1, 0, waveform_string_14, 0);
AudioConnection          patchCord56(string_vibrato_mixer1, 0, waveform_string_15, 0);
AudioConnection          patchCord57(voice2_frequency_dc, 0, voice2_vibrato_mixer, 2);
AudioConnection          patchCord58(voice1_vibrato_envelope, 0, voice1_vibrato_mixer, 0);
AudioConnection          patchCord59(voice1_vibrato_dc_envelope, 0, voice1_vibrato_mixer, 1);
AudioConnection          patchCord60(string_vibrato_mixer, 0, waveform_string_1, 0);
AudioConnection          patchCord61(string_vibrato_mixer, 0, waveform_string_2, 0);
AudioConnection          patchCord62(string_vibrato_mixer, 0, waveform_string_3, 0);
AudioConnection          patchCord63(string_vibrato_mixer, 0, waveform_string_4, 0);
AudioConnection          patchCord64(string_vibrato_mixer, 0, waveform_string_5, 0);
AudioConnection          patchCord65(string_vibrato_mixer, 0, waveform_string_6, 0);
AudioConnection          patchCord66(string_vibrato_mixer, 0, waveform_string_7, 0);
AudioConnection          patchCord67(string_vibrato_mixer, 0, waveform_string_8, 0);
AudioConnection          patchCord68(string_vibrato_mixer, 0, waveform_string_9, 0);
AudioConnection          patchCord69(string_vibrato_mixer, 0, waveform_string_10, 0);
AudioConnection          patchCord70(string_vibrato_mixer, 0, waveform_string_11, 0);
AudioConnection          patchCord71(string_vibrato_mixer, 0, waveform_string_12, 0);
AudioConnection          patchCord72(voice1_frequency_dc, 0, voice1_vibrato_mixer, 2);
AudioConnection          patchCord73(voice3_vibrato_mixer1, 0, voice3_osc5, 0);
AudioConnection          patchCord74(voice3_vibrato_mixer1, 0, voice3_osc4, 0);
AudioConnection          patchCord75(voice3_vibrato_mixer1, 0, voice_osc34, 0);
AudioConnection          patchCord76(voice4_vibrato_mixer1, 0, voice4_osc6, 0);
AudioConnection          patchCord77(voice4_vibrato_mixer1, 0, voice4_osc5, 0);
AudioConnection          patchCord78(voice4_vibrato_mixer1, 0, voice4_osc4, 0);
AudioConnection          patchCord79(waveform_string_17, envelope_string_17);
AudioConnection          patchCord80(waveform_string_19, envelope_string_19);
AudioConnection          patchCord81(waveform_string_21, envelope_string_21);
AudioConnection          patchCord82(waveform_string_22, envelope_string_22);
AudioConnection          patchCord83(waveform_string_16, envelope_string_16);
AudioConnection          patchCord84(waveform_string_20, envelope_string_20);
AudioConnection          patchCord85(waveform_string_15, envelope_string_15);
AudioConnection          patchCord86(waveform_string_24, envelope_string_24);
AudioConnection          patchCord87(waveform_string_23, envelope_string_23);
AudioConnection          patchCord88(waveform_string_18, envelope_string_18);
AudioConnection          patchCord89(waveform_string_13, envelope_string_13);
AudioConnection          patchCord90(waveform_string_14, envelope_string_14);
AudioConnection          patchCord91(waveform_string_4, envelope_string_4);
AudioConnection          patchCord92(waveform_string_2, envelope_string_2);
AudioConnection          patchCord93(waveform_string_5, envelope_string_5);
AudioConnection          patchCord94(waveform_string_7, envelope_string_7);
AudioConnection          patchCord95(waveform_string_10, envelope_string_10);
AudioConnection          patchCord96(waveform_string_3, envelope_string_3);
AudioConnection          patchCord97(waveform_string_12, envelope_string_12);
AudioConnection          patchCord98(waveform_string_6, envelope_string_6);
AudioConnection          patchCord99(waveform_string_8, envelope_string_8);
AudioConnection          patchCord100(waveform_string_1, envelope_string_1);
AudioConnection          patchCord101(waveform_string_9, envelope_string_9);
AudioConnection          patchCord102(waveform_string_11, envelope_string_11);
AudioConnection          patchCord103(voice2_vibrato_mixer1, 0, voice2_osc5, 0);
AudioConnection          patchCord104(voice2_vibrato_mixer1, 0, voice_osc23, 0);
AudioConnection          patchCord105(voice2_vibrato_mixer1, 0, voice2_osc4, 0);
AudioConnection          patchCord106(voice3_vibrato_mixer, 0, voice3_osc1, 0);
AudioConnection          patchCord107(voice3_vibrato_mixer, 0, voice3_osc2, 0);
AudioConnection          patchCord108(voice3_vibrato_mixer, 0, voice3_osc3, 0);
AudioConnection          patchCord109(filter_dc1, envelope_filter_18);
AudioConnection          patchCord110(filter_dc1, envelope_filter_19);
AudioConnection          patchCord111(filter_dc1, envelope_filter_20);
AudioConnection          patchCord112(filter_dc1, envelope_filter_17);
AudioConnection          patchCord113(filter_dc1, envelope_filter_21);
AudioConnection          patchCord114(filter_dc1, envelope_filter_24);
AudioConnection          patchCord115(filter_dc1, envelope_filter_22);
AudioConnection          patchCord116(filter_dc1, envelope_filter_23);
AudioConnection          patchCord117(filter_dc1, envelope_filter_13);
AudioConnection          patchCord118(filter_dc1, envelope_filter_16);
AudioConnection          patchCord119(filter_dc1, envelope_filter_14);
AudioConnection          patchCord120(filter_dc1, envelope_filter_15);
AudioConnection          patchCord121(voice1_vibrato_mixer1, 0, voice_osc12, 0);
AudioConnection          patchCord122(voice1_vibrato_mixer1, 0, voice1_osc5, 0);
AudioConnection          patchCord123(voice1_vibrato_mixer1, 0, voice1_osc4, 0);
AudioConnection          patchCord124(voice4_vibrato_mixer, 0, voice4_osc1, 0);
AudioConnection          patchCord125(voice4_vibrato_mixer, 0, voice4_osc2, 0);
AudioConnection          patchCord126(voice4_vibrato_mixer, 0, voice4_osc3, 0);
AudioConnection          patchCord127(filter_dc, envelope_filter_1);
AudioConnection          patchCord128(filter_dc, envelope_filter_2);
AudioConnection          patchCord129(filter_dc, envelope_filter_3);
AudioConnection          patchCord130(filter_dc, envelope_filter_4);
AudioConnection          patchCord131(filter_dc, envelope_filter_5);
AudioConnection          patchCord132(filter_dc, envelope_filter_6);
AudioConnection          patchCord133(filter_dc, envelope_filter_7);
AudioConnection          patchCord134(filter_dc, envelope_filter_8);
AudioConnection          patchCord135(filter_dc, envelope_filter_9);
AudioConnection          patchCord136(filter_dc, envelope_filter_10);
AudioConnection          patchCord137(filter_dc, envelope_filter_11);
AudioConnection          patchCord138(filter_dc, envelope_filter_12);
AudioConnection          patchCord139(waveform_transient_22, envelope_transient_22);
AudioConnection          patchCord140(waveform_transient_15, envelope_transient_15);
AudioConnection          patchCord141(waveform_transient_16, envelope_transient_16);
AudioConnection          patchCord142(waveform_transient_18, envelope_transient_18);
AudioConnection          patchCord143(waveform_transient_19, envelope_transient_19);
AudioConnection          patchCord144(waveform_transient_20, envelope_transient_20);
AudioConnection          patchCord145(waveform_transient_17, envelope_transient_17);
AudioConnection          patchCord146(waveform_transient_13, envelope_transient_13);
AudioConnection          patchCord147(waveform_transient_14, envelope_transient_14);
AudioConnection          patchCord148(waveform_transient_23, envelope_transient_23);
AudioConnection          patchCord149(waveform_transient_21, envelope_transient_21);
AudioConnection          patchCord150(waveform_transient_24, envelope_transient_24);
AudioConnection          patchCord151(voice2_vibrato_mixer, 0, voice2_osc1, 0);
AudioConnection          patchCord152(voice2_vibrato_mixer, 0, voice2_osc2, 0);
AudioConnection          patchCord153(voice2_vibrato_mixer, 0, voice2_osc3, 0);
AudioConnection          patchCord154(waveform_transient_9, envelope_transient_9);
AudioConnection          patchCord155(waveform_transient_5, envelope_transient_5);
AudioConnection          patchCord156(waveform_transient_6, envelope_transient_6);
AudioConnection          patchCord157(waveform_transient_1, envelope_transient_1);
AudioConnection          patchCord158(waveform_transient_2, envelope_transient_2);
AudioConnection          patchCord159(waveform_transient_3, envelope_transient_3);
AudioConnection          patchCord160(waveform_transient_4, envelope_transient_4);
AudioConnection          patchCord161(waveform_transient_7, envelope_transient_7);
AudioConnection          patchCord162(waveform_transient_8, envelope_transient_8);
AudioConnection          patchCord163(waveform_transient_11, envelope_transient_11);
AudioConnection          patchCord164(voice1_vibrato_mixer, 0, voice1_osc1, 0);
AudioConnection          patchCord165(voice1_vibrato_mixer, 0, voice1_osc2, 0);
AudioConnection          patchCord166(voice1_vibrato_mixer, 0, voice1_osc3, 0);
AudioConnection          patchCord167(waveform_transient_10, envelope_transient_10);
AudioConnection          patchCord168(waveform_transient_12, envelope_transient_12);
AudioConnection          patchCord169(voice3_noise1, 0, voice3_mixer1, 3);
AudioConnection          patchCord170(voice_osc34, 0, voice3_mixer1, 2);
AudioConnection          patchCord171(voice4_noise1, 0, voice4_mixer1, 3);
AudioConnection          patchCord172(voice3_osc4, 0, voice3_mixer1, 1);
AudioConnection          patchCord173(voice4_osc4, 0, voice4_mixer1, 2);
AudioConnection          patchCord174(voice3_osc5, 0, voice3_mixer1, 0);
AudioConnection          patchCord175(voice4_osc5, 0, voice4_mixer1, 1);
AudioConnection          patchCord176(voice4_osc6, 0, voice4_mixer1, 0);
AudioConnection          patchCord177(chords_filter_LFO1, voice1_envelope_filter1);
AudioConnection          patchCord178(chords_filter_LFO1, voice2_envelope_filter1);
AudioConnection          patchCord179(chords_filter_LFO1, voice3_envelope_filter1);
AudioConnection          patchCord180(chords_filter_LFO1, voice4_envelope_filter1);
AudioConnection          patchCord181(voice2_noise1, 0, voice2_mixer1, 3);
AudioConnection          patchCord182(voice2_osc4, 0, voice2_mixer1, 2);
AudioConnection          patchCord183(voice_osc23, 0, voice2_mixer1, 1);
AudioConnection          patchCord184(voice2_osc5, 0, voice2_mixer1, 0);
AudioConnection          patchCord185(voice3_noise, 0, voice3_mixer, 3);
AudioConnection          patchCord186(envelope_filter_19, 0, filter_string_19, 1);
AudioConnection          patchCord187(envelope_filter_20, 0, filter_string_20, 1);
AudioConnection          patchCord188(envelope_filter_21, 0, filter_string_21, 1);
AudioConnection          patchCord189(envelope_filter_24, 0, filter_string_24, 1);
AudioConnection          patchCord190(envelope_filter_22, 0, filter_string_22, 1);
AudioConnection          patchCord191(voice3_osc3, 0, voice3_mixer, 2);
AudioConnection          patchCord192(envelope_filter_18, 0, filter_string_18, 1);
AudioConnection          patchCord193(envelope_filter_17, 0, filter_string_17, 1);
AudioConnection          patchCord194(envelope_filter_15, 0, filter_string_15, 1);
AudioConnection          patchCord195(envelope_filter_13, 0, filter_string_13, 1);
AudioConnection          patchCord196(voice4_noise, 0, voice4_mixer, 3);
AudioConnection          patchCord197(envelope_filter_14, 0, filter_string_14, 1);
AudioConnection          patchCord198(voice3_osc2, 0, voice3_mixer, 1);
AudioConnection          patchCord199(voice4_osc3, 0, voice4_mixer, 2);
AudioConnection          patchCord200(voice1_noise1, 0, voice1_mixer1, 3);
AudioConnection          patchCord201(envelope_filter_16, 0, filter_string_16, 1);
AudioConnection          patchCord202(envelope_filter_23, 0, filter_string_23, 1);
AudioConnection          patchCord203(voice3_osc1, 0, voice3_mixer, 0);
AudioConnection          patchCord204(voice4_osc2, 0, voice4_mixer, 1);
AudioConnection          patchCord205(envelope_string_24, 0, filter_string_24, 0);
AudioConnection          patchCord206(envelope_string_23, 0, filter_string_23, 0);
AudioConnection          patchCord207(envelope_string_16, 0, filter_string_16, 0);
AudioConnection          patchCord208(envelope_string_15, 0, filter_string_15, 0);
AudioConnection          patchCord209(envelope_string_21, 0, filter_string_21, 0);
AudioConnection          patchCord210(envelope_string_22, 0, filter_string_22, 0);
AudioConnection          patchCord211(voice4_osc1, 0, voice4_mixer, 0);
AudioConnection          patchCord212(envelope_string_13, 0, filter_string_13, 0);
AudioConnection          patchCord213(envelope_string_20, 0, filter_string_20, 0);
AudioConnection          patchCord214(voice1_osc4, 0, voice1_mixer1, 2);
AudioConnection          patchCord215(envelope_string_17, 0, filter_string_17, 0);
AudioConnection          patchCord216(voice_osc12, 0, voice1_mixer1, 0);
AudioConnection          patchCord217(voice1_osc5, 0, voice1_mixer1, 1);
AudioConnection          patchCord218(envelope_string_14, 0, filter_string_14, 0);
AudioConnection          patchCord219(envelope_string_18, 0, filter_string_18, 0);
AudioConnection          patchCord220(envelope_string_19, 0, filter_string_19, 0);
AudioConnection          patchCord221(envelope_filter_2, 0, filter_string_2, 1);
AudioConnection          patchCord222(envelope_filter_3, 0, filter_string_3, 1);
AudioConnection          patchCord223(envelope_filter_5, 0, filter_string_5, 1);
AudioConnection          patchCord224(envelope_filter_6, 0, filter_string_6, 1);
AudioConnection          patchCord225(envelope_filter_7, 0, filter_string_7, 1);
AudioConnection          patchCord226(envelope_filter_1, 0, filter_string_1, 1);
AudioConnection          patchCord227(envelope_filter_4, 0, filter_string_4, 1);
AudioConnection          patchCord228(envelope_filter_12, 0, filter_string_12, 1);
AudioConnection          patchCord229(envelope_filter_9, 0, filter_string_9, 1);
AudioConnection          patchCord230(envelope_filter_11, 0, filter_string_11, 1);
AudioConnection          patchCord231(envelope_filter_10, 0, filter_string_10, 1);
AudioConnection          patchCord232(envelope_filter_8, 0, filter_string_8, 1);
AudioConnection          patchCord233(chords_filter_LFO, voice1_envelope_filter);
AudioConnection          patchCord234(chords_filter_LFO, voice2_envelope_filter);
AudioConnection          patchCord235(chords_filter_LFO, voice3_envelope_filter);
AudioConnection          patchCord236(chords_filter_LFO, voice4_envelope_filter);
AudioConnection          patchCord237(envelope_string_6, 0, filter_string_6, 0);
AudioConnection          patchCord238(envelope_string_8, 0, filter_string_8, 0);
AudioConnection          patchCord239(envelope_string_10, 0, filter_string_10, 0);
AudioConnection          patchCord240(envelope_string_12, 0, filter_string_12, 0);
AudioConnection          patchCord241(envelope_string_5, 0, filter_string_5, 0);
AudioConnection          patchCord242(envelope_string_7, 0, filter_string_7, 0);
AudioConnection          patchCord243(envelope_string_9, 0, filter_string_9, 0);
AudioConnection          patchCord244(envelope_string_3, 0, filter_string_3, 0);
AudioConnection          patchCord245(envelope_string_4, 0, filter_string_4, 0);
AudioConnection          patchCord246(voice2_noise, 0, voice2_mixer, 3);
AudioConnection          patchCord247(envelope_string_11, 0, filter_string_11, 0);
AudioConnection          patchCord248(envelope_string_1, 0, filter_string_1, 0);
AudioConnection          patchCord249(envelope_string_2, 0, filter_string_2, 0);
AudioConnection          patchCord250(voice2_osc3, 0, voice2_mixer, 2);
AudioConnection          patchCord251(voice2_osc2, 0, voice2_mixer, 1);
AudioConnection          patchCord252(voice2_osc1, 0, voice2_mixer, 0);
AudioConnection          patchCord253(voice1_noise, 0, voice1_mixer, 3);
AudioConnection          patchCord254(voice1_osc3, 0, voice1_mixer, 2);
AudioConnection          patchCord255(voice1_osc1, 0, voice1_mixer, 0);
AudioConnection          patchCord256(voice1_osc2, 0, voice1_mixer, 1);
AudioConnection          patchCord257(envelope_transient_13, 0, transient_mix_4, 2);
AudioConnection          patchCord258(envelope_transient_21, 0, transient_mix_6, 1);
AudioConnection          patchCord259(envelope_transient_14, 0, transient_mix_4, 3);
AudioConnection          patchCord260(envelope_transient_22, 0, transient_mix_6, 0);
AudioConnection          patchCord261(envelope_transient_15, 0, transient_mix_4, 0);
AudioConnection          patchCord262(envelope_transient_16, 0, transient_mix_4, 1);
AudioConnection          patchCord263(envelope_transient_23, 0, transient_mix_6, 2);
AudioConnection          patchCord264(envelope_transient_17, 0, transient_mix_5, 3);
AudioConnection          patchCord265(envelope_transient_24, 0, transient_mix_6, 3);
AudioConnection          patchCord266(envelope_transient_18, 0, transient_mix_5, 0);
AudioConnection          patchCord267(envelope_transient_19, 0, transient_mix_5, 1);
AudioConnection          patchCord268(envelope_transient_20, 0, transient_mix_5, 2);
AudioConnection          patchCord269(envelope_transient_7, 0, transient_mix_2, 2);
AudioConnection          patchCord270(envelope_transient_10, 0, transient_mix_3, 1);
AudioConnection          patchCord271(envelope_transient_8, 0, transient_mix_2, 3);
AudioConnection          patchCord272(envelope_transient_9, 0, transient_mix_3, 0);
AudioConnection          patchCord273(envelope_transient_5, 0, transient_mix_2, 0);
AudioConnection          patchCord274(envelope_transient_6, 0, transient_mix_2, 1);
AudioConnection          patchCord275(envelope_transient_11, 0, transient_mix_3, 2);
AudioConnection          patchCord276(envelope_transient_4, 0, transient_mix_1, 3);
AudioConnection          patchCord277(envelope_transient_12, 0, transient_mix_3, 3);
AudioConnection          patchCord278(envelope_transient_1, 0, transient_mix_1, 0);
AudioConnection          patchCord279(envelope_transient_2, 0, transient_mix_1, 1);
AudioConnection          patchCord280(envelope_transient_3, 0, transient_mix_1, 2);
AudioConnection          patchCord281(voice4_envelope_filter1, 0, voice4_filter1, 1);
AudioConnection          patchCord282(voice3_mixer1, 0, voice3_filter1, 0);
AudioConnection          patchCord283(voice4_mixer1, 0, voice4_filter1, 0);
AudioConnection          patchCord284(voice2_mixer1, 0, voice2_filter1, 0);
AudioConnection          patchCord285(voice3_envelope_filter1, 0, voice3_filter1, 1);
AudioConnection          patchCord286(voice2_envelope_filter1, 0, voice2_filter1, 1);
AudioConnection          patchCord287(voice1_envelope_filter1, 0, voice1_filter1, 1);
AudioConnection          patchCord288(voice1_mixer1, 0, voice1_filter1, 0);
AudioConnection          patchCord289(voice4_envelope_filter, 0, voice4_filter, 1);
AudioConnection          patchCord290(voice3_mixer, 0, voice3_filter, 0);
AudioConnection          patchCord291(voice4_mixer, 0, voice4_filter, 0);
AudioConnection          patchCord292(voice2_mixer, 0, voice2_filter, 0);
AudioConnection          patchCord293(voice3_envelope_filter, 0, voice3_filter, 1);
AudioConnection          patchCord294(voice2_envelope_filter, 0, voice2_filter, 1);
AudioConnection          patchCord295(voice1_envelope_filter, 0, voice1_filter, 1);
AudioConnection          patchCord296(voice1_mixer, 0, voice1_filter, 0);
AudioConnection          patchCord297(filter_string_17, 0, string_mix_5, 3);
AudioConnection          patchCord298(filter_string_21, 0, string_mix_6, 0);
AudioConnection          patchCord299(filter_string_18, 0, string_mix_5, 0);
AudioConnection          patchCord300(filter_string_22, 0, string_mix_6, 2);
AudioConnection          patchCord301(filter_string_19, 0, string_mix_5, 1);
AudioConnection          patchCord302(filter_string_23, 0, string_mix_6, 3);
AudioConnection          patchCord303(filter_string_20, 0, string_mix_5, 2);
AudioConnection          patchCord304(filter_string_24, 0, string_mix_6, 1);
AudioConnection          patchCord305(filter_string_13, 0, string_mix_4, 0);
AudioConnection          patchCord306(filter_string_14, 0, string_mix_4, 2);
AudioConnection          patchCord307(filter_string_15, 0, string_mix_4, 3);
AudioConnection          patchCord308(filter_string_16, 0, string_mix_4, 1);
AudioConnection          patchCord309(filter_string_4, 0, string_mix_1, 3);
AudioConnection          patchCord310(filter_string_5, 0, string_mix_2, 0);
AudioConnection          patchCord311(filter_string_1, 0, string_mix_1, 0);
AudioConnection          patchCord312(filter_string_7, 0, string_mix_2, 2);
AudioConnection          patchCord313(filter_string_2, 0, string_mix_1, 1);
AudioConnection          patchCord314(filter_string_8, 0, string_mix_2, 3);
AudioConnection          patchCord315(filter_string_3, 0, string_mix_1, 2);
AudioConnection          patchCord316(filter_string_6, 0, string_mix_2, 1);
AudioConnection          patchCord317(filter_string_9, 0, string_mix_3, 0);
AudioConnection          patchCord318(filter_string_11, 0, string_mix_3, 2);
AudioConnection          patchCord319(filter_string_12, 0, string_mix_3, 3);
AudioConnection          patchCord320(filter_string_10, 0, string_mix_3, 1);
AudioConnection          patchCord321(transient_mix_4, 0, transient_full_mix1, 1);
AudioConnection          patchCord322(transient_mix_5, 0, transient_full_mix1, 0);
AudioConnection          patchCord323(transient_mix_6, 0, transient_full_mix1, 2);
AudioConnection          patchCord324(transient_mix_2, 0, transient_full_mix, 1);
AudioConnection          patchCord325(transient_mix_1, 0, transient_full_mix, 0);
AudioConnection          patchCord326(transient_mix_3, 0, transient_full_mix, 2);
AudioConnection          patchCord327(chords_tremolo_lfo1, voice1_envelope1);
AudioConnection          patchCord328(chords_tremolo_lfo1, voice2_envelope1);
AudioConnection          patchCord329(chords_tremolo_lfo1, voice3_envelope1);
AudioConnection          patchCord330(chords_tremolo_lfo1, voice4_envelope1);
AudioConnection          patchCord331(chords_tremolo_lfo, voice1_envelope);
AudioConnection          patchCord332(chords_tremolo_lfo, voice2_envelope);
AudioConnection          patchCord333(chords_tremolo_lfo, voice3_envelope);
AudioConnection          patchCord334(chords_tremolo_lfo, voice4_envelope);
AudioConnection          patchCord335(voice4_filter1, 0, voice4_tremolo_mult1, 0);
AudioConnection          patchCord336(voice2_filter1, 0, voice2_tremolo_mult1, 0);
AudioConnection          patchCord337(voice3_filter1, 0, voice3_tremolo_mult1, 0);
AudioConnection          patchCord338(transient_full_mix1, 0, all_string_mix1, 3);
AudioConnection          patchCord339(string_mix_4, 0, all_string_mix1, 2);
AudioConnection          patchCord340(string_mix_5, 0, all_string_mix1, 0);
AudioConnection          patchCord341(voice1_filter1, 0, voice1_tremolo_mult1, 0);
AudioConnection          patchCord342(voice4_filter, 0, voice4_tremolo_mult, 0);
AudioConnection          patchCord343(transient_full_mix, 0, all_string_mix, 3);
AudioConnection          patchCord344(string_mix_3, 0, all_string_mix, 2);
AudioConnection          patchCord345(voice2_filter, 0, voice2_tremolo_mult, 0);
AudioConnection          patchCord346(voice3_filter, 0, voice3_tremolo_mult, 0);
AudioConnection          patchCord347(string_mix_1, 0, all_string_mix, 0);
AudioConnection          patchCord348(string_mix_6, 0, all_string_mix1, 1);
AudioConnection          patchCord349(string_mix_2, 0, all_string_mix, 1);
AudioConnection          patchCord350(voice1_filter, 0, voice1_tremolo_mult, 0);
AudioConnection          patchCord351(voice1_envelope1, 0, voice1_tremolo_mult1, 1);
AudioConnection          patchCord352(voice3_envelope1, 0, voice3_tremolo_mult1, 1);
AudioConnection          patchCord353(voice2_envelope1, 0, voice2_tremolo_mult1, 1);
AudioConnection          patchCord354(voice4_envelope1, 0, voice4_tremolo_mult1, 1);
AudioConnection          patchCord355(voice1_envelope, 0, voice1_tremolo_mult, 1);
AudioConnection          patchCord356(voice3_envelope, 0, voice3_tremolo_mult, 1);
AudioConnection          patchCord357(voice2_envelope, 0, voice2_tremolo_mult, 1);
AudioConnection          patchCord358(voice4_envelope, 0, voice4_tremolo_mult, 1);
AudioConnection          patchCord359(voice2_tremolo_mult1, 0, chord_voice_mixer1, 1);
AudioConnection          patchCord360(voice3_tremolo_mult1, 0, chord_voice_mixer1, 2);
AudioConnection          patchCord361(voice4_tremolo_mult1, 0, chord_voice_mixer1, 3);
AudioConnection          patchCord362(voice1_tremolo_mult1, 0, chord_voice_mixer1, 0);
AudioConnection          patchCord363(all_string_mix1, string_waveshape1);
AudioConnection          patchCord364(all_string_mix1, 0, string_waveshaper_mix1, 0);
AudioConnection          patchCord365(all_string_mix, string_waveshape);
AudioConnection          patchCord366(all_string_mix, 0, string_waveshaper_mix, 0);
AudioConnection          patchCord367(voice2_tremolo_mult, 0, chord_voice_mixer, 1);
AudioConnection          patchCord368(voice3_tremolo_mult, 0, chord_voice_mixer, 2);
AudioConnection          patchCord369(voice4_tremolo_mult, 0, chord_voice_mixer, 3);
AudioConnection          patchCord370(voice1_tremolo_mult, 0, chord_voice_mixer, 0);
AudioConnection          patchCord371(string_waveshape1, 0, string_waveshaper_mix1, 1);
AudioConnection          patchCord372(string_waveshape, 0, string_waveshaper_mix, 1);
AudioConnection          patchCord373(string_waveshaper_mix1, 0, strings_effect_mix1, 0);
AudioConnection          patchCord374(string_waveshaper_mix1, 0, string_delay_mix1, 0);
AudioConnection          patchCord375(string_waveshaper_mix, 0, strings_effect_mix, 0);
AudioConnection          patchCord376(string_waveshaper_mix, 0, string_delay_mix, 0);
AudioConnection          patchCord377(chord_voice_mixer1, chord_waveshape1);
AudioConnection          patchCord378(chord_voice_mixer1, 0, chord_waveshaper_mix1, 0);
AudioConnection          patchCord379(chord_voice_mixer, chord_waveshape);
AudioConnection          patchCord380(chord_voice_mixer, 0, chord_waveshaper_mix, 0);
AudioConnection          patchCord381(filter_delay_strings1, 0, string_delay_mix1, 1);
AudioConnection          patchCord382(filter_delay_strings1, 1, string_delay_mix1, 2);
AudioConnection          patchCord383(filter_delay_strings1, 2, string_delay_mix1, 3);
AudioConnection          patchCord384(chord_waveshape1, 0, chord_waveshaper_mix1, 1);
AudioConnection          patchCord385(filter_delay_strings, 0, string_delay_mix, 1);
AudioConnection          patchCord386(filter_delay_strings, 1, string_delay_mix, 2);
AudioConnection          patchCord387(filter_delay_strings, 2, string_delay_mix, 3);
AudioConnection          patchCord388(chord_waveshape, 0, chord_waveshaper_mix, 1);
AudioConnection          patchCord389(chord_waveshaper_mix1, 0, chord_delay_mix1, 0);
AudioConnection          patchCord390(chord_waveshaper_mix1, 0, chords_effect_mix1, 0);
AudioConnection          patchCord391(string_delay_mix1, delay_strings1);
AudioConnection          patchCord392(string_delay_mix1, 0, strings_effect_mix1, 1);
AudioConnection          patchCord393(chord_waveshaper_mix, 0, chord_delay_mix, 0);
AudioConnection          patchCord394(chord_waveshaper_mix, 0, chords_effect_mix, 0);
AudioConnection          patchCord395(string_delay_mix, delay_strings);
AudioConnection          patchCord396(string_delay_mix, 0, strings_effect_mix, 1);
AudioConnection          patchCord397(filter_delay_chords1, 0, chord_delay_mix1, 1);
AudioConnection          patchCord398(filter_delay_chords1, 1, chord_delay_mix1, 2);
AudioConnection          patchCord399(filter_delay_chords1, 2, chord_delay_mix1, 3);
AudioConnection          patchCord400(filter_delay_chords, 0, chord_delay_mix, 1);
AudioConnection          patchCord401(filter_delay_chords, 1, chord_delay_mix, 2);
AudioConnection          patchCord402(filter_delay_chords, 2, chord_delay_mix, 3);
AudioConnection          patchCord403(string_tremolo_lfo1, 0, string_multiply1, 1);
AudioConnection          patchCord404(string_tremolo_lfo, 0, string_multiply, 1);
AudioConnection          patchCord405(strings_effect_mix1, 0, string_multiply1, 0);
AudioConnection          patchCord406(strings_effect_mix, 0, string_multiply, 0);
AudioConnection          patchCord407(chord_delay_mix1, delay_chords1);
AudioConnection          patchCord408(chord_delay_mix1, 0, chords_effect_mix1, 1);
AudioConnection          patchCord409(chord_delay_mix, delay_chords);
AudioConnection          patchCord410(chord_delay_mix, 0, chords_effect_mix, 1);
AudioConnection          patchCord411(delay_strings1, 0, filter_delay_strings1, 0);
AudioConnection          patchCord412(delay_strings, 0, filter_delay_strings, 0);
AudioConnection          patchCord413(string_filter_lfo1, 0, string_filter1, 1);
AudioConnection          patchCord414(chords_effect_mix1, 0, chords_main_filter1, 0);
AudioConnection          patchCord415(string_filter_lfo, 0, string_filter, 1);
AudioConnection          patchCord416(string_multiply1, 0, string_filter1, 0);
AudioConnection          patchCord417(string_multiply, 0, string_filter, 0);
AudioConnection          patchCord418(chords_effect_mix, 0, chords_main_filter, 0);
AudioConnection          patchCord419(delay_chords1, 0, filter_delay_chords1, 0);
AudioConnection          patchCord420(delay_chords, 0, filter_delay_chords, 0);
AudioConnection          patchCord421(string_filter1, 0, string_filter_mixer1, 0);
AudioConnection          patchCord422(string_filter1, 1, string_filter_mixer1, 1);
AudioConnection          patchCord423(string_filter1, 2, string_filter_mixer1, 2);
AudioConnection          patchCord424(string_filter, 0, string_filter_mixer, 0);
AudioConnection          patchCord425(string_filter, 1, string_filter_mixer, 1);
AudioConnection          patchCord426(string_filter, 2, string_filter_mixer, 2);
AudioConnection          patchCord427(chords_main_filter1, 0, chords_main_filter_mixer1, 0);
AudioConnection          patchCord428(chords_main_filter1, 1, chords_main_filter_mixer1, 1);
AudioConnection          patchCord429(chords_main_filter1, 2, chords_main_filter_mixer1, 2);
AudioConnection          patchCord430(chords_main_filter, 0, chords_main_filter_mixer, 0);
AudioConnection          patchCord431(chords_main_filter, 1, chords_main_filter_mixer, 1);
AudioConnection          patchCord432(chords_main_filter, 2, chords_main_filter_mixer, 2);
AudioConnection          patchCord433(string_filter_mixer1, 0, string_filter_mixer2, 1);
AudioConnection          patchCord434(string_filter_mixer, 0, string_filter_mixer2, 0);
AudioConnection          patchCord435(chords_main_filter_mixer1, 0, chords_main_filter_mixer2, 1);
AudioConnection          patchCord436(chords_main_filter_mixer, 0, chords_main_filter_mixer2, 0);
// GUItool: end automatically generated code



//MANUAL OUTPUT SECTION
#include "effect_platervbstereo.h"
AudioSynthWaveformDc     string_gain; 
AudioEffectMultiply      string_multiplier;  
AudioAmplifier           string_amplifier; 
AudioSynthWaveformDc     string_l_stereo_gain;        
AudioSynthWaveformDc     string_r_stereo_gain;        
AudioEffectMultiply      string_l_stereo_multiply;  
AudioEffectMultiply      string_r_stereo_multiply;  
AudioSynthWaveformDc     chords_gain;        
AudioEffectMultiply      chords_multiplier;  
AudioAmplifier           chords_amplifier; 
AudioSynthWaveformDc     chords_l_stereo_gain;        
AudioSynthWaveformDc     chords_r_stereo_gain;        
AudioEffectMultiply      chords_l_stereo_multiply;  
AudioEffectMultiply      chords_r_stereo_multiply;  
AudioMixer4              reverb_mixer;   
AudioEffectPlateReverb   main_reverb;    
AudioMixer4              stereo_l_mixer;        
AudioMixer4              stereo_r_mixer;   
AudioOutputI2S           DAC_out;    
AudioOutputUSB           USB_out;      
AudioConnection          patchCord1000(string_filter_mixer, 0, string_filter_mixer2, 0);
AudioConnection          patchCord1001(string_filter_mixer1, 0, string_filter_mixer2, 1);
AudioConnection          patchCord2000(string_filter_mixer2, 0, string_multiplier, 0);
AudioConnection          patchCord2001(string_gain, 0, string_multiplier, 1);
AudioConnection          patchCord2002(string_multiplier, 0, string_amplifier, 0);
AudioConnection          patchCord2003(string_amplifier, 0, string_l_stereo_multiply, 0);
AudioConnection          patchCord2004(string_l_stereo_gain, 0, string_l_stereo_multiply, 1);
AudioConnection          patchCord2005(string_amplifier, 0, string_r_stereo_multiply, 0);
AudioConnection          patchCord2006(string_r_stereo_gain, 0, string_r_stereo_multiply, 1);
AudioConnection          patchCord2007(string_r_stereo_multiply, 0, stereo_r_mixer, 0);
AudioConnection          patchCord2008(string_l_stereo_multiply, 0, stereo_l_mixer, 0);
AudioConnection          patchCord2009(string_amplifier, 0, reverb_mixer, 0);

AudioConnection          patchCord1100(chords_main_filter_mixer, 0, chords_main_filter_mixer2, 0);
AudioConnection          patchCord1101(chords_main_filter_mixer1, 0, chords_main_filter_mixer2, 1);
AudioConnection          patchCord2010(chords_main_filter_mixer2, 0, chords_multiplier, 0);
AudioConnection          patchCord2011(chords_gain, 0, chords_multiplier, 1);
AudioConnection          patchCord2012(chords_multiplier, 0, chords_amplifier, 0);
AudioConnection          patchCord2013(chords_amplifier, 0, chords_l_stereo_multiply, 0);
AudioConnection          patchCord2014(chords_l_stereo_gain, 0, chords_l_stereo_multiply, 1);
AudioConnection          patchCord2015(chords_amplifier, 0, chords_r_stereo_multiply, 0);
AudioConnection          patchCord2016(chords_r_stereo_gain, 0, chords_r_stereo_multiply, 1);
AudioConnection          patchCord2017(chords_r_stereo_multiply, 0, stereo_r_mixer, 1);
AudioConnection          patchCord2018(chords_l_stereo_multiply, 0, stereo_l_mixer, 1);
AudioConnection          patchCord2019(chords_amplifier, 0, reverb_mixer, 1);

AudioConnection          patchCord2020(reverb_mixer, 0, main_reverb, 0);
AudioConnection          patchCord2021(main_reverb, 0, stereo_r_mixer, 2);
AudioConnection          patchCord2022(main_reverb, 1, stereo_l_mixer, 2);

AudioConnection          patchCord2023(stereo_l_mixer, 0, DAC_out, 1);
AudioConnection          patchCord2024(stereo_r_mixer, 0, DAC_out, 0);

AudioConnection          patchCord2025(stereo_l_mixer, 0, USB_out, 1);
AudioConnection          patchCord2026(stereo_r_mixer, 0, USB_out, 0);
