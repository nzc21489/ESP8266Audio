project(ESP8266AudioPico)
        
add_library(fixed STATIC 
LTP_analysis_filter_FIX.c
LTP_scale_ctrl_FIX.c
apply_sine_window_FIX.c
autocorr_FIX.c
burg_modified_FIX.c
corrMatrix_FIX.c
encode_frame_FIX.c
find_LPC_FIX.c
find_LTP_FIX.c
find_pitch_lags_FIX.c
find_pred_coefs_FIX.c
k2a_FIX.c
k2a_Q16_FIX.c
noise_shape_analysis_FIX.c
pitch_analysis_core_FIX.c
process_gains_FIX.c
regularize_correlations_FIX.c
residual_energy16_FIX.c
residual_energy_FIX.c
schur64_FIX.c
schur_FIX.c
vector_ops_FIX.c
warped_autocorrelation_FIX.c
)